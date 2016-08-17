//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Kinect Skeleton capture program
//Function: Return the skeleton information
//Lib used: OpenCV, Microsoft Kinect SDK
#include <windows.h>  
#include <iostream>   
#include <NuiApi.h>  
#include <opencv2/opencv.hpp>  
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include "Kinect_Skeleton.h"

using namespace std;
using namespace cv;

bool tracked[NUI_SKELETON_COUNT]={FALSE};   
CvPoint skeletonPoint[NUI_SKELETON_COUNT][NUI_SKELETON_POSITION_COUNT]={cvPoint(0,0)};   
CvPoint colorPoint[NUI_SKELETON_COUNT][NUI_SKELETON_POSITION_COUNT]={cvPoint(0,0)};   
NUI_SKELETON_FRAME skeletonFrame={0};

void skeleton::kinect_initial(){	//initial
	colorImage.create(480, 640, CV_8UC3); //form a 640*480 Mat(color image)   
    depthImage.create(240, 320, CV_8UC3); //form a 320*240 Mat(depth image)
    skeletonImage.create(240, 320, CV_8UC3);   //form a 320*240 Mat(skeleton image)
	mask.create(240, 320, CV_8UC3);   //form a 320*240 Mat (mask for depth image)
	colorEvent = CreateEvent( NULL, TRUE, FALSE, NULL );   
    depthEvent = CreateEvent( NULL, TRUE, FALSE, NULL );   
    skeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );   
   
    colorStreamHandle = NULL;   
    depthStreamHandle = NULL;   
   
	/*------check the kinect is connected or not-------*/
    hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON);
	 if( hr != S_OK )     
    {     
        cout<<"NuiInitialize failed"<<endl;
		exit(-1);
    }   
   
    hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, NULL, 4, colorEvent, &colorStreamHandle);   
    if( hr != S_OK )     
    {     
        cout<<"Open the color Stream failed"<<endl;   
        NuiShutdown();  
		exit(-1);
    }   
    hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_320x240, NULL, 2, depthEvent, &depthStreamHandle);   
    if( hr != S_OK )     
    {     
        cout<<"Open the depth Stream failed"<<endl;   
        NuiShutdown();    
		exit(-1);
    }   
    hr = NuiSkeletonTrackingEnable( skeletonEvent, 0 );//打开骨骼跟踪事件     
    if( hr != S_OK )     
    {     
        cout << "NuiSkeletonTrackingEnable failed" << endl;     
        NuiShutdown();   
		exit(-1);
    }   
}

void skeleton::skeleton_mainloop(Mat &colorImage_Back_up, vector<int> &tracked_id){	//skeleton_mainloop(output image, vector with skeleton id tracked)   //This is main loop
//tracked_id 是一個vector，用來存放已偵測骨架之ID
        if(WaitForSingleObject(colorEvent, 0)==0)   
            getColorImage(colorEvent, colorStreamHandle, colorImage);   
		    
		colorImage_Back_up=colorImage;
	
        //这里使用INFINITE是为了避免没有激活skeletonEvent而跳过此代码出现colorimage频闪的现象   
        tracked_id.clear();	//清空此vector，防呆用
		if(WaitForSingleObject(skeletonEvent, INFINITE)==0)  //偵測骨架
            getSkeletonImage(skeletonEvent, skeletonImage, colorImage, depthImage, tracked_id);   //提出骨架影像 (HANDLE skeletonEvent, image_input, image_output, depth_image_output, vector tracked_id output)
        for (int i=0; i<6; i++)     //check each id of skeleton
        {   
            if(tracked[i] == TRUE)  //tracked[0~5]是用來檢查每個ID是否被偵測狀態，若是->TRUE，若否->否
            {   
				//cout << i << endl;
				mask.setTo(0);  
                getTheContour(depthImage, i, mask);   //取輪廓
                tracked[i] = FALSE; //若該骨架ID被被偵測，傳回FALSE狀態
                break;   
            }
        }   
		
		for (int i=0; i<6; i++){	//防呆用
			tracked[i]=FALSE;
		}
        imshow("skeletonImage", skeletonImage);   
}



