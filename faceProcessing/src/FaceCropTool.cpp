///////////////////////////////////////////////////////////////////////////////////////////////////////
/// DemoDetector.cpp
/// 
/// Description:
/// This program shows you how to use FaceAlignment class in detecting facial landmarks on one image. 
///
/// There are two modes: INTERACTIVE, AUTO.
///
/// In the INTERACTIVE mode, the user is asked to create a draggable rectangle to locate one's face. 
/// To obtain good performance, the upper and lower boundaries need to exceed one's eyebrow and lip. 
/// For examples of good input rectangles, please refer to "../data/good_input_rect.jpg".
///
/// In the AUTO mode, the faces are found through OpenCV face detector.
///
/// Note that the initialization is optimized for OpenCV face detector. However, the algorithm is not
/// very sensitive to initialization. It is possible to replace OpenCV's with your own face detector. 
/// If the output of your face detector largely differs from the OpenCV's, you can add a constant offset
/// to the output of your detector using an optional parameter in the constructor of FaceAlignment.
/// See more details in "FaceAlignment.h".
///
/// Dependencies: None. OpenCV DLLs and include folders are copied.
///
/// Author: Xuehan Xiong, xiong828@gmail.com
///
/// Creation Date: 10/15/2013
///
/// Version: 1.0
///
/// Citation: 	
/// Xuehan Xiong, Fernando de la Torre, Supervised Descent Method and Its Application to Face Alignment. CVPR, 2013
///
///
/// Modified By: Pujan Dave
/// 		 BS, Computer Engineering, University of Illinois at Urbana-Champaign
/// 		 Work done as part of SERIUS Program, National University of Singapore
///
/// Date: May 28, 2015
///
/// Description: Added support for Facial feature detection and cropping through C++ using openCV
///		 No longer dependent on Python modules. This made the optimized the run time by ~5x
///
///////////////////////////////////////////////////////////////////////////////////////////////////////



#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <FaceAlignment.h>
#include <XXDescriptor.h>
//#include <Python.h>  

#include <fstream>
#include <ctime>

using namespace cv;
using namespace std;

# define OFFSET_PICT 0.2
# define DST_SIZE 100
# define RAD_TO_DEG 57.2957795


// Function used for Rotating the image around the specified center by the specified angle in Clockwise direction!
Mat scaleRotateTranslate(Mat img, double angle, int center_x, int center_y){

	string showWindow = "Rotated";
	namedWindow( showWindow, CV_WINDOW_AUTOSIZE );

   	// Load Image!
   	Mat imgOrig = img;
	
	// Rotate!
	Mat matRotation = getRotationMatrix2D(Point(center_x, center_y), angle*RAD_TO_DEG, 1);
	// Rotate by Affine Transformation!
	Mat imgRot;
	warpAffine(imgOrig, imgRot, matRotation, imgOrig.size());

	return imgRot;
}


int crop(string src, int eye_lx, int eye_ly, int eye_rx, int eye_ry, string dst){

	int offset_h, offset_v;
	int eye_directionX, eye_directionY;
	double rotation, dist, reference, scale, crop_x, crop_y, crop_w, crop_h;
	Mat resized;
	Mat img = imread(src, CV_LOAD_IMAGE_UNCHANGED);	// Read the image from the src file

	// Error Check!
	if(img.empty()){
		cout<<"Couldn't load the image: "<<src<<" !!!"<<endl;
		return -1;
	}

	// Offsets for original image
	offset_h = floor((double)OFFSET_PICT*DST_SIZE);
	offset_v = floor((double)OFFSET_PICT*DST_SIZE);

	// Get the direction
	eye_directionX = eye_rx - eye_lx;
	eye_directionY = eye_ry - eye_ly;

	// Rotation angles in radians
	rotation = (atan2((double)eye_directionY,(double)eye_directionX));

	// Distance between 2 eyes
	dist = sqrt((eye_rx - eye_lx)*(eye_rx - eye_lx) + (eye_ry - eye_ly)*(eye_ry - eye_ly));

	// reference eye-width
	reference = DST_SIZE - (2.0 * offset_h);
	// Scale factor
	scale = dist/reference;

	// Rotate the original image around the left eye
	img = scaleRotateTranslate(img, rotation, eye_lx, eye_ly);

	// crop the rotated image!
	crop_x = eye_lx - scale*offset_h;
	crop_y = eye_ly - scale*offset_v;
	crop_w = DST_SIZE*scale;
	crop_h = DST_SIZE*scale;

	img = img(Rect(crop_x, crop_y, crop_w, crop_h));

	resize(img, resized, Size(DST_SIZE, DST_SIZE), 0, 0, INTER_CUBIC);

	imwrite(dst, resized);

	return 0;
}

