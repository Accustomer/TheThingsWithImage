/*
����һЩMat�Ļ�������

Author: Alex
*/

#pragma once
#include <iostream>
#include <opencv2/highgui.hpp>


void testAttributes()
{
	cv::Mat m0(5, 5, CV_8UC3);
	cv::RNG rng;
	rng.fill(m0, rng.UNIFORM, 0, 256, true);
	std::cout << "m0: \n" << cv::format(m0, cv::Formatter::FMT_NUMPY) << std::endl;

	cv::Mat m1;
	m0.assignTo(m1, CV_32FC1);
	std::cout << "m1: \n" << cv::format(m1, cv::Formatter::FMT_NUMPY) << std::endl;

	cv::Mat m2 = m0.rowRange(2, 4).colRange(2, 4).clone();

	// adjustROIֻ��ֱ�Ӹ�.row(), .col(), .rowRange(), .colRange()֮������á��ڸ�������б߽����� 
	cv::Mat m3 = m0.rowRange(2, 4).colRange(2, 4).adjustROI(1, 1, 1, 1);
	std::cout << "m2: \n" << cv::format(m2, cv::Formatter::FMT_NUMPY) << std::endl;
	std::cout << "m3: \n" << cv::format(m3, cv::Formatter::FMT_NUMPY) << std::endl;

	// locateROI��λ������ĳߴ���Ӿ����ڸ������е����Ͻ�λ��
	cv::Size sz;
	cv::Point offset;
	m0.colRange(1, 3).rowRange(2, 3).locateROI(sz, offset);
	std::cout << "sz: \n" << sz << std::endl;
	std::cout << "offset: \n" << offset << std::endl;

	std::cout << "m0.total(): \n" << m0.total() << std::endl;
	std::cout << "m0.type(): \n" << m0.type() << std::endl;
	std::cout << "m0.depth(): \n" << m0.depth() << std::endl;
}
