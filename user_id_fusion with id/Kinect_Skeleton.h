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
	void skeleton_mainloop(Mat&, vector<int>&);	//Kinect �D�n�B�椧function
	void get_skeleton_feature(int, vector<double>&, vector<int>&, vector<int>&);	//����body modality �H�� dress modality�һݤ�feature
	void get_skeleton_hip_center(int, vector<double>&);	//�����S�w���[��hip center �y��
    void kinect_initial();	//��l��Kinect
	int whichone;
	bool tracked[NUI_SKELETON_COUNT];   
	int get_tracked_count();

private:
	void getColorImage(HANDLE &, HANDLE &, Mat &);	//�ofunction�D�n�ΨӴ����m��v���V
	void getSkeletonImage(HANDLE &, Mat &, Mat &, Mat &, vector<int>&); //�ofunction�D�n�ΨӴ������[�v���V
	void drawSkeleton(Mat &, CvPoint pointSet[] , int);    //�ofunction�D�n�Ψ�ø�e���[
	void getTheContour(Mat &, int , Mat &);	//�ofunction�D�n�Ψӵe�������[����t
	Mat colorImage;   //�m��v��
    Mat depthImage;    //�`�׼v��
    Mat skeletonImage; //���[�v��
    Mat mask;			//�B�n�A�Ω�colorImage
	HANDLE colorEvent;   
    HANDLE depthEvent;   
    HANDLE skeletonEvent;   
    HANDLE colorStreamHandle;   
    HANDLE depthStreamHandle;   
    HRESULT hr;
	int skeleton_tracked_count;	//�������[�`�q
};