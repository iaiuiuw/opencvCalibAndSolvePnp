#include<opencv2/opencv.hpp>   
#include<cv.h>  
#include<stdio.h>
#include <highgui.h>  

using namespace cv;
using namespace std;
//-----------------------------------��main()������--------------------------------------------  
//                ����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ  
//-------------------------------------------------------------------------------------------------  
int notmain()
{
	//��1��������ͷ������Ƶ  
	VideoCapture capture;//����������ͷ��û�д򿪣�/*if(!capture.isOpened())                {cout<< "cannot open the camera.";cin.get();return -1;}*  
	capture.open(0);

	Mat frame; //����һ��Mat���������ڴ洢ÿһ֡��ͼ��  
			   //��2��ѭ����ʾÿһ֡  
	while (1)
	{
		capture >> frame;  //��ȡ��ǰ֡                          
		if (frame.empty())
		{
			printf("--(!) No captured frame -- Break!");
			//break;                  
		}
		else
		{
			//resize(frame, frameToDraw, Size(960, 540), 0, 0, INTER_LINEAR);
			namedWindow("��Ƶ", 0);
			resizeWindow("��Ƶ", 960, 540);
			imshow("��Ƶ", frame); //��ʾ��ǰ֡  
		}

		if (waitKey(30) == 27)//esc
			break;//��ʱ30ms   
	}
	return 0;
}