void skeleton::getColorImage(HANDLE &colorEvent, HANDLE &colorStreamHandle, Mat &colorImage)	//提取colorImage
{   
    const NUI_IMAGE_FRAME *colorFrame = NULL;   
   
    NuiImageStreamGetNextFrame(colorStreamHandle, 0, &colorFrame);		//提取彩色影像
    INuiFrameTexture *pTexture = colorFrame->pFrameTexture;     
   
    NUI_LOCKED_RECT LockedRect;   
    pTexture->LockRect(0, &LockedRect, NULL, 0);     
   
    if( LockedRect.Pitch != 0 )   
    {   
        for (int i=0; i<colorImage.rows; i++)   
        {  
            uchar *ptr = colorImage.ptr<uchar>(i);  //第i行的指针                    
            //每个字节代表一个颜色信息，直接使用uchar  
            uchar *pBuffer = (uchar*)(LockedRect.pBits) + i * LockedRect.Pitch;  
            for (int j=0; j<colorImage.cols; j++)   
            {   
                ptr[3*j] = pBuffer[4*j];  //内部数据是4个字节，0-1-2是BGR，第4个现在未使用   
                ptr[3*j+1] = pBuffer[4*j+1];   
                ptr[3*j+2] = pBuffer[4*j+2];   
            }   
        }   
    }   
    else   
    {   
        cout<<"捕捉色彩图像出现错误"<<endl;   
    }  
  
    pTexture->UnlockRect(0);   
    NuiImageStreamReleaseFrame(colorStreamHandle, colorFrame );  
}   
   
void skeleton::getSkeletonImage(HANDLE &skeletonEvent, Mat &skeletonImage, Mat &colorImage, Mat &depthImage,vector<int> &tracked_id)	//提取骨架影像
{   
	tracked_id.clear(); //clear the vector tracked_id
	skeleton_tracked_count=0;
	//NUI_SKELETON_FRAME skeletonFrame = {0};  

    bool bFoundSkeleton = false;    
    if(NuiSkeletonGetNextFrame( 0, &skeletonFrame ) == S_OK )     
    {     
        for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )     
        {     
            if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED )   
            {     
                bFoundSkeleton = true;		//此變數用於檢查骨架是否被偵測
                break;   
            }     
        }     
    }   
    else   
    {   
        cout << "没有找到合适的骨骼帧" << endl;   
        return;    
    }   
   
    if( !bFoundSkeleton )     
    {     
        return;    
    }     
    
	//class definition
	_NUI_TRANSFORM_SMOOTH_PARAMETERS NUI1;

	//平滑骨骼帧,消除抖动  
	//Smoothing parameters
	NUI1.fSmoothing=0.9f;
	NUI1.fCorrection=0.4f;
	NUI1.fPrediction=0.2f;
	NUI1.fJitterRadius=1.0f;
	NUI1.fMaxDeviationRadius=0.5f;
    NuiTransformSmooth(&skeletonFrame, NULL);//平滑骨骼帧,消除抖动  
	

    skeletonImage.setTo(0);     
     int track_id;
	 //track_id=-1;

    for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )     
    {     
        if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED &&     
            skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_SHOULDER_CENTER] != NUI_SKELETON_POSITION_NOT_TRACKED)     
        {   
			skeleton_tracked_count++;
			float fx, fy;     
   
            for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++)//所有的坐标转化为深度图的坐标     
            {     
                NuiTransformSkeletonToDepthImage(skeletonFrame.SkeletonData[i].SkeletonPositions[j], &fx, &fy );     //坐标转化为深度图的坐标     
                skeletonPoint[i][j].x = (int)fx;     
                skeletonPoint[i][j].y = (int)fy;     
            }     
   
            for (int j=0; j<NUI_SKELETON_POSITION_COUNT ; j++)     
            {     
                if (skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[j] != NUI_SKELETON_POSITION_NOT_TRACKED)//跟踪点一用有三种状态：1没有被跟踪到，2跟踪到，3根据跟踪到的估计到     
                {     
                    LONG colorx, colory;
                    NuiImageGetColorPixelCoordinatesFromDepthPixel(NUI_IMAGE_RESOLUTION_640x480, 0,    
                    skeletonPoint[i][j].x, skeletonPoint[i][j].y, 0,&colorx, &colory);   
                    colorPoint[i][j].x = int(colorx);  
					//skeleton x axis shift
                    colorPoint[i][j].y = int(colory); //存储坐标点  
                    //circle(colorImage, colorPoint[i][j], 4, cvScalar(0, 255, 255), 1, 8, 0);
                    circle(skeletonImage, skeletonPoint[i][j], 3, cvScalar(0, 255, 255), 1, 8, 0);   
                    tracked[i] = TRUE; 
                }   
            }   
            drawSkeleton(skeletonImage, skeletonPoint[i], i);   
        }
	}   

	for(int i=0;i<6;i++){
		if(tracked[i]==TRUE){
			//cout<<i<<" ";
			tracked_id.push_back(i);
		}
	}

}   

