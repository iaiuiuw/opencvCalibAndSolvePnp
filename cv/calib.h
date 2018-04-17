#include "opencv2/core/core.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/calib3d/calib3d.hpp"  
#include "opencv2/highgui/highgui.hpp"  

using namespace cv;

#pragma once

void initCalib();
bool getCornerSeq(Mat imageInput);
void calibcamerafromFiles();
void calibcamera();