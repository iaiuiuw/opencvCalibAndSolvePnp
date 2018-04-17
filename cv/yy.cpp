//#include <opencv2/opencv.hpp>
//#include <vector>
//#include <iostream>
//
//#include "Eigen/Core"
//#include "Eigen/Geometry"
//
//int main() {
//	int num = 4;
//	cv::Mat K(cv::Matx33d(2208.34748, 0, 953.39087, 0, 2203.08854, 509.59149, 0, 0, 1));
//	cv::Mat dist(cv::Matx14d(-0.02978, 0.21003, -0.00474, -0.00166));
//	std::vector<cv::Point3d> P(num);
//	std::vector<cv::Point2d> p(num);
//
//	P[0].x = 12.3; P[0].y = -7.15; P[0].z = 0;
//	P[1].x = 1.25; P[1].y = 14.1; P[1].z = 0;
//	P[2].x = -14; P[2].y = 1.2; P[2].z = 0;
//	P[3].x = -7.3; P[3].y = -11.8; P[3].z = 0;
//
//	p[0].x = 560; p[0].y = 538;
//	p[1].x = 1234; p[1].y = 543;
//	p[2].x = 1022; p[2].y = 358;
//	p[3].x = 708; p[3].y = 391;
//
//	cv::Mat R_w2c, r_w2c, t_w2c;
//	cv::solvePnP(P, p, K, dist, r_w2c, t_w2c, false, cv::SOLVEPNP_P3P);
//	cv::Rodrigues(r_w2c, R_w2c);
//	cv::Mat R_c2w, t_c2w;
//	R_c2w = R_w2c.t();
//	t_c2w = -R_c2w * t_w2c;
//
//	//convert the 3x3 rotation matrix to three rotation angles around x, y, z axis
//	Eigen::Matrix3d eigenR;
//	for (int i = 0; i < 3; ++i) {
//		for (int j = 0; j < 3; ++j) {
//			eigenR(i, j) = R_c2w.at<double>(i, j);
//		}
//	}
//	Eigen::Vector3d eulerAngle1 = eigenR.eulerAngles(0, 1, 2);
//	Eigen::Vector3d eulerAngle2 = eigenR.eulerAngles(2, 1, 0);
//	std::cout << "rotation around x,y,z:" << std::endl;
//	std::cout << eulerAngle1 * 180 / 3.1415926 << std::endl;
//	std::cout << "rotation around z,y,x:" << std::endl;
//	std::cout << eulerAngle2 * 180 / 3.1415926 << std::endl;
//	std::cout << "translation:" << std::endl;
//	std::cout << t_c2w << std::endl;
//
//	return 0;
//}