void skeleton::get_skeleton_feature(int track_id, vector<double> &skeleton_length,vector<int> &point_hip_c_x, vector<int> &point_hip_c_y){
	//initial the variable of length features
	double dis_cervical_spine;
	double dis_thoracic_spine;
	double dis_l_forearm;
	double dis_l_arm;
	double dis_r_forearm;
	double dis_r_arm;
	double dis_l_thigh;
	double dis_r_thigh;
	double dis_l_leg;
	double dis_r_leg;
	double height;
	point_hip_c_x.clear();
	point_hip_c_y.clear();
	skeleton_length.clear();
	for(int i=0;i<NUI_SKELETON_POSITION_COUNT;i++){
		point_hip_c_x.push_back(colorPoint[track_id][i].x);  //push back the coordinates of skeleton joint in 2D image
		point_hip_c_y.push_back(colorPoint[track_id][i].y);
	}
	//calculate the length of each part of tracked skeleton, they are calculated by two points of 3D coordinates
	dis_cervical_spine=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z),2));
		//skeleton_length.push_back(dis_cervical_spine);
		dis_thoracic_spine=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x-(skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].x+skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].x)/2),2)+
								pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y-(skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].y+skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].y)/2),2)+
								pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z-(skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].z+skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].z)/2),2));
		//skeleton_length.push_back(dis_thoracic_spine);
		dis_l_forearm=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].x),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].y),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].z),2));
		//skeleton_length.push_back(dis_l_forearm);
		dis_r_forearm=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z),2));
		//skeleton_length.push_back(dis_r_forearm);
		dis_l_arm=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].x),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].y),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].z),2));
		//skeleton_length.push_back(dis_l_arm);
		dis_r_arm=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].x),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].y),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].z),2));
		//skeleton_length.push_back(dis_r_arm);
		dis_l_thigh=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].x),2)+
						    pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].y),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].z),2));
		//skeleton_length.push_back(dis_l_thigh);
		dis_r_thigh=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].x),2)+
						    pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].y),2)+
							pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].z),2));
		//skeleton_length.push_back(dis_r_thigh);
		dis_l_leg=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].x),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].y),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].z),2));
		//skeleton_length.push_back(dis_l_leg);
		dis_r_leg=sqrt(pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].x-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].x),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].y-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].y),2)+
						pow((skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].z-skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].z),2));
		//skeleton_length.push_back(dis_r_leg);
		height=dis_cervical_spine+dis_thoracic_spine+(dis_l_thigh+dis_r_thigh)/2+(dis_l_leg+dis_r_leg)/2;
		//skeleton_length.push_back(height);
		//Push back the features (length and ratio)
		skeleton_length.push_back(dis_thoracic_spine); //push back the length of thoracic spine
		skeleton_length.push_back(dis_l_forearm+dis_l_arm);  //push back the length of left arm
		skeleton_length.push_back(dis_r_forearm+dis_r_arm);  //push back the length of right arm
		skeleton_length.push_back(dis_l_thigh+dis_l_leg);    //push back the length of left leg
		skeleton_length.push_back(dis_r_thigh+dis_r_leg);    //push back the length of right leg
		skeleton_length.push_back(dis_l_forearm/dis_l_arm);  //push back the ratio of upper and lower arm (left
		skeleton_length.push_back(dis_r_forearm/dis_r_arm);   //push back the ratio of upper and lower arm (right
		skeleton_length.push_back(dis_l_thigh/dis_l_leg);    //push back the ratio of upper and lower leg (left
		skeleton_length.push_back(dis_r_thigh/dis_r_leg);    //push back the ratio of upper and lower leg (right
		skeleton_length.push_back((dis_l_thigh+dis_l_leg)/dis_thoracic_spine); //push back the ratio of left leg and thoracic spine
		skeleton_length.push_back((dis_r_thigh+dis_r_leg)/dis_thoracic_spine); //push back the ratio of right leg and thoracic spine
		skeleton_length.push_back(height); //push back of height
}

