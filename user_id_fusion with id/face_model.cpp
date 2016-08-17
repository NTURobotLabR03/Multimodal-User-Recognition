//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Face modality
//Function: Recognizes a user's Face, the flowchart use of the face_model.cpp
//Lib used: OpenCV
#include "face_match.h"
#include "face_model.h"

void face_model::face_model_initial(){	//initial the basic variable
	// Initialize the variable
	prediction=-1;
	confidence=1000.0;
	// Get the path to your CSV:
	string fn_haar = string("haarcascade_frontalface_alt_tree.xml");//讀haar features
	string fn_csv = string("csv6.txt");								//讀取csv，人臉template jpg之路徑以及label
	 
	try {	// Read in the data (fails if no valid input filename is given, but you'll get an error message):
        face_match1.read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
	  // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size AND we need to reshape incoming faces to this size:
	im_width = images[0].cols;
    im_height = images[0].rows;

	// Create a FaceRecognizer and train it on the given images:
	face_match1.training_model(model,haar_cascade,fn_haar,images,labels); //run the face recognizer model training(in face_match.cpp)
}

void face_model::face_model_loop(Mat& frame, int& prediction, double& confidence){
		//Copy the frame Mat
		Mat original = frame.clone();
        // Convert the current frame to grayscale:
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        // Find the faces in the frame:
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);
        // At this point you have the position of the faces in
        // faces. Now we'll get the faces, make a prediction and
        // annotate it in the video. Cool or what?
        for(int i = 0; i < faces.size(); i++) {
			 Rect face_i = faces[i];	//在影像找到人臉後，把該區域之影像存入face_i
			//model->predict(face_resized, prediction, confidence);
			face_match1.face_match(model,gray,faces,i,im_width,im_height,prediction,confidence); //input the trained model, captured image, template faces vector, number of faces tracked, size of image, and output the prediction and confidence
            // And finally write all we've found out to the original image!
            // First of all draw a green rectangle around the detected face:
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            // Create the text we will annotate the box with:
            string box_text = format("Prediction = %d  Confidence = %.2f", prediction, confidence);
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            // And now put it into the image:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
			//cout<<prediction<<" "<<confidence<<endl;
        }
        // Show the result:
        imshow("face_recognizer", original);
}