//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Face modality
//Function: Recognizes a user's Face
//Lib used: OpenCV
#pragma once
//#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/objdetect/objdetect.hpp"
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

class Face_Match{
public:
	//*------------all of functions are call by reference
	static void read_csv(const string&, vector<Mat>&, vector<int>&);	//Ū��CSV�A�ΨӾɤJ�H�y��template�v���H��label�A(��J)�ɦW�r��B(��X):�v�� �Blabel)  //(input, output, output)
	void face_match(Ptr<FaceRecognizer>&,Mat&,vector< Rect_<int> >&,int& ,int& ,int& ,int& ,double&);	//camera���줧�H�y��T��template�@���   //(input, input, input, input, input, input, output, output)
																										//��J: (facerecognizer��Ptr�A�v���A�I�����H�y�v���A�����H�y��ID�A�v�����סA�v�����)�A��X: (�w��label�Aconfidence)
	void training_model(Ptr<FaceRecognizer>&,CascadeClassifier &,string,vector<Mat>,vector<int>);	//�V�mface recognizer		//(input, input, input, input)
																									//��J: (facerecognizer��Ptr�AHaar Feature�Ahaar feature���ɦW�A�ҪO�v���Alabel)
private:

};