//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Face modality
//Function: Recognizes a user's Face
//Lib used: OpenCV

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include "face_match.h"

using namespace cv;
using namespace std;

void Face_Match::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels) {//this function is used for read the CSV file which contains the link of the template face image and label
    std::ifstream file(filename.c_str(), ifstream::in); //read file
	//if the file cannot read, output the error message
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
	//read the line until the end of file
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, ';');
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));			//push back the image data into (vector images)
            labels.push_back(atoi(classlabel.c_str()));	//push back the label data into (vector labels)
        }
    }
}

//This function is used for training the face recognizer
void Face_Match::training_model(Ptr<FaceRecognizer>& model,CascadeClassifier &haar_cascade,string fn_haar,vector<Mat> images,vector<int> labels){
	int num_components=1;
	double threshold=1000.0; //setup the threshold
	model = createFisherFaceRecognizer(num_components,threshold);
    //Ptr<FaceRecognizer> model = createEigenFaceRecognizer(num_components,threshold); 
	//Ptr<FaceRecognizer> model = createLBPHFaceRecognizer(); 
	model->train(images, labels);
    // That's it for learning the Face Recognition model. You now
    // need to create the classifier for the task of Face Detection.
    // We are going to use the haar cascade you have specified in the
    // command line arguments:
    //

	//讀取haar features
    haar_cascade.load(fn_haar);
};

//This function is used for matching the face
void Face_Match::face_match(Ptr<FaceRecognizer>& model,Mat &gray,vector< Rect_<int> >& faces,int& i,int& im_width,int& im_height,int& prediction, double& confidence){
	 // Process face by face:
      Rect face_i = faces[i];
     // Crop the face from the image. So simple with OpenCV C++:
      Mat face = gray(face_i);	//灰階化
            // Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
            // verify this, by reading through the face recognition tutorial coming with OpenCV.
            // Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
            // input data really depends on the algorithm used.
            //
            // I strongly encourage you to play around with the algorithms. See which work best
            // in your scenario, LBPH should always be a contender for robust face recognition.
            //
            // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            // face you have just found:
      Mat face_resized;
      cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC); //把臉部影像大小resize 到template之影像大小
	  //face prediction
	  model->predict(face_resized, prediction, confidence);
}
