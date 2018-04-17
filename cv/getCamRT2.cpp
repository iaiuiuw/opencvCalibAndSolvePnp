﻿// SlovePNPByOpenCV.cpp : 定义控制台应用程序的入口点。
//

/*
例程基于VS2013+OpenCV2.4.X开发，大家运行前需要将opencv的路径重新配置成自己电脑上的，
不懂的话参考我的博客《OpenCV2+入门系列（一）：OpenCV2.4.9的安装与测试》。例程中提供两张照片，
其中DSC03323就是“实验”中所用图片，例程在计算完成后，会在D盘根目录下生成两个txt，分别存储：
相机在世界坐标系的坐标、相机的三个旋转角。
@Author：VShawn
@URL：http://www.cnblogs.com/singlex/
*/


#include <opencv2\opencv.hpp>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

//将空间点绕Z轴旋转
//输入参数 x y为空间点原始x y坐标
//thetaz为空间点绕Z轴旋转多少度，角度制范围在-180到180
//outx outy为旋转后的结果坐标
void codeRotateByZ(double x, double y, double thetaz, double& outx, double& outy)
{
	double x1 = x;//将变量拷贝一次，保证&x == &outx这种情况下也能计算正确
	double y1 = y;
	double rz = thetaz * CV_PI / 180;
	outx = cos(rz) * x1 - sin(rz) * y1;
	outy = sin(rz) * x1 + cos(rz) * y1;
}

//将空间点绕Y轴旋转
//输入参数 x z为空间点原始x z坐标
//thetay为空间点绕Y轴旋转多少度，角度制范围在-180到180
//outx outz为旋转后的结果坐标
void codeRotateByY(double x, double z, double thetay, double& outx, double& outz)
{
	double x1 = x;
	double z1 = z;
	double ry = thetay * CV_PI / 180;
	outx = cos(ry) * x1 + sin(ry) * z1;
	outz = cos(ry) * z1 - sin(ry) * x1;
}

//将空间点绕X轴旋转
//输入参数 y z为空间点原始y z坐标
//thetax为空间点绕X轴旋转多少度，角度制，范围在-180到180
//outy outz为旋转后的结果坐标
void codeRotateByX(double y, double z, double thetax, double& outy, double& outz)
{
	double y1 = y;//将变量拷贝一次，保证&y == &y这种情况下也能计算正确
	double z1 = z;
	double rx = thetax * CV_PI / 180;
	outy = cos(rx) * y1 - sin(rx) * z1;
	outz = cos(rx) * z1 + sin(rx) * y1;
}


//点绕任意向量旋转，右手系
//输入参数old_x，old_y，old_z为旋转前空间点的坐标
//vx，vy，vz为旋转轴向量
//theta为旋转角度角度制，范围在-180到180
//返回值为旋转后坐标点
cv::Point3f RotateByVector(double old_x, double old_y, double old_z, double vx, double vy, double vz, double theta)
{
	double r = theta * CV_PI / 180;
	double c = cos(r);
	double s = sin(r);
	double new_x = (vx*vx*(1 - c) + c) * old_x + (vx*vy*(1 - c) - vz * s) * old_y + (vx*vz*(1 - c) + vy * s) * old_z;
	double new_y = (vy*vx*(1 - c) + vz * s) * old_x + (vy*vy*(1 - c) + c) * old_y + (vy*vz*(1 - c) - vx * s) * old_z;
	double new_z = (vx*vz*(1 - c) - vy * s) * old_x + (vy*vz*(1 - c) + vx * s) * old_y + (vz*vz*(1 - c) + c) * old_z;
	return cv::Point3f(new_x, new_y, new_z);
}
void test();

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

