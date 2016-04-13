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
#include <Python.h>  

#include <fstream>

void printDict(PyObject* obj) {  
 
    if (!PyDict_Check(obj))  
 
        return;  
 
    PyObject *k, *keys;  
 
    keys = PyDict_Keys(obj);  
 
    for (int i = 0; i < PyList_GET_SIZE(keys); i++) {  
 
        k = PyList_GET_ITEM(keys, i);  
 
        char* c_name = PyString_AsString(k);  
 
        printf("%s\n", c_name);  
 
    }  
 
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
        char *a="HH";
        ofstream in;
        ifstream out;
        //in.open("com.txt",ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
        //in.close();//关闭文件
        int i,j;


	fa = new INTRAFACE::FaceAlignment(detectionModel, trackingModel, &xxd);

	if (!fa->Initialized()) {
		cerr << "FaceAlignment cannot be initialized." << endl;
		return -1;
	}
	// read image
        char *psr=argv[1];
        cout<<psr[9]<<psr[10];
        //"test4.jpg"
	string filename(psr);
	//string filename("./data/Screenshot_1.png");
	//string filename("./data/205_2_Freeform_video.mp4");
	cv::Mat frame  = cv::imread(filename);
	// create a window
	string winname("Demo IntraFace Detector");
	cv::namedWindow(winname);
        //Python Initialization
	Py_Initialize();  
	if (!Py_IsInitialized())  

	return -1;  

	PyRun_SimpleString("import sys");  

	PyRun_SimpleString("sys.path.append('./')");  

	//导入模块  

	PyObject* pModule = PyImport_ImportModule("Preprocess");  

	if (!pModule) {  

	printf("Cant open python file!\n");  

	return -1;  

	}  

	//模块的字典列表  

	PyObject* pDict = PyModule_GetDict(pModule);  

	if (!pDict) {  

	printf("Cant find dictionary.\n");  

	return -1;  

	}  

	//打印出来看一下  

	//printDict(pDict);  
	//演示函数调用  
	PyObject* pFunHi = PyDict_GetItemString(pDict, "action"); 

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
	// load OpenCV face detector model
	string faceDetectionModel("./models/haarcascade_frontalface_alt2.xml");
	cv::CascadeClassifier face_cascade;
	if( !face_cascade.load( faceDetectionModel ) )
	{ 
		cerr << "Error loading face detection model." << endl;
		return -1; 
	}
	vector<cv::Rect> faces;
	float score, notFace = 0.5;
	// face detection
	face_cascade.detectMultiScale(frame, faces, 1.2, 2, 0, cv::Size(50, 50));
	

	for (int i = 0 ;i < faces.size(); i++) {
        //for (int i = 0 ;i < 2; i++) {
		// face alignment
		if (fa->Detect(frame,faces[i],X,score) == INTRAFACE::IF_OK)
		{
			// only draw valid faces
                        m=0;
			if (score >= notFace) {
                                m++;
                                //std::cout<<X;
				for (int i = 0 ; i < X.cols ; i++)
				{
				    //cv::circle(frame,cv::Point((int)X.at<float>(0,i), (int)X.at<float>(1,i)), 1, cv::Scalar(0,255,0), -1);
 				    //std::cout<<i<<"\t"<<(int)X.at<float>(0,i)<<"\t"<<(int)X.at<float>(1,i)<<"\n";
                                    facex[i]=(int)X.at<float>(0,i);
                                    facey[i]=(int)X.at<float>(1,i);
				}
			}


		}

	}
        /*in.open("label.txt",ios::app);
        if(psr[3]=='S'&&psr[4]=='A')
        {
           in<<1<<"\n";
        }
        if(psr[3]=='F'&&psr[4]=='E')
        {
           in<<2<<"\n";
        }
        if(psr[3]=='A'&&psr[4]=='N')
        {
           in<<3<<"\n";
        }
        if(psr[3]=='H'&&psr[4]=='A')
        {
           in<<4<<"\n";
        }
        if(psr[3]=='D'&&psr[4]=='I')
        {
           in<<5<<"\n";
        }
        if(psr[3]=='S'&&psr[4]=='U')
        {
           in<<6<<"\n";
        }
        in.close();
        in.open("com.txt",ios::app);
        for(int i=0;i<49;i++)
        {
        	std::cout<<facex[i]<<"\t"<<facey[i]<<"\n";
                in<<facex[i]<<"\t"<<facey[i]<<"\t";
	}
        in<<"\n";
        in.close();*/

        eyeleftx=(int)(0.5*(facex[19]+facex[22]));
        eyerightx=(int)(0.5*(facex[25]+facex[28]));
        eyelefty=(int)(0.5*(facey[19]+facey[22]));
        eyerighty=(int)(0.5*(facey[25]+facey[28])); 
	PyObject_CallFunction(pFunHi,"siiiis",psr,eyeleftx,eyelefty,eyerightx,eyerighty,argv[2]);
	Py_DECREF(pFunHi);   
	Py_DECREF(pModule);  
	Py_Finalize(); 

	//cv::imshow(winname,frame);
	//cv::waitKey(0); // press any key to quit
#endif

    delete fa;
    return 0;

}

