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
	static void read_csv(const string&, vector<Mat>&, vector<int>&);	//讀取CSV，用來導入人臉之template影像以及label，(輸入)檔名字串、(輸出):影像 、label)  //(input, output, output)
	void face_match(Ptr<FaceRecognizer>&,Mat&,vector< Rect_<int> >&,int& ,int& ,int& ,int& ,double&);	//camera收到之人臉資訊跟template作比較   //(input, input, input, input, input, input, output, output)
																										//輸入: (facerecognizer之Ptr，影像，截取之人臉影像，偵測人臉之ID，影像高度，影像闊度)，輸出: (預測label，confidence)
	void training_model(Ptr<FaceRecognizer>&,CascadeClassifier &,string,vector<Mat>,vector<int>);	//訓練face recognizer		//(input, input, input, input)
																									//輸入: (facerecognizer之Ptr，Haar Feature，haar feature之檔名，模板影像，label)
private:

};