int caculateCamRT()
{
	cv::Mat im = cv::imread("test.jpg");
	vector<cv::Point2f> Points2D;
	/****************a6000参数**********************/
	//初始化相机参数Opencv
	//double camD[9] = {
	//	6800.7, 0, 3065.8,
	//	0, 6798.1, 1667.6,
	//	0, 0, 1 };
	//cv::Mat camera_matrix = cv::Mat(3, 3, CV_64FC1, camD);

	////畸变参数
	//double distCoeffD[5] = { -0.189314, 0.444657, -0.00116176, 0.00164877, -2.57547 };
	//cv::Mat distortion_coefficients = cv::Mat(5, 1, CV_64FC1, distCoeffD);

	/****************test3参数**********************/
	//初始化相机参数Opencv

	//读入内参
	ifstream camD_fin("calib_output.txt");
	string camD_str;
	string distCoeffD_str;
	getline(camD_fin, camD_str);
	getline(camD_fin, distCoeffD_str);
	camD_fin.close();
	vector<string> camD_strs;
	vector<string> distCoeffD_strs;
	split(&camD_str[0], camD_strs, ',');
	split(&distCoeffD_str[0], distCoeffD_strs,',');


	double camD[9];

	for (int i = 0; i < 9; i++) {
		sscanf_s(&camD_strs[i][0], "%lf", &camD[i]);
	}

	cv::Mat camera_matrix = cv::Mat(3, 3, CV_64FC1, camD);

	//畸变参数
	double distCoeffD[5];// = { 0.265902785145087, -9.349493851523201, -0.007005920612312271, -0.009140975503677254, 76.79043372421278 };
	for (int i = 0; i < 5; i++) {
		sscanf_s(&distCoeffD_strs[i][0], "%lf", &distCoeffD[i]);
	}
	
	cv::Mat distortion_coefficients = cv::Mat(5, 1, CV_64FC1, distCoeffD);


	//读入校对点
	ifstream points_fin("points.txt");
	for (int i = 0; i < 4; i++) {
		string p2d_str;
		vector<string> p2d_strs;
		getline(points_fin, p2d_str);
		split(&p2d_str[0], p2d_strs, ',');
		if (p2d_strs.size() == 2) {
			double x2d, y2d;
			sscanf_s(&p2d_strs[0][0], "%lf", &x2d);
			sscanf_s(&p2d_strs[1][0], "%lf", &y2d);
			Points2D.push_back(cv::Point2d(x2d, y2d));
		}
	}
	//Points2D.push_back(cv::Point2d(316, 464));    // 1
	//Points2D.push_back(cv::Point2d(1343, 492));
	//Points2D.push_back(cv::Point2d(1422, 717));    // 6
	//Points2D.push_back(cv::Point2d(248, 694));    // 5



	//特征点世界坐标

	vector<cv::Point3f> Points3D;
	string p3d_str;

	for (int i = 0; i < 4; i++) {
		vector<string> p3d_strs;
		getline(points_fin, p3d_str);
		split(&p3d_str[0], p3d_strs, ',');
		if (p3d_strs.size() == 3) {
			double x3d, y3d, z3d;
			sscanf_s(&p3d_strs[0][0], "%lf", &x3d);
			sscanf_s(&p3d_strs[1][0], "%lf", &y3d);
			sscanf_s(&p3d_strs[2][0], "%lf", &z3d);
			Points3D.push_back(cv::Point3d(x3d, y3d, z3d));
		}
	}

	//Points3D.push_back(cv::Point3d(-150, 0, 0));             // 1
	//Points3D.push_back(cv::Point3d(150,0, 0));          // 4
	//Points3D.push_back(cv::Point3d(150, 0, -100));
	//Points3D.push_back(cv::Point3d(-150, 0, -100));           // 5


//初始化输出矩阵
	cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64FC1);
	cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64FC1);

	//三种方法求解
	//solvePnP(Points3D, Points2D, camera_matrix, distortion_coefficients, rvec, tvec, false, CV_ITERATIVE);	//实测迭代法似乎只能用4个共面特征点求解，5个点或非共面4点解不出正确的解
	solvePnP(Points3D, Points2D, camera_matrix, distortion_coefficients, rvec, tvec, false,CV_P3P);			//Gao的方法可以使用任意四个特征点，特征点数量不能少于4也不能多于4
																												//solvePnP(Points3D, Points2D, camera_matrix, distortion_coefficients, rvec, tvec, false, CV_EPNP);			//该方法可以用于N点位姿估计

																												//旋转向量变旋转矩阵
																												//提取旋转矩阵
	double rm[9];
	cv::Mat rotM(3, 3, CV_64FC1, rm);
	Rodrigues(rvec, rotM);
	double r11 = rotM.ptr<double>(0)[0];
	double r12 = rotM.ptr<double>(0)[1];
	double r13 = rotM.ptr<double>(0)[2];
	double r21 = -rotM.ptr<double>(1)[0];
	double r22 = rotM.ptr<double>(1)[1];
	double r23 = rotM.ptr<double>(1)[2];
	double r31 = rotM.ptr<double>(2)[0];
	double r32 = rotM.ptr<double>(2)[1];
	double r33 = rotM.ptr<double>(2)[2];

	/*************************************此处计算出相机的旋转角**********************************************/
	//计算出相机坐标系的三轴旋转欧拉角，旋转后可以转出世界坐标系。
	//旋转顺序为z、y、x
	//原理见帖子：
	double thetaz = atan2(r21, r11) / CV_PI * 180;
	double thetay = atan2(-1 * r31, sqrt(r32*r32 + r33 * r33)) / CV_PI * 180;
	double thetax = atan2(r32, r33) / CV_PI * 180;

	ofstream fout("./pnp_theta旋转.txt");
	fout << -1 * thetax << endl << -1 * thetay << endl << -1 * thetaz << endl;
	cout << "相机的三轴旋转角：" << -1 * thetax << ", " << -1 * thetay << ", " << -1 * thetaz << endl;
	fout.close();
	/*************************************此处计算出相机的旋转角END**********************************************/




	/*************************************此处计算出相机坐标系原点Oc在世界坐标系中的位置**********************************************/
	/* 当原始坐标系经过旋转z、y、x三次旋转后，会与世界坐标系完全平行，而三次旋转中向量OcOw会跟着旋转 */
	/* 而我们想知道的是两个坐标系完全平行时，OcOw的值 */
	/* 因此，原始坐标系每次旋转完成后，对向量OcOw进行一次反相旋转，最终可以得到两个坐标系完全平行时的OcOw */
	/* 该向量乘以-1就是世界坐标系下相机的坐标 */
	/***********************************************************************************/

	//提出平移矩阵，表示从相机坐标系原点，跟着向量(x,y,z)走，就到了世界坐标系原点
	double tx = tvec.ptr<double>(0)[0];
	double ty = tvec.ptr<double>(0)[1];
	double tz = tvec.ptr<double>(0)[2];

	//x y z 为唯一向量在相机原始坐标系下的向量值
	//也就是向量OcOw在相机坐标系下的值
	double x = tx, y = ty, z = tz;

	//进行三次反向旋转
	codeRotateByZ(x, y, -1 * thetaz, x, y);
	codeRotateByY(x, z, -1 * thetay, x, z);
	codeRotateByX(y, z, -1 * thetax, y, z);


	//获得相机在世界坐标系下的位置坐标
	//即向量OcOw在世界坐标系下的值
	double Cx = x * -1;
	double Cy = y * -1;
	double Cz = z * -1;

	

	ofstream fout2("./pnp_t位移.txt");
	fout2 << Cx << endl << Cy << endl << Cz << endl;
	cout << "相机的世界坐标：" << Cx << ", " << Cy << ", " << Cz << endl;
	fout2.close();
	/*************************************此处计算出相机坐标系原点Oc在世界坐标系中的位置END**********************************************/






	while(getline(points_fin, p3d_str)) {
		vector<string> p3d_strs;
		split(&p3d_str[0], p3d_strs, ',');
		if (p3d_strs.size() == 3) {
			double x3d, y3d, z3d;
			sscanf_s(&p3d_strs[0][0], "%lf", &x3d);
			sscanf_s(&p3d_strs[1][0], "%lf", &y3d);
			sscanf_s(&p3d_strs[2][0], "%lf", &z3d);
			Points3D.push_back(cv::Point3d(x3d, y3d, z3d));
		}
	}

	/*Points3D.push_back(cv::Point3d(0, 0, 0));
	Points3D.push_back(cv::Point3d(-150, 0, 100));
	Points3D.push_back(cv::Point3d(0, 0, 100));
	Points3D.push_back(cv::Point3d(150, 0, 100));
	Points3D.push_back(cv::Point3d(0, 0, -100));
	Points3D.push_back(cv::Point3d(-55, 0, 11));
	Points3D.push_back(cv::Point3d(-106, 0, -62));*/

	points_fin.close();

	//重投影测试位姿解是否正确
	vector<cv::Point2f> projectedPoints;
	// Points3D.push_back(cv::Point3f(150, 0, 0));
	cv::projectPoints(Points3D, rvec, tvec, camera_matrix, distortion_coefficients, projectedPoints);

	for (int i = 0; i < projectedPoints.size(); i++)
	{
		cv::circle(im, projectedPoints[i], 10, cv::Scalar(0, 255, 255), -1);
	}



	// 缩放
	double scale = 1;
	cv::Size dsize = cv::Size(im.cols*scale, im.rows*scale);
	cv::Mat image2 = cv::Mat(dsize, CV_32S);
	cv::resize(im, image2, dsize);

	for (int i = 0; i < Points2D.size(); i++)
	{
		cv::circle(im, Points2D[i] * scale, 3, cv::Scalar(0, 0, 255), -1);
	}

	cv::imshow("Out", im);
	cv::waitKey(0);
	return 0;
}


