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
	void svm_forward_initial();	//body modality ��l��
	void svm_forward_predict(vector<double> &,double& ,double&); //�����w���N�����[���ۦ��� //(input, output, output)
																 //��J:�s��feature��vector�A��X: prediction label �H�Ψ�simiarity
private:
	void svm_forward_read_scale();	//Ū�� ��Kinect���줧���[�ƾڥ��W�Ʃһݭn���ܼơA���t���W�Ʃһ��ܼƤ�Ū���ɦW�A�n���ɦW�b�o�Ө禡����
	double svm_scale_data(double&, int);	//���W�� //(input,input)
											//�ofunction�����|�^�Ǥ@�ӥ��W�ƫ᪺��
											//��J: Feature���ȡA��J: ��i��feature
	int min_label;	//�x�s�Yfeature���̤p��
	int max_label;  //�x�s�Yfeature���̤j��
	vector<double> v_min;	//�x�s���W�Ʃһ� ��feature���̤p��
	vector<double> v_max;	//�x�s���W�Ʃһ� ��feature���̤j��
	//const int FEATRUE_NUM;	//�bcpp�ئ��w�q�Afeature�`�Ʀ����ܮɡA�O�o��!!
};