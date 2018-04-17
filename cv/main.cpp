#include "getCamRT.h"
#include <iostream>  
using namespace std;

void main22() {

	cv::Mat im = cv::imread("test.jpg");
	vector<cv::Point2d> p2ds;
	p2ds.push_back(cv::Point2d(316, 464));
	p2ds.push_back(cv::Point2d(1343, 492));
	p2ds.push_back(cv::Point2d(1422, 717));
	p2ds.push_back(cv::Point2d(248, 694));
	caculateCamRT(im, p2ds);
	cvWaitKey(0);

}