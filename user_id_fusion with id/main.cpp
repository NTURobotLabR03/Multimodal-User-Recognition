//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Multimodal User Recognition
//Function: Recognizes a user
//Lib used: OpenCV, Fuzzylite 5.0, Microsoft Kinect SDK, libsvm-3.16

//include the hpp
/*---------------------------
Face:
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

Body:
#include "svm.h"
#include <math.h>
#include <vector>
#include <iostream>  
#include <fstream>

Dress:
#include <cv.h>
#include <highgui.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <windows.h>  
#include <iostream> 
#include <conio.h>

Mulitmodal Fusion:
#include "fl/Headers.h"
#include <vector>
#include <iostream>
---------------------------*/
#include "User_Recog.h" //�Ҧ��]�t��header�ɡA���g�b�W��

using namespace std;
using namespace cv;

user_recog user_recog1;

int main(){
	bool g_track;	//��l��bunny���H�Ҧ����X��
	vector<double> hip_center_value;	//��l��hip center���ƾ�
	user_recog1.var_initial();	//��l��User_Recog.cpp
	user_recog1.user_recog_loop(g_track,hip_center_value);	//�B��multimodal user recogniton
}