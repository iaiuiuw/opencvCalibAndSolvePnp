//#include <opencv2/opencv.hpp>
//#include <math.h>
//#include <iostream>
//#include <fstream>
//
//using namespace std;
//using namespace cv;
//
////将空间点绕Z轴旋转
////输入参数 x y为空间点原始x y坐标
////thetaz为空间点绕Z轴旋转多少度，角度制范围在-180到180
////outx outy为旋转后的结果坐标
//void codeRotateByZ(double x, double y, double thetaz, double& outx, double& outy)
//{
//	double x1 = x;//将变量拷贝一次，保证&x == &outx这种情况下也能计算正确
//	double y1 = y;
//	double rz = thetaz * CV_PI / 180;
//	outx = cos(rz) * x1 - sin(rz) * y1;
//	outy = sin(rz) * x1 + cos(rz) * y1;
//}
//
////将空间点绕Y轴旋转
////输入参数 x z为空间点原始x z坐标
////thetay为空间点绕Y轴旋转多少度，角度制范围在-180到180
////outx outz为旋转后的结果坐标
//void codeRotateByY(double x, double z, double thetay, double& outx, double& outz)
//{
//	double x1 = x;
//	double z1 = z;
//	double ry = thetay * CV_PI / 180;
//	outx = cos(ry) * x1 + sin(ry) * z1;
//	outz = cos(ry) * z1 - sin(ry) * x1;
//}
//
////将空间点绕X轴旋转
////输入参数 y z为空间点原始y z坐标
////thetax为空间点绕X轴旋转多少度，角度制，范围在-180到180
////outy outz为旋转后的结果坐标
//void codeRotateByX(double y, double z, double thetax, double& outy, double& outz)
//{
//	double y1 = y;//将变量拷贝一次，保证&y == &y这种情况下也能计算正确
//	double z1 = z;
//	double rx = thetax * CV_PI / 180;
//	outy = cos(rx) * y1 - sin(rx) * z1;
//	outz = cos(rx) * z1 + sin(rx) * y1;
//}
//
//
////点绕任意向量旋转，右手系
////输入参数old_x，old_y，old_z为旋转前空间点的坐标
////vx，vy，vz为旋转轴向量
////theta为旋转角度角度制，范围在-180到180
////返回值为旋转后坐标点
//cv::Point3f RotateByVector(double old_x, double old_y, double old_z, double vx, double vy, double vz, double theta)
//{
//	double r = theta * CV_PI / 180;
//	double c = cos(r);
//	double s = sin(r);
//	double new_x = (vx*vx*(1 - c) + c) * old_x + (vx*vy*(1 - c) - vz * s) * old_y + (vx*vz*(1 - c) + vy * s) * old_z;
//	double new_y = (vy*vx*(1 - c) + vz * s) * old_x + (vy*vy*(1 - c) + c) * old_y + (vy*vz*(1 - c) - vx * s) * old_z;
//	double new_z = (vx*vz*(1 - c) - vy * s) * old_x + (vy*vz*(1 - c) + vx * s) * old_y + (vz*vz*(1 - c) + c) * old_z;
//	return cv::Point3f(new_x, new_y, new_z);
//}
//
//int main(int argc, char **argv)
//{
//
//	// Read input image
//	cv::Mat im = cv::imread("测试用图1.jpg");
//
//	// 2D image points. If you change the image, you need to change vector
//	std::vector<cv::Point2d> image_points;
//	image_points.push_back(cv::Point2d(2654, 1354));    // 1
//	image_points.push_back(cv::Point2d(1631, 1161));    // 2
//	image_points.push_back(cv::Point2d(2587, 911));    // 3
//	image_points.push_back(cv::Point2d(3860, 1324));    // 4
//	image_points.push_back(cv::Point2d(3105, 1746));    // 5
//	image_points.push_back(cv::Point2d(1589, 1744));    // 6
//
//	// 3D model points.三维坐标的单位是毫米
//	std::vector<cv::Point3d> model_points;
//	model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));             // 1
//	model_points.push_back(cv::Point3d(118.0f, 73.0f, 0.0f));          // 2
//	model_points.push_back(cv::Point3d(12.0f, 140.0f, 0.0f));          // 3
//	model_points.push_back(cv::Point3d(141.0f, 12.5f, 0.0f));          // 4
//	model_points.push_back(cv::Point3d(32.0f, 90.0f, 0.0f));           // 5
//	model_points.push_back(cv::Point3d(123.0f, 71.5f, 0.0f));          // 6
//
//	 //初始化相机参数Opencv
//	double camD[9] = {
//		2701.557091346351, 0, 1224.110792113303,
//		0, 2674.593151143197, 672.7778980404739,
//		0, 0, 1 };
//	cv::Mat camera_matrix = cv::Mat(3, 3, CV_64FC1, camD);
//
//	//畸变参数
//	double distCoeffD[5] = { 0.04852125769290959, -8.849070319949437, 0.008930005947805285, -0.008255448934211105, 144.5356118611395 };
//	cv::Mat distortion_coefficients = cv::Mat(5, 1, CV_64FC1, distCoeffD);
//																		 // Camera internals
//	double focal_length = im.cols; // Approximate focal length.
//	Point2d center = cv::Point2d(im.cols / 2, im.rows / 2);
//	// cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
//	// cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion
//
//	cout << "Camera Matrix " << endl << camera_matrix << endl;
//	// Output rotation and translation
//	cv::Mat rotation_vector; // Rotation in axis-angle form
//	cv::Mat translation_vector;
//
//	// Solve for pose
//	cv::solvePnP(model_points, image_points, camera_matrix, distortion_coefficients, rotation_vector, translation_vector);
//
//
//	// Project a 3D point (0, 0, 1000.0) onto the image plane.
//	// We use this to draw a line sticking out of the nose
//
//	vector<Point3d> nose_end_point3D;
//	vector<Point2d> nose_end_point2D;
//	nose_end_point3D.push_back(Point3d(0, 0, 1000.0));
//
//	projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, distortion_coefficients, nose_end_point2D);
//
//
//	for (int i = 0; i < image_points.size(); i++)
//	{
//		circle(im, image_points[i], 3, Scalar(0, 0, 255), -1);
//	}
//
//	/*cv::line(im, image_points[0], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);*/
//
//	cout << "Rotation Vector " << endl << rotation_vector << endl;
//	cout << "Translation Vector" << endl << translation_vector << endl;
//
//	cout << nose_end_point2D << endl;
//
//	// Display image.
//	cv::imshow("Output", im);
//	cv::waitKey(0);
//
//	return 0;
//}