//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: correlation_histogram.cpp
//Function: calculate the similarity of histogram
//Lib used: OpenCV
#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

class Hist_match{
	//*------------all of functions are call by reference
public:
	void vector_push(Mat& , vector<int> &); //��X��J�v����histogram�A�ÿ�X��vector //(input,output)
	double matching(vector<int> &, vector<int> &); //histogram matching //(input, input)
												   //��J: initialize����A����ϼƾڡA��J: Kinect��������A����ϼƾ�
private:
	int divided_histogram;   //8-elements, density of corrsponding histogram level
	float original_pixel_density[2][8];	//�x�s��ldress color��Ƥ�histogram [2]��H-S�Ŷ��A[8]����8������0~255
	float current_pixel_density[2][8];	//�x�sKinect���착�[joint�g��v����histogram
	double vector_product;	//�p����ܼ�
	double confirmity;		//�p��ۦ���
};