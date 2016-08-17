//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Dress Color modality
//Function: Recognizes a user by dress, the flowchart use of correlation_histogram.cpp
//Lib used: OpenCV
#pragma once
/*
���]
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
	void dress_color_initial();	//Dress Color ��l��
	double dress_color_mainloop(vector<int> &, vector<int> &, Mat&, bool&);	//dress color��main function //(input,input,output, output, output)
																			//��J:(���[�I��x�y�СA���[�I��y �y��)�A��X:(�e�X���[�I���v���A�P�_���[���S���Q������boolean)
private:
	vector<int> point_x;	//���[�I��x�y�СA���[�I��y �y��
	vector<int> point_y;
	vector<int> v_original;	//�ഫ��histogram ��A�s���ܼ� (��l�Ƹ��)
	vector<int> v_current;		//�ഫ��histogram ��A�s���ܼ� (Kinect Ū��)
	//RAW IMAGE of the kinect
	Mat colorImage_raw;		//�sKinect Ū�����m��v��
	Mat tracked_image;
	Mat tracked_current_image;
	Mat image_original;
	bool initial_image_capture;	//�P�_��A�C�⦳�S��initialize
	bool skeleton_check;	//�P�_���S�����[�Q����
	Hist_match histmatch1;
};