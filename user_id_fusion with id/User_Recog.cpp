//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Multimodal User Recognition
//Function: Recognizes a user by multimodal
//Lib used: OpenCV, Fuzzylite 5.0, Microsoft Kinect SDK, libsvm-3.16
#include "User_Recog.h"
/*
內包
#include "Kinect_Skeleton.h"
#include "body_size_forward.h"
#include "dress_color.h"
#include "face_model.h"
#include "fuzzy_forward.h"
*/

//This function is used for display the score bar
void display_img(double face_distance, double dress_match, double body_match, double fusion){
	Mat display_image = Mat::zeros( 480, 640, CV_8UC3 );
	
	Scalar c_title = cv::Scalar(0,0,255);
	Scalar c_elem  = cv::Scalar(200,255,0);
	
    putText(display_image,"Multimodal User Identification" , Point(20,40),FONT_HERSHEY_COMPLEX,0.8,c_title);	//輸入文字
    putText(display_image,"Face" , Point(20,120),FONT_HERSHEY_DUPLEX,0.7,c_elem);
	putText(display_image,"Dress" , Point(20,220),FONT_HERSHEY_DUPLEX,0.7,c_elem);
	putText(display_image,"Body" , Point(20,320),FONT_HERSHEY_DUPLEX,0.7,c_elem);
	putText(display_image,"Fusion" , Point(20,420),FONT_HERSHEY_DUPLEX,0.7,c_elem);

	for(int i=0;i<6;i++){
		line(display_image,Point(125+i*100,100),Point(125+i*100,440),Scalar(255,255,255),0.5);		//畫分割線
	}

	//Face
	line(display_image,Point(125,120),Point(125+500-face_distance*500,120),Scalar(0,0,255),5);		//畫face score bar

	//Dress
	line(display_image,Point(125,220),Point(125+dress_match*500,220),Scalar(0,0,255),5);  //畫dress score bar

	//Body
	line(display_image,Point(125,320),Point(125+body_match*500,320),Scalar(0,0,255),5);		//畫body score bar

	//Fusion
	line(display_image,Point(125,420),Point(125+fusion*500,420),Scalar(0,0,255),5);		//畫fusion score bar

	imshow("display_image",display_image);
	display_image.release();
}

void user_recog::var_initial(){
	//class initialize
	face_model1.face_model_initial();
	skeleton1.kinect_initial();  
	dress_color1.dress_color_initial();
	svm_forward1.svm_forward_initial();
	fuzzy_forward1.fuzzy_initial();
	fuzzy_forward1.fuzzy_rule();
	fuzzy_forward1.defuzz_initial();

	//設定初始值
	//Kinect Variable
	accepted_id=-1;
	skeleton_check=0;

	//Face variable
	prediction=-1;
	face_confidence=1000.0;

	//Dress Variable
	dress_similarity=0.0;

	//Body Variable
	body_predict=-1;
	body_predict_probability=0.0;

	//Fuzzy lite Variable
	fuzzy_fusion_similarity=0.0;

	//clear all the vector
	tracked_id.clear();
	point_x.clear();
	point_y.clear();
}

void user_recog::user_recog_loop(bool& g_track, vector<double>& hip_center_value){
	//Camera初始化
	int deviceId = atoi("0"); 
	VideoCapture cap(deviceId);
	//if no camera checked, display the warning message
	if(!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		exit(-1);
	}
	
	//this is the boolean flag for Bunny, 1=Bunny follow the user, 0=following mode disabled
	g_track=0;

	//loop
	while(1){
		skeleton1.skeleton_mainloop(colorImage_raw,tracked_id); //跑Kinect，回傳影像以及已偵測骨架之ID
		if(g_track==0){ //g_track=0->Bunny不跟人，g_track=1->Bunny跟人
			cap>>frame;   //從camera輸入影像
			if(!tracked_id.empty()){		//檢查是否有骨架被偵測
				skeleton_check=1;			//若骨架被查出，skeleton_check=1，給dress_color.cpp做參考
				tracked_count=skeleton1.get_tracked_count();	
				face_model1.face_model_loop(frame,prediction,face_confidence);	//Face modality之prediction
				if(prediction!=0){	//若偵測label非0，confidence統一設定為1000
					face_confidence=1000.0;
				}
				for(int i=0;i<tracked_id.size();i++){
					skeleton1.get_skeleton_feature(tracked_id[i],skeleton_size,point_x,point_y);	//提取Kinect_Skeleton.cpp傳回之body modality feature以及dress color.cpp需用到的骨架點座標
					dress_similarity=dress_color1.dress_color_mainloop(point_x,point_y,colorImage_raw,skeleton_check); //dress color similarity calculation
					svm_forward1.svm_forward_predict(skeleton_size,body_predict,body_predict_probability);	//body modality similarity calculation
					
					//Fuzzy Fusion (Mulitmodal Fusion)
					fuzzy_forward1.fuzzy_output(face_confidence/1000.0,body_predict_probability,dress_similarity,fuzzy_fusion_similarity); //fuzzy output(face_input,body_input,dress_input,fusion_similarity_output)

					if(fuzzy_fusion_similarity>=0.74){	//若受測使用者之similarity超過0.75，啓動跟人flag g_track=1
						//accepted_id is used for record the skeleton id of specific target
						accepted_id=tracked_id[i];	//特定使用者之骨架ID存至accepted_id，用來回傳hip_center資訊用
						g_track=1;
						frame.release();	//關閉其他視窗
						destroyWindow("Kinect Image");
						destroyWindow("skeletonImage");
						destroyWindow("face_recognizer");
						destroyWindow("display_image");

					}else{
						accepted_id=-1;
					}
				}
					//cout<<tracked_id.size()<<endl;
					//cout<<tracked_count<<endl;
			}else{		//若沒有骨測骨架，其他資訊需被清空
				skeleton_check=0;
				tracked_count=0;
				accepted_id=-1;
				hip_center_value.clear();
				for(int i=0;i<3;i++){
					hip_center_value.push_back(-1);
				}
			}
		}

		if(g_track==1){			//g_track=1 ->  following mode enabled
			skeleton1.get_skeleton_hip_center(accepted_id,hip_center_value);	//使用上一組if-then的accepted_id並輸入至kinect_skeleton.cpp內，提取特定骨架之hip center 座標
			cout<<hip_center_value[2]<<" "<<hip_center_value[0]<<endl;
		}else{
			hip_center_value.clear();
		}
		display_img(face_confidence/1000, dress_similarity, body_predict_probability, fuzzy_fusion_similarity);	//輸入至display_img function 內作顯示，(face_input, dress_input,body_input, fusion_similarity_input)
		cout<<"Accepted_ID: "<<accepted_id<<endl;
		cout<<"track state:"<<g_track<<endl;
		cout<<endl;
		imshow("Kinect Image",colorImage_raw);
		if(cvWaitKey(1)==27)   
            break;   
	}
	NuiShutdown();
	exit(0);
}