#include <opencv2\opencv.hpp>
using namespace cv;
#pragma once
void readCamParameter(double *cameraMatrixArray, double *distCoeffsArray);
void split(const char *s, vector<string> &strs, char delim = ' ');