//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Body modality
//Function: Recognizes a user in Body size and limb ratio
//Lib used: libsvm-3.16
#pragma once
#include "svm.h"
#include <math.h>
#include <vector>
#include <iostream>  
#include <fstream>
using namespace std;

class svm_forward{
public:
	void svm_forward_initial();	//body modality 初始化
	void svm_forward_predict(vector<double> &,double& ,double&); //直接預測就測骨架之相似度 //(input, output, output)
																 //輸入:存有feature之vector，輸出: prediction label 以及其simiarity
private:
	void svm_forward_read_scale();	//讀取 把Kinect收到之骨架數據正規化所需要的變數，內含正規化所需變數之讀檔檔名，要改檔名在這個函式內改
	double svm_scale_data(double&, int);	//正規化 //(input,input)
											//這function本身會回傳一個正規化後的值
											//輸入: Feature之值，輸入: 第i個feature
	int min_label;	//儲存某feature之最小值
	int max_label;  //儲存某feature之最大值
	vector<double> v_min;	//儲存正規化所需 該feature之最小值
	vector<double> v_max;	//儲存正規化所需 該feature之最大值
	//const int FEATRUE_NUM;	//在cpp裏有定義，feature總數有改變時，記得改!!
};