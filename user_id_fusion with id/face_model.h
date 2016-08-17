//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Face modality
//Function: Recognizes a user's Face, the flowchart use of the face_model.cpp
//Lib used: OpenCV
#pragma once
/*
內包
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
*/
#include "face_match.h"

class face_model{
public:
	//*------------all of functions are call by reference
	void face_model_initial();	//初始化face model，內含haar feature之讀檔，以及人臉模板的讀檔
	void face_model_loop(Mat&, int& ,double&);	//face model 主運行function		//(input, output, output)
												//輸入: 影像，輸出: prediction以及confidence
private:
	int prediction;	//predicted label
	double confidence;	//人臉影像跟template影像之歐氏距離
	// These vectors hold the images and corresponding labels:
	vector<Mat> images;
    vector<int> labels;

	string fn_haar;	//haar feature之xml檔名
    string fn_csv;	//csv 檔名
	Mat frame;
	//影像大小
	int im_width;
    int im_height;

	//初始化face_match.cpp之class
	Face_Match face_match1;
	
	//初始化haar cascsade
	CascadeClassifier haar_cascade;
	
	//初始化model
	Ptr<FaceRecognizer> model;
};