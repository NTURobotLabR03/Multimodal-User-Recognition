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
	void vector_push(Mat& , vector<int> &); //算出輸入影像之histogram，並輸出至vector //(input,output)
	double matching(vector<int> &, vector<int> &); //histogram matching //(input, input)
												   //輸入: initialize之衣服直方圖數據，輸入: Kinect接收之衣服直方圖數據
private:
	int divided_histogram;   //8-elements, density of corrsponding histogram level
	float original_pixel_density[2][8];	//儲存初始dress color資料之histogram [2]為H-S空間，[8]為割8等分的0~255
	float current_pixel_density[2][8];	//儲存Kinect測到骨架joint週邊影像之histogram
	double vector_product;	//計算用變數
	double confirmity;		//計算相似度
};