//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: correlation_histogram.cpp
//Function: calculate the similarity of histogram
//Lib used: OpenCV
#include "correlation_histogram.h"

using namespace std;
using namespace cv;

void Hist_match::vector_push(Mat &image_original, vector<int> &v_original){	//算出輸入影像之histogram，並輸出至vector
	divided_histogram=8;
	for(int i=0;i<2;i++){ //H-S channel
		for(int j=0;j<divided_histogram;j++){	//divided histogram
			original_pixel_density[i][j]=0;	//histogram資訊歸0
			current_pixel_density[i][j]=0;
		}
	}
	vector_product=0;
	confirmity=0;
	Mat image_original_hsv;
	cvtColor( image_original, image_original_hsv, COLOR_BGR2HSV );
	
		//H-S channel
		for(int k=0;k<2;k++){
			//Image rows
			for(int i=0;i<image_original_hsv.rows;i++){
				//Image cols
				for(int j=0;j<image_original_hsv.cols;j++){
					//Divided Histogram
					for(int l=0;l<divided_histogram;l++){
						//If the value of corrsponding pixel is in a range, count++ at the corrsponding array which can form a histogram
						Vec3b intensity_original = image_original_hsv.at<Vec3b>(i, j);
						if(intensity_original.val[k]>=0+255/divided_histogram*l && intensity_original.val[k]<=255/divided_histogram+255/divided_histogram*l){
							original_pixel_density[k][l]++;
						}
					}
				}
			}
		}

		//Push_back the vector
		for(int j=0;j<2;j++){
			for(int i=0;i<divided_histogram;i++){
				v_original.push_back(original_pixel_density[j][i]);
			}
		}
}

double Hist_match::matching(vector<int> &v_original, vector<int> &v_current){	//計算相似度，(input initial dress color, input current dress color)
	//Calculate Z'(t)Z(0)
	for(int i=0;i<v_original.size();i++){
		vector_product+=v_original[i]*v_current[i];
	}

		//Calculate the confirmity   Z'(t)Z(0)/(||Z(t)||*||Z(0)||)
	confirmity=vector_product/(norm(v_original)*norm(v_current));
	//cout<<norm(v_original)<<" "<<norm(v_current)<<" "<<vector_product<<" "<<endl;
	//imshow("Original",image_original);
	//imshow("Current",image_current);
	return confirmity;	//計算相似度並回傳
}