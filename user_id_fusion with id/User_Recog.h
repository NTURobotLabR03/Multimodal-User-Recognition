//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Multimodal User Recognition
//Function: Recognizes a user by multimodal
//Lib used: OpenCV, Fuzzylite 5.0, Microsoft Kinect SDK, libsvm-3.16
#include "Kinect_Skeleton.h"
#include "body_size_forward.h"
#include "dress_color.h"
#include "face_model.h"
#include "fuzzy_forward.h"

using namespace cv;

class user_recog{
public:
	//*--------------all of functions are call by references
	void var_initial();	//這function是用來初始化各種變數
	void user_recog_loop(bool&, vector<double>&); //進入使用者辨識，輸出g_track旗標以及hip_center 之vector 輸出
private:
	
	skeleton skeleton1;
	dress_color dress_color1;
	svm_forward svm_forward1;
	face_model face_model1;
	fuzzy_forward fuzzy_forward1;

/*--------variable of each modality------------*/
	//Kinect Variable
	Mat colorImage_raw;
	vector<int> point_x;
	vector<int> point_y;
	vector<int> tracked_id;
	vector<double> skeleton_size;
	int accepted_id;
	bool skeleton_check;
	int tracked_count;

	
	//Face variable
	Mat frame;
	int prediction;
	double face_confidence;

	//Dress Variable
	double dress_similarity;

	//Body Variable
	double body_predict;
	double body_predict_probability;

	//Fuzzy lite Variable
	double fuzzy_fusion_similarity;

};