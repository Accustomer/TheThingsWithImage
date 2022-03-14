/*
通过块访问数组元素

Author: Alex
*/

#pragma once
#include <iostream>
#include <opencv2/highgui.hpp>

void visitTest()
{
	cv::Mat m(5, 5, CV_8UC1);
	cv::RNG rng;
	rng.fill(m, rng.UNIFORM, 0, 256, true);
	std::cout << "m:\n" << m << std::endl;
	std::cout << "\nm.row(1): " << m.row(1) << std::endl;
	std::cout << "\nm.col(1): " << m.col(1) << std::endl;
	std::cout << "\nm.rowRange(1, 3):\n" << m.rowRange(1, 3) << std::endl;
	std::cout << "\nm.colRange(1, 3):\n" << m.colRange(1, 3) << std::endl;
	std::cout << "\nm.rowRange(cv::Range(2, 4)):\n" << m.rowRange(cv::Range(2, 4)) << std::endl;
	std::cout << "\nm.colRange(cv::Range(2, 4)):\n" << m.colRange(cv::Range(2, 4)) << std::endl;
	std::cout << "\nm(cv::Range(2, 4), cv::Range(1, 3)):\n" << m(cv::Range(2, 4), cv::Range(1, 3)) << std::endl;
	std::cout << "\nm.diag(0): " << m.diag(0) << std::endl;
	std::cout << "\nm.diag(-1): " << m.diag(-1) << std::endl;
	std::cout << "\nm.diag(2): " << m.diag(2) << std::endl;
	std::cout << "\nm(cv::Rect(1, 1, 3, 2)):\n" << m(cv::Rect(1, 1, 3, 2)) << std::endl;

	uchar& Vxy = m.at<uchar>(3, 3);
	Vxy = cv::saturate_cast<uchar>((Vxy - 128) * 3 + 128);
	std::cout << "Vxy = " << (int)Vxy << std::endl;
	std::cout << "m = " << m << std::endl;
}
