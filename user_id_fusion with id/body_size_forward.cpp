//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Body modality
//Function: Recognizes a user in Body size and limb ratio
//Lib used: libsvm-3.16

//include the hpp
/*
內包
#include "svm.h"
#include <math.h>
#include <vector>
#include <iostream>  
#include <fstream>
*/
#include "body_size_forward.h"

using namespace std;
//Setup the const, there are 12 features in this modality
const int FEATRUE_NUM=12;	//Feature數目不同時，記得改!!!!

//read the model and setup the svm_node
svm_model *model = svm_load_model("modle.txt");
//this is the libsvm function
svm_node *x_space = new svm_node[FEATRUE_NUM+1];

//this is the function of libsvm, used for read the range file for the data normalization
void svm_forward::svm_forward_read_scale(){
	ifstream file_i;
	int index;
	double _v_min;
	double _v_max;

	file_i.open("training_data_0719_svm.txt.range",ios::in); //讀取正規化所需之最大值以及最小值
	if(!file_i){ //if no txt, display the warning message
		cout<<"Read File error"<<endl;
		exit(-1);
	}

	char x;
	file_i>>x;

	file_i>>min_label;
	file_i>>max_label;

	//read the txt until the end of file
	while(!file_i.eof()){
		file_i>>index;    //the index of feature
		file_i>>_v_min;   //the min value of corresponding feature
		v_min.push_back(_v_min);
		file_i>>_v_max;  //the max value of corresponding feature
		v_max.push_back(_v_max);
	}

	v_min.pop_back();
	v_max.pop_back();

	file_i.close();
}

//initial
void svm_forward::svm_forward_initial(){
	v_min.clear();
	v_max.clear();
	svm_forward_read_scale();
}

//normalization
double svm_forward::svm_scale_data(double& v_old, int i){
	double v_new;
	v_new= 2 * (v_old-v_min[i])/(v_max[i]-v_min[i]) - 1; //normalization algorithm, mapping to the range -1 to 1
	return v_new;
}

//svm prediction
void svm_forward::svm_forward_predict(vector<double>& point_list,double &label_predict1,double &predict1_probability){
	vector<double> point_list_new;
	double v_new_push_back;
	point_list_new.clear();

	for(int i=0;i<FEATRUE_NUM;i++){
		//point_list = the features of body size and ratio
		v_new_push_back=svm_scale_data(point_list[i],i);  //normalization and save the normalized feature data into v_new
		point_list_new.push_back(v_new_push_back);
	}
	
	//this part is used for transform the vector v_new into variable x
	for(int i =0 ;i<FEATRUE_NUM;i++)
	{
		x_space[i].index = i+1;
		x_space[i].value = point_list_new[i];
	}
	x_space[FEATRUE_NUM].index = -1; //essential, refers for examples of libsvm

	double *ProbVal = new double[1];
	label_predict1=svm_predict_probability(model,x_space,ProbVal); //SVM prediction and output ProbVal
	predict1_probability=*ProbVal; //把 predict後之機率回傳至 predict1_probability
}