void skeleton::drawSkeleton(Mat &image, CvPoint pointSet[], int whichone)   
{   
    CvScalar color;   
    switch(whichone) //跟踪不同的人显示不同的颜色   
    {   
    case 0:   
        color = cvScalar(255);   
        break;   
    case 1:   
        color = cvScalar(0,255);   
        break;   
    case 2:   
        color = cvScalar(0, 0, 255);   
        break;   
    case 3:   
        color = cvScalar(255, 255, 0);   
        break;   
    case 4:   
        color = cvScalar(255, 0, 255);   
        break;   
    case 5:   
        color = cvScalar(0, 255, 255);   
        break;   
    }   
   
    if((pointSet[NUI_SKELETON_POSITION_HEAD].x!=0 || pointSet[NUI_SKELETON_POSITION_HEAD].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_HEAD], pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_SPINE].x!=0 || pointSet[NUI_SKELETON_POSITION_SPINE].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SPINE], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_SPINE].x!=0 || pointSet[NUI_SKELETON_POSITION_SPINE].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_SPINE], pointSet[NUI_SKELETON_POSITION_HIP_CENTER], color, 2);   
   
    //左上肢   
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT], pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT], pointSet[NUI_SKELETON_POSITION_WRIST_LEFT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_HAND_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HAND_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_WRIST_LEFT], pointSet[NUI_SKELETON_POSITION_HAND_LEFT], color, 2);   
   
    //右上肢   
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT], pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT], pointSet[NUI_SKELETON_POSITION_HAND_RIGHT], color, 2);   
   
    //左下肢   
    if((pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_HIP_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_CENTER], pointSet[NUI_SKELETON_POSITION_HIP_LEFT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_HIP_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_LEFT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_LEFT], pointSet[NUI_SKELETON_POSITION_KNEE_LEFT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_KNEE_LEFT], pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].y!=0) &&    
        (pointSet[NUI_SKELETON_POSITION_FOOT_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_FOOT_LEFT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT], pointSet[NUI_SKELETON_POSITION_FOOT_LEFT], color, 2);   
   
    //右下肢   
    if((pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_CENTER], pointSet[NUI_SKELETON_POSITION_HIP_RIGHT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_RIGHT], pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT],color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT], pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT], color, 2);   
    if((pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].y!=0) &&   
        (pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT].y!=0))   
        line(image, pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT], pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT], color, 2);   
}   
   
 //根据给定的深度数据的关系（在getDepthImage()中的）确定不同的跟踪目标   
void skeleton::getTheContour(Mat &image, int whichone, Mat &mask)  
{   
    for (int i=0; i<image.rows; i++)   
    {   
        uchar *ptr = image.ptr<uchar>(i);   
        uchar *ptrmask = mask.ptr<uchar>(i);    
        for (int j=0; j<image.cols; j++)   
        {   
            if (ptr[3*j]==0 && ptr[3*j+1]==0 && ptr[3*j+2]==0) //都为0的时候予以忽略   
            {   
                ptrmask[3*j]=ptrmask[3*j+1]=ptrmask[3*j+2]=0;   
            }else if(ptr[3*j]==0 && ptr[3*j+1]==0 && ptr[3*j+2]!=0)//ID为1的时候，显示绿色   
            {   
                ptrmask[3*j] = 0;   
                ptrmask[3*j+1] = 255;   
                ptrmask[3*j+2] = 0;   
            }else if (ptr[3*j]==0 && ptr[3*j+1]!=0 && ptr[3*j+2]==0)//ID为2的时候，显示红色   
            {   
                ptrmask[3*j] = 0;   
                ptrmask[3*j+1] = 0;   
                ptrmask[3*j+2] = 255;   
            }else if (ptr[3*j]==ptr[3*j+1] && ptr[3*j]==4*ptr[3*j+2])//ID为3的时候   
            {   
                ptrmask[3*j] = 255;   
                ptrmask[3*j+1] = 255;   
                ptrmask[3*j+2] = 0;   
            }else if (4*ptr[3*j]==ptr[3*j+1] && ptr[3*j+1]==ptr[3*j+2])//ID为4的时候   
            {   
                ptrmask[3*j] = 255;   
                ptrmask[3*j+1] = 0;   
                ptrmask[3*j+2] = 255;   
            }else if (ptr[3*j]==4*ptr[3*j+1] && ptr[3*j]==ptr[3*j+2])//ID为5的时候   
            {   
                ptrmask[3*j] = 0;   
                ptrmask[3*j+1] = 255;   
                ptrmask[3*j+2] = 255;   
            }else if (ptr[3*j]==2*ptr[3*j+1] && ptr[3*j+1]==ptr[3*j+2])//ID为6的时候   
            {   
                ptrmask[3*j] = 255;   
                ptrmask[3*j+1] = 255;   
                ptrmask[3*j+2] = 255;   
            }else if (ptr[3*j]==ptr[3*j+1] && ptr[3*j]==ptr[3*j+2])//ID为7的时候或者ID为0的时候，显示蓝色   
            {   
                ptrmask[3*j] = 255;   
                ptrmask[3*j+1] = 0;   
                ptrmask[3*j+2] = 0;   
            }/*else   
            {   
                cout <<"如果输出这段代码，说明有遗漏的情况，请查询getTheContour函数" << endl;   
            } */  
        }   
    }   
}  

//This function is used for tracked the count of skeleton tracked
int skeleton::get_tracked_count(){
	return skeleton_tracked_count;
}

void skeleton::get_skeleton_hip_center(int track_id, vector<double>& hip_center_coordinate){ //this function is used for return the 3D coordinates of target's skeleton
	hip_center_coordinate.clear();
	hip_center_coordinate.push_back(skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x);
	hip_center_coordinate.push_back(skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y);
	hip_center_coordinate.push_back(skeletonFrame.SkeletonData[track_id].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z);
}