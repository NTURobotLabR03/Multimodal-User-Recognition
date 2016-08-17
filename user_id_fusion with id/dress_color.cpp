//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Dress Color modality
//Function: Recognizes a user by dress, the flowchart use of correlation_histogram.cpp
//Lib used: OpenCV
#include "dress_color.h"

//this function is offset the joint of skeleton if the skeleton point 
//input the rows and cols of image, and the vector of tracked skeleton point
void point_offset(int& image_rows, int& image_cols, int& point_x, int& point_y){
	if(point_x<2){ //if the skeleton point is out of range of image (left), turn it back to the image
		point_x=2;
	}
	else if(point_x > (image_cols-3)){  //if the skeleton point is out of range of image (right), turn it back to the image
		point_x=image_cols-3;
	}
	if(point_y<2){ //if the skeleton point is out of range of image (up), turn it back to the image
		point_y=2; 
	}
	else if(point_y > (image_rows-3)){ //if the skeleton point is out of range of image (down), turn it back to the image
		point_y=image_rows-3;
	}
}

void dress_color::dress_color_initial(){
	initial_image_capture=0;
}

//mainloop of the dress color modality
double dress_color::dress_color_mainloop(vector<int> &point_x, vector<int> &point_y, Mat& colorImage_raw, bool& skeleton_check){
	double confirmity=0; //initialize
	if(skeleton_check==1){ //this variable is used for checking the skeleton tracked or not, 1=tracked, 0=not tracked
			if(kbhit() && initial_image_capture==0){ //checking the initialize dress data captured or not, if yes, initial_image_capture=1
					for(int i=0;i<20;i++){
						point_offset(colorImage_raw.rows, colorImage_raw.cols, point_x[i], point_y[i]); //checking the skeleton joint is offset or not, if yes, offset the skeleton point

						Rect rect(point_x[i]-2,point_y[i]-2, 5, 5); //capture 25 pixels around the skeleton point
						colorImage_raw(rect).copyTo(tracked_image); //input the 25 pixels image into tracked_image Mat
						histmatch1.vector_push(tracked_image, v_original); //form the 8 divided histogram information and input to the vector v_original
					}
				initial_image_capture=1; //changed the initial_image_capture flag
				//cout<<"Initial Data Captured"<<endl;
				//cout<<v_original.size()<<endl;
			}

			if(initial_image_capture==1){ //if initial dress color information captured, get in this function
				for(int i=0;i<20;i++){ //each skeleton point
					if(point_x[i]<=0 && point_y[i]<=0){
						//if one of the skeleton pt is not detected, clear the vector and break the loop
						v_current.clear();
						point_x[i]=NULL;
						point_y[i]=NULL;
						break;
					}else{
						//Point offset if the skeleton point get out of the frame
						point_offset(colorImage_raw.rows, colorImage_raw.cols, point_x[i], point_y[i]);
						
						//cout << point_x[i] << " " << point_y[i] << endl << endl;
						Rect rect(point_x[i]-2,point_y[i]-2, 5, 5); //capture 25 pixels around the skeleton point
						colorImage_raw(rect).copyTo(tracked_current_image); //input the 25 pixels image into tracked_current_image Mat
						//cout<<point_x[i]<<" "<<point_y[i]<<endl;
						histmatch1.vector_push(tracked_current_image, v_current); //form the 8 divided histogram information and input to the vector v_original
					}
				}
				if(v_current.size()!=0){ //check the dress color data is captured by Kinect or not
					confirmity=histmatch1.matching(v_original,v_current);    //calculate the similarity
					//cout<<"The confirmity of Template and test is "<<confirmity*100<<"%"<<endl;
					v_current.clear();
				}else{
					cout<<"Size error"<<endl;
				}
			}
		}else{
			//point_x=NULL;
			point_x.clear();
			//point_y=NULL;
			point_y.clear();
			v_current.clear();
			confirmity=0;
		}
		//imshow("Image_original",image_original);
		//imshow("Tracked_image",tracked_image);
		//cout<<skeleton_check<<endl; 
		return confirmity;
}