//测试用，不必理会
void test()
{
	//double x = 1, y = 2, z = 3;
	////codeRotateByZ(x, y, -90, x, y);
	////codeRotateByY(x, z, -90, x, z);
	////codeRotateByX(y, z, 90, y, z);
	////cout << endl << "   (1,2,3) -> (" << x << ',' << y << ',' << z << ")" << endl << endl;

	//if (1 == 1)
	//{
	//	double x1 = 0, y1 = 1, z1 = 2;

	//	vector<cv::Point3f> rotateAxis;//旋转的轴顺序
	//	rotateAxis.push_back(cv::Point3f(1, 0, 0));//先x轴
	//	rotateAxis.push_back(cv::Point3f(0, 1, 0));//先y轴
	//	rotateAxis.push_back(cv::Point3f(0, 0, 1));//再z轴

	//	vector<double> theta;//旋转的角度顺序
	//	theta.push_back(90);
	//	theta.push_back(-90);
	//	theta.push_back(-180);
	//	cv::Point3f p1 = RotateByVector(x1, y1, z1, rotateAxis[0].x, rotateAxis[0].y, rotateAxis[0].z, theta[0]);
	//	rotateAxis[1] = RotateByVector(rotateAxis[1].x, rotateAxis[1].y, rotateAxis[1].z, rotateAxis[0].x, rotateAxis[0].y, rotateAxis[0].z, theta[0]);
	//	rotateAxis[2] = RotateByVector(rotateAxis[2].x, rotateAxis[2].y, rotateAxis[2].z, rotateAxis[0].x, rotateAxis[0].y, rotateAxis[0].z, theta[0]);
	//	cv::Point3f p2 = RotateByVector(p1.x, p1.y, p1.z, rotateAxis[1].x, rotateAxis[1].y, rotateAxis[1].z, theta[1]);
	//	rotateAxis[2] = RotateByVector(rotateAxis[2].x, rotateAxis[2].y, rotateAxis[2].z, rotateAxis[1].x, rotateAxis[1].y, rotateAxis[1].z, theta[1]);
	//	cv::Point3f p3 = RotateByVector(p2.x, p2.y, p2.z, rotateAxis[2].x, rotateAxis[2].y, rotateAxis[2].z, theta[2]);
	//}

	//if (1 == 1)
	//{
	//	double x1 = 0, y1 = 1, z1 = 2;

	//	codeRotateByZ(x1, y1, -180, x1, y1);
	//	codeRotateByY(x1, z1, -90, x1, z1);
	//	codeRotateByX(y1, z1, 90, y1, z1);
	//	cout << x1;
	//}

	////cv::Point3f np = RotateByVector(x1, y1, z1, 1, 0, 0, 90);
	//////codeRotateByX(y1, z1, -90, y1, z1);
	//////codeRotateByZ(x1, y1, -90, x1, y1);
	//////codeRotateByY(x1, z1, -90, x1, z1);
	////codeRotateByX(y1, z1, 90, y1, z1);
	//////codeRotateByZ(x1, y1, -90, x1, y1);
}