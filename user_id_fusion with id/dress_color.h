//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Dress Color modality
//Function: Recognizes a user by dress, the flowchart use of correlation_histogram.cpp
//Lib used: OpenCV
#pragma once
/*
內包
#include <cv.h>
#include <highgui.h>
#include <vector>
#include <iostream>
#include <math.h>
*/
#include "correlation_histogram.h"
#include <windows.h>  
#include <iostream> 
#include <conio.h>

using namespace std;
using namespace cv;

class dress_color{
public:
	//*------------all of functions are call by reference
	void dress_color_initial();	//Dress Color 初始化
	double dress_color_mainloop(vector<int> &, vector<int> &, Mat&, bool&);	//dress color之main function //(input,input,output, output, output)
																			//輸入:(骨架點之x座標，骨架點之y 座標)，輸出:(畫出骨架點之影像，判斷骨架有沒有被偵測之boolean)
private:
	vector<int> point_x;	//骨架點之x座標，骨架點之y 座標
	vector<int> point_y;
	vector<int> v_original;	//轉換成histogram 後，存於此變數 (初始化資料)
	vector<int> v_current;		//轉換成histogram 後，存於此變數 (Kinect 讀取)
	//RAW IMAGE of the kinect
	Mat colorImage_raw;		//存Kinect 讀取之彩色影像
	Mat tracked_image;
	Mat tracked_current_image;
	Mat image_original;
	bool initial_image_capture;	//判斷衣服顏色有沒有initialize
	bool skeleton_check;	//判斷有沒有骨架被偵測
	Hist_match histmatch1;
};