using namespace std;
int facex[49];
int facey[49];
// 2 modes: AUTO, INTERACTIVE
#define AUTO

bool drawing_box = false;
cv::Mat X;
cv::Rect box;
INTRAFACE::FaceAlignment *fa;
int m,eyeleftx,eyelefty,eyerightx,eyerighty;
void draw_box(cv::Mat* img, cv::Rect rect ){
	cv::rectangle( *img, rect, cv::Scalar(0,255,0));
}

// Implement mouse callback

void my_mouse_callback( int event, int x, int y, int flags, void* param ){
	cv::Mat* image = (cv::Mat*) param;

	switch( event ){
		case CV_EVENT_MOUSEMOVE: 
			if( drawing_box ){
				box.width = x-box.x;
				box.height = y-box.y;
			}
			break;

		case CV_EVENT_LBUTTONDOWN:
			drawing_box = true;
			box = cv::Rect( x, y, 0, 0 );
			break;
		// when right button is release, face alignment is performed on the created rectangle.
		case CV_EVENT_LBUTTONUP:
			drawing_box = false;
			if( box.width < 0 ){
				box.x += box.width;
				box.width *= -1;
			}
			if( box.height < 0 ){
				box.y += box.height;
				box.height *= -1;
			}
			draw_box( image, box );
			float score;
			// detect facial landmarks
			fa->Detect(*image,box,X,score);
			// draw prediction
			for (int i = 0 ; i < X.cols ; i++)
				cv::circle(*image,cv::Point((int)X.at<float>(0,i), (int)X.at<float>(1,i)), 1, cv::Scalar(0,255,0), -1);

			break;
	}
}




int main(int argc, char **argv)
{
	char detectionModel[] = "./models/DetectionModel-v1.5.bin";
	char trackingModel[] = "./models/TrackingModel-v1.10.bin";
        
	INTRAFACE::XXDescriptor xxd(4);
        string a="HH";
        ofstream in;
        ifstream out;
        in.open("face.txt",ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
        int i,j;


	fa = new INTRAFACE::FaceAlignment(detectionModel, trackingModel, &xxd);

	if (!fa->Initialized()) {
		cerr << "FaceAlignment cannot be initialized." << endl;
		return -1;
	}
        char *psr=argv[1];
        cout<<psr[9]<<psr[10];
	string filename(psr);

	cv::Mat frame  = cv::imread(filename);
	string winname("Demo IntraFace Detector");
	cv::namedWindow(winname);

#ifdef INTERACTIVE
	
	// Set up the callback
	cvSetMouseCallback( winname.c_str(), my_mouse_callback, (void*) &frame);
	
	while( true ) {
		cv::Mat image = frame.clone();
		if( drawing_box ) 
			draw_box(&image, box );
		cv::imshow(winname,image);
		// press Esc to quit
		if( cv::waitKey( 30 )==27 ) 
			break;
	}

#endif

#ifdef AUTO
	string faceDetectionModel("./models/haarcascade_frontalface_alt2.xml");
	cv::CascadeClassifier face_cascade;
	if( !face_cascade.load( faceDetectionModel ) )
	{ 
		cerr << "Error loading face detection model." << endl;
		return -1; 
	}
	vector<cv::Rect> faces;
	float score, notFace = 0.5;
	face_cascade.detectMultiScale(frame, faces, 1.2, 2, 0, cv::Size(50, 50));

	for (int i = 0 ;i < faces.size(); i++) {
		if (fa->Detect(frame,faces[i],X,score) == INTRAFACE::IF_OK)
		{
                        m=0;
			if (score >= notFace) {
                                m++;
				in<<'1';
                                //std::cout<<X;
				for (int i = 0 ; i < X.cols ; i++)
				{
                                    facex[i]=(int)X.at<float>(0,i);
                                    facey[i]=(int)X.at<float>(1,i);
				}
			}


		}

	}
	in.close();//关闭文件
        eyeleftx=(int)(0.5*(facex[19]+facex[22]));
        eyerightx=(int)(0.5*(facex[25]+facex[28]));
        eyelefty=(int)(0.5*(facey[19]+facey[22]));
        eyerighty=(int)(0.5*(facey[25]+facey[28]));

	// Attempting to use the C++ code for cropping instead of a Python module
	
	// Crop only when a face was detected!!!
	if(eyeleftx || eyelefty || eyerighty)	
	    crop(psr, eyeleftx, eyelefty, eyerightx, eyerighty, argv[2]);
	
/*
      	PyObject_CallFunction(pFunHi,"siiiis",psr,eyeleftx,eyelefty,eyerightx,eyerighty,argv[2]);
	Py_DECREF(pFunHi);   
	Py_DECREF(pModule);  
	Py_Finalize();
*/

#endif

    delete fa;
    return 0;

}

