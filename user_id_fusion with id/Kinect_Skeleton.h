//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Kinect Skeleton capture program
//Function: Return the skeleton information
//Lib used: OpenCV, Microsoft Kinect SDK
#pragma once
#include <windows.h>  
#include <iostream>   
#include <NuiApi.h>  
//#include <opencv2/opencv.hpp>  
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class skeleton{
public:
	void skeleton_mainloop(Mat&, vector<int>&);	//Kinect 主要運行之function
	void get_skeleton_feature(int, vector<double>&, vector<int>&, vector<int>&);	//提取body modality 以及 dress modality所需之feature
	void get_skeleton_hip_center(int, vector<double>&);	//提取特定骨架之hip center 座標
    void kinect_initial();	//初始化Kinect
	int whichone;
	bool tracked[NUI_SKELETON_COUNT];   
	int get_tracked_count();

private:
	void getColorImage(HANDLE &, HANDLE &, Mat &);	//這function主要用來提取彩色影像幀
	void getSkeletonImage(HANDLE &, Mat &, Mat &, Mat &, vector<int>&); //這function主要用來提取骨架影像幀
	void drawSkeleton(Mat &, CvPoint pointSet[] , int);    //這function主要用來繪畫骨架
	void getTheContour(Mat &, int , Mat &);	//這function主要用來畫受測骨架之邊緣
	Mat colorImage;   //彩色影像
    Mat depthImage;    //深度影像
    Mat skeletonImage; //骨架影像
    Mat mask;			//遮罩，用於colorImage
	HANDLE colorEvent;   
    HANDLE depthEvent;   
    HANDLE skeletonEvent;   
    HANDLE colorStreamHandle;   
    HANDLE depthStreamHandle;   
    HRESULT hr;
	int skeleton_tracked_count;	//受測骨架總量
};