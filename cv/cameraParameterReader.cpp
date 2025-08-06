#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

void split(const char *s, vector<string> &strs, char delim = ' ') {
	if (s == nullptr) {
		return;
	}

	const char *head, *tail;
	head = tail = s;

	while (*head != '\0') {
		while (*head != '\0' && *head == delim) {
			head++;
		}

		tail = head;
		while (*tail != '\0' && *tail != delim) {
			tail++;
		}

		if (head != tail) {
			strs.push_back(string(head, tail));
			head = tail;
		}
		else {
			break;
		}
	}
}

void readCamParameter(double *cameraMatrixArray, double *distCoeffsArray) {
	//�����ڲ�
	ifstream camD_fin("calib_output.txt");

	string camD_str;
	string distCoeffD_str;
	if (!getline(camD_fin, camD_str)) {
		cout << "camera parameters required!!!" << endl;
		camD_fin.close();
		return ;
	}
	if (!getline(camD_fin, distCoeffD_str)) {
		cout << "camera parameters required!!!" << endl;
		camD_fin.close();
		return;
	}
	camD_fin.close();
	vector<string> camD_strs;
	vector<string> distCoeffD_strs;
	split(&camD_str[0], camD_strs, ',');
	split(&distCoeffD_str[0], distCoeffD_strs, ',');


	//double camD[9];

	for (int i = 0; i < 9; i++) {
		if (i < camD_strs.size()) {
			sscanf_s(camD_strs[i].c_str(), "%lf", &cameraMatrixArray[i]);
		} else {
			cameraMatrixArray[i] = 0.0; // Default value for missing parameters
		}
	}

	//*cameraMatrix = cv::Mat(3, 3, CV_64FC1, camD);

	//�������
	//double distCoeffD[5];// = { 0.265902785145087, -9.349493851523201, -0.007005920612312271, -0.009140975503677254, 76.79043372421278 };
	for (int i = 0; i < 5; i++) {
		if (i < distCoeffD_strs.size()) {
			sscanf_s(distCoeffD_strs[i].c_str(), "%lf", &distCoeffsArray[i]);
		} else {
			distCoeffsArray[i] = 0.0; // Default value for missing parameters
		}
	}

	//*distCoeffs = cv::Mat(5, 1, CV_64FC1, distCoeffD);

}