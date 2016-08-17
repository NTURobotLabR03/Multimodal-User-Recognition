//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Face modality
//Function: Recognizes a user's Face, the flowchart use of the face_model.cpp
//Lib used: OpenCV
#pragma once
/*
���]
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
*/
#include "face_match.h"

class face_model{
public:
	//*------------all of functions are call by reference
	void face_model_initial();	//��l��face model�A���thaar feature��Ū�ɡA�H�ΤH�y�ҪO��Ū��
	void face_model_loop(Mat&, int& ,double&);	//face model �D�B��function		//(input, output, output)
												//��J: �v���A��X: prediction�H��confidence
private:
	int prediction;	//predicted label
	double confidence;	//�H�y�v����template�v�����ڤ�Z��
	// These vectors hold the images and corresponding labels:
	vector<Mat> images;
    vector<int> labels;

	string fn_haar;	//haar feature��xml�ɦW
    string fn_csv;	//csv �ɦW
	Mat frame;
	//�v���j�p
	int im_width;
    int im_height;

	//��l��face_match.cpp��class
	Face_Match face_match1;
	
	//��l��haar cascsade
	CascadeClassifier haar_cascade;
	
	//��l��model
	Ptr<FaceRecognizer> model;
};