#include<opencv2/opencv.hpp>   
#include<cv.h>  
#include<stdio.h>
#include <highgui.h>  

using namespace cv;
using namespace std;
//-----------------------------------【main()函数】--------------------------------------------  
//                描述：控制台应用程序的入口函数，我们的程序从这里开始  
//-------------------------------------------------------------------------------------------------  
int notmain()
{
	//【1】从摄像头读入视频  
	VideoCapture capture;//若测试摄像头有没有打开，/*if(!capture.isOpened())                {cout<< "cannot open the camera.";cin.get();return -1;}*  
	capture.open(0);

	Mat frame; //定义一个Mat变量，用于存储每一帧的图像  
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
			namedWindow("视频", 0);
			resizeWindow("视频", 960, 540);
			imshow("视频", frame); //显示当前帧  
		}

		if (waitKey(30) == 27)//esc
			break;//延时30ms   
	}
	return 0;
}