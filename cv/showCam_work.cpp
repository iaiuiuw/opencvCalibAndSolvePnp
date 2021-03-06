#include<opencv2/opencv.hpp>   
#include<cv.h>  
#include<stdio.h>
#include <highgui.h>  
#include "calib.h"
#include "getCamRT.h"
#include "cameraParameterReader.h"
//#include "getCamRT2.h"
using namespace cv;
using namespace std;


vector<Point2d> Points2D;

Mat calibrator(Mat &view, Mat &cameraMatrix, Mat &distCoeffs)
{
	vector<string> imageList;
	static bool bLoadCameraParams = false;
	static Mat map1, map2;
	Mat rview;
	Size imageSize, newImageSize;

	if (!view.data)
		return Mat();

	imageSize.width = view.cols;
	imageSize.height = view.rows;

	newImageSize.width = imageSize.width;
	newImageSize.height = imageSize.height;

	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
		getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, newImageSize, 0), newImageSize, CV_16SC2, map1, map2);


	//undistort( view, rview, cameraMatrix, distCoeffs, cameraMatrix );  
	remap(view, rview, map1, map2, INTER_LINEAR);

	//imshow("Image View", rview);  
	//int c = 0xff & waitKey();  

	rview.copyTo(view);

	return view;
}

void on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		Point2d p(x, y);
		Points2D.push_back(p);
		cout << "Point Add：" << p<<endl;
	}

	
}

//-----------------------------------【main()函数】--------------------------------------------  
//                描述：控制台应用程序的入口函数，我们的程序从这里开始  
//-------------------------------------------------------------------------------------------------  
int main()
{
	//【1】从摄像头读入视频  
	VideoCapture capture(0);//若测试摄像头有没有打开，/*if(!capture.isOpened())                {cout<< "cannot open the camera.";cin.get();return -1;}*  
	Mat frame; //定义一个Mat变量，用于存储每一帧的图像  
	Mat frameToShow;
	int state = 0;
	int usecalibrator = 0;
	int rotatecount = 3;
	int isFlip = 0;

	Mat cameraMatrix, distCoeffs;

	cout << "Press R rotateView" << endl 
		<< "Press F flip"<<endl
		<< "Press A do calibration from the folder \"caliberation\"" << endl
		<<"Press C start/cancel calibrate"<<endl
		<<"Press Space record calibrate img"<<endl
		<<"Press Enter DO calibration"<<endl
		<<"Press P start solvePnp"<<endl
		<<"Pick 4 points from the graph, the solvePNP will work automatically"<<endl
		<<"Watch out your Capslock, all input should be in lowercase letter"<<endl;

	namedWindow("CameraView", 0);
			   //【2】循环显示每一帧  
	while (1)
	{
		capture >> frame;  //读取当前帧                          
		if (frame.empty())
		{
			printf("--(!) No captured frame -- Break!");
			//break;                  
		}
		else
		{
			//resize(frame, frameToDraw, Size(960, 540), 0, 0, INTER_LINEAR);
			if (rotatecount != 3)
				rotate(frame, frame, rotatecount);
			if (isFlip)
				flip(frame, frame, 1);
			if (state == 2) {
				for (int i = 0; i < Points2D.size(); i++)
				{
					cv::circle(frame, Points2D[i], 5, cv::Scalar(255, 0, 255), -1);
				}
			}

			if (usecalibrator)
			{
				if (cameraMatrix.rows == 0) {
					double camD[9];
					double distCoeffD[5];
					readCamParameter(camD, distCoeffD);
					cameraMatrix = cv::Mat(3, 3, CV_64FC1, camD);
					distCoeffs = cv::Mat(5, 1, CV_64FC1, distCoeffD);

				}

				frameToShow = calibrator(frame, cameraMatrix, distCoeffs);
			}
			else
			{
				if (cameraMatrix.rows != 0) {
					cameraMatrix.empty();
					distCoeffs.empty();
				}
				frame.copyTo(frameToShow);
			}
			//resize(frameToShow, frameToShow, Size(frame.cols / 2, frame.rows / 2));

			imshow("CameraView", frameToShow); //显示当前帧  

			
			resizeWindow("CameraView", frame.cols / 2, frame.rows / 2);


		}

		char key = waitKey(60);//延时30ms   

		if (key == 27) {//esc
			break;

		}
		else if (key == '1') {
			usecalibrator = 1 - usecalibrator;
		}
		else if (key == 'c'&& state == 0) {//start calibration
			initCalib();
			state = 1;
		}
		else if (key == 'c'&& state == 1) {//start calibration
			state = 0;
			cout << "Stop Calib!!!" << endl;
		}
		else if (key == ' ' && state == 1) {//calibration get corners
			getCornerSeq(frame);
		}
		else if (key == '\r' && state == 1) {//calibration caculate
			calibcamera();
			state = 0;
		}
		else if (key == 'p' && state == 0) {//pnp start
			state = 2;
			cout << "Please Set Point2Ds" << endl;
			setMouseCallback("CameraView", on_mouse, 0);
			Points2D.clear();
		}
		else if (key == 'a' && state == 0) {
			calibcamerafromFiles();
		}
		else if (key == 'r') {
			rotatecount++;
			rotatecount %= 4;
		}
		else if (key == 'f') {
			isFlip = 1 - isFlip;
		}

		if (state == 2) {
			if (Points2D.size() == 4) {
				setMouseCallback("CameraView", NULL, 0);
				caculateCamRT(frame, Points2D);
				state = 0;
			}
		}

	}
	return 0;
}