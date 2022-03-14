/*
Create a cv::Mat.
cv::Mat类用于表示任意维度的稠密数组，“稠密”的意思是该数组的所有部分都有一个值存储。
本程序总结一些常用的创建cv::Mat的方法

Author: Alex
*/

#pragma once
#include <iostream>
#include <opencv2/highgui.hpp>


void createMat()
{
	/**************** 二维数组构造 **************/

	// 默认构造
	cv::Mat m0;

	// 指定宽和高，类型
	cv::Mat m1(3, 3, CV_8UC1);

	// 指定宽和高，类型，初始值
	cv::Mat m2(3, 3, CV_8UC1, cv::Scalar(125));

	// 指定宽和高，类型，预先存储的数据
	uchar p[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
	cv::Mat m3(3, 3, CV_8UC1, *p);

	// 通过指定size来指定宽(4)和高(5)
	cv::Size sz(4, 5);
	cv::Mat m4(sz, CV_8UC1);

	/**************** 多维数组构造 **************/
	// 并没搞清楚到底是怎么创建多维数组的。。。
	int new_sz[3] = { 3, 1, 2 };
	cv::Mat m5(5, *new_sz, CV_16F);
	std::cout << "m5: \n" << m5.dims << std::endl;

	cv::Mat m6(3, *new_sz, CV_8U, cv::Scalar(3));
	std::cout << "m6: \n" << cv::format(m6, cv::Formatter::FMT_NUMPY) << std::endl;

	const int n_mat_size = 5;
	const int n_mat_sz[] = { n_mat_size, n_mat_size, n_mat_size };
	cv::Mat n_mat(3, n_mat_sz, CV_32FC1);

	cv::RNG rng;
	rng.fill(n_mat, cv::RNG::UNIFORM, 0.f, 1.f);
	std::cout << "n_mat: \n" << n_mat.dims << std::endl;

	/**************** 复制构造 **************/
	cv::Mat m7(m6);
	std::cout << "m7: \n" << cv::format(m7, cv::Formatter::FMT_NUMPY) << std::endl;

	cv::Mat m8(m6, cv::Range(0, 2), cv::Range(1, 3));
	std::cout << "m8: \n" << cv::format(m8, cv::Formatter::FMT_NUMPY) << std::endl;

	cv::Vec<int, 5> v1(0, 1, 2, 3, 4);
	cv::Mat m9(v1, true);
	std::cout << "m9: \n" << cv::format(m9, cv::Formatter::FMT_NUMPY) << std::endl;

	std::vector<int> v2 = { 0, 1, 2, 3, 4 };
	cv::Mat m10(v2, true);
	std::cout << "m10: \n" << cv::format(m10, cv::Formatter::FMT_NUMPY) << std::endl;

	cv::Matx<int, 3, 3> mx1(0, 1, 2, 3, 4, 5, 6, 7, 8);
	cv::Mat m11(mx1, true);
	std::cout << "m11: \n" << cv::format(m11, cv::Formatter::FMT_NUMPY) << std::endl;
}


void matType()
{
	std::cout << "CV_8U： " << CV_8U << std::endl;
	std::cout << "CV_8UC1： " << CV_8UC1 << std::endl;
	std::cout << "CV_8UC2： " << CV_8UC2 << std::endl;
	std::cout << "CV_8UC3： " << CV_8UC3 << std::endl;
	std::cout << "CV_8UC4： " << CV_8UC4 << std::endl;
	std::cout << "CV_8S： " << CV_8S << std::endl;
	std::cout << "CV_8SC1： " << CV_8SC1 << std::endl;
	std::cout << "CV_8SC2： " << CV_8SC2 << std::endl;
	std::cout << "CV_8SC3： " << CV_8SC3 << std::endl;
	std::cout << "CV_8SC4： " << CV_8SC4 << std::endl;
	std::cout << "CV_16U： " << CV_16U << std::endl;
	std::cout << "CV_16UC1： " << CV_16UC1 << std::endl;
	std::cout << "CV_16UC2： " << CV_16UC2 << std::endl;
	std::cout << "CV_16UC3： " << CV_16UC3 << std::endl;
	std::cout << "CV_16UC4： " << CV_16UC4 << std::endl;
	std::cout << "CV_16S： " << CV_16S << std::endl;
	std::cout << "CV_16SC1： " << CV_16SC1 << std::endl;
	std::cout << "CV_16SC2： " << CV_16SC2 << std::endl;
	std::cout << "CV_16SC3： " << CV_16SC3 << std::endl;
	std::cout << "CV_16SC4： " << CV_16SC4 << std::endl;
	std::cout << "CV_32S： " << CV_32S << std::endl;
	std::cout << "CV_32SC1： " << CV_32SC1 << std::endl;
	std::cout << "CV_32SC2： " << CV_32SC2 << std::endl;
	std::cout << "CV_32SC3： " << CV_32SC3 << std::endl;
	std::cout << "CV_32SC4： " << CV_32SC4 << std::endl;
	std::cout << "CV_32F： " << CV_32F << std::endl;
	std::cout << "CV_32FC1： " << CV_32FC1 << std::endl;
	std::cout << "CV_32FC2： " << CV_32FC2 << std::endl;
	std::cout << "CV_32FC3： " << CV_32FC3 << std::endl;
	std::cout << "CV_32FC4： " << CV_32FC4 << std::endl;
	std::cout << "CV_64F： " << CV_64F << std::endl;
	std::cout << "CV_64FC1： " << CV_64FC1 << std::endl;
	std::cout << "CV_64FC2： " << CV_64FC2 << std::endl;
	std::cout << "CV_64FC3： " << CV_64FC3 << std::endl;
	std::cout << "CV_64FC4： " << CV_64FC4 << std::endl;
	std::cout << "CV_16F： " << CV_16F << std::endl;
	std::cout << "CV_16FC1： " << CV_16FC1 << std::endl;
	std::cout << "CV_16FC2： " << CV_16FC2 << std::endl;
	std::cout << "CV_16FC3： " << CV_16FC3 << std::endl;
	std::cout << "CV_16FC4： " << CV_16FC4 << std::endl;
	std::cout << "CV_8UC(10)： " << CV_8UC(5) << std::endl;

	std::vector<std::string> p = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F" };
	int types[8] = { CV_8U, CV_8S, CV_16U, CV_16S, CV_32S, CV_32F, CV_64F, CV_16F };
	for (int i = 0; i < 8; i++)
	{
		cv::Mat m(10, 10, types[i]);
		std::cout << "Bytes of " << p[i] << ": " << m.elemSize() << std::endl;
	}
}


void noCopyCreate()
{
	/* 非复制构造 */

	// 默认构造
	cv::Mat m0;

	// 指定尺寸和类型
	const int m_rows = 5; 
	const int m_cols = 5;
	const int m_type = CV_8UC1;
	cv::Mat m1(m_rows, m_cols, m_type);

	// 指定尺寸、类型和初始值
	const cv::Scalar m_init(3);
	cv::Mat m2(m_rows, m_cols, m_type, m_init);

	// 通过cv::Size指定尺寸
	const cv::Size m_sz(m_rows, m_cols);
	cv::Mat m3(m_sz, m_type);
	cv::Mat m4(m_sz, m_type, m_init);

	// 构造多通道矩阵
	const int m_type2 = CV_8UC3;
	cv::Mat m5(m_sz, m_type2);

	// 构造多维数组
	int m_dims = 5;
	const int mul_size[5] = { 5, 4, 3, 2, 1 };
	cv::Mat m6(m_dims, mul_size, m_type);

	std::cout << "m0.empty(): " << m0.empty() << "\tm0.size(): " << m0.size() << std::endl;
	std::cout << "\nm1.empty(): " << m1.empty() << "\tm1.size(): " << m1.size() << std::endl;
	std::cout << "m1: \n" << m1 << std::endl;

	std::cout << "\nm2.empty(): " << m2.empty() << "\tm2.size(): " << m2.size() << std::endl;
	std::cout << "m2: \n" << m2 << std::endl;

	std::cout << "\nm3.empty(): " << m3.empty() << "\tm3.size(): " << m3.size() << std::endl;
	std::cout << "m3: \n" << m3 << std::endl;

	std::cout << "\nm4.empty(): " << m4.empty() << "\tm4.size(): " << m4.size() << std::endl;
	std::cout << "m4: \n" << m4 << std::endl;

	std::cout << "\nm5.empty(): " << m5.empty() << "\tm5.size(): " << m5.size() << std::endl;
	std::cout << "m5: \n" << m5 << std::endl;

	std::cout << "\nDimension of m0 ~ m6: " << m0.dims << ", " << m1.dims << ", " << m2.dims << ", " <<
		m3.dims << ", " << m4.dims << ", " << m5.dims << ", " << m6.dims << std::endl;
	std::cout << "Size of m6: " << std::endl;
	for (int i = 0; i < 5; i++)
	{
		std::cout << "dimension " << i << ": " << m6.size[i] << std::endl;
	}
}


void copyCreate()
{
	// 从数组中复制，也可看作是通过数组赋初值
	uchar p[2][3] = { {1, 2, 3}, {4, 5, 6} };
	cv::Mat m0(2, 3, CV_8UC1, p);

	// 从已有的矩阵中复制
	cv::Mat m1(m0);
	cv::Mat m2(m0, cv::Range(0, 2), cv::Range(1, 3));
	cv::Mat m3(m0, cv::Rect(0, 0, 2, 2));

	// 从其他数据类型复制
	cv::Vec<int, 3> vec0 = { 3, 4, 5 };
	cv::Mat m4(vec0, true);

	cv::Matx<int, 3, 3> mx = { 1, 2, 3, 4, 45, 5, 6, 7, 8 };
	cv::Mat m5(mx, true);

	std::vector<int> vec1 = { 5, 6, 7 };
	cv::Mat m6(vec1, true);

	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
	std::cout << "m3:\n" << m3 << std::endl;
	std::cout << "m4:\n" << m4 << std::endl;
	std::cout << "m5:\n" << m5 << std::endl;
	std::cout << "m6:\n" << m6 << std::endl;

	std::cout << m0.isContinuous() << std::endl;
	std::cout << m1.isContinuous() << std::endl;
	std::cout << m2.isContinuous() << std::endl;
	std::cout << m3.isContinuous() << std::endl;
	std::cout << m4.isContinuous() << std::endl;
	std::cout << m5.isContinuous() << std::endl;
	std::cout << m6.isContinuous() << std::endl;
}


void staticCreate()
{
	cv::Mat m0 = cv::Mat::zeros(3, 3, CV_8UC1);
	cv::Mat m1 = cv::Mat::ones(3, 3, CV_8UC2);
	cv::Mat m2 = cv::Mat::eye(3, 3, CV_8UC3);

	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
}


void createMatx()
{
	// 构造一个固定矩阵类
	cv::Matx22d m0(1, 2, 3, 4);
	cv::Matx<double, 2, 2> m1(1, 2, 3, 4);
	
	// 复制构造
	cv::Matx22d m2(m0);

	// 静态构造
	cv::Matx22d m3 = cv::Matx22d::zeros();

	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
	std::cout << "m3:\n" << m3 << std::endl;

	// 成员访问
	std::cout << "m2(0, 0) = " << m2(0, 0) << std::endl;
}


void createMat_()
{
	// 默认构造
	cv::Mat_<uchar> m0(3, 3);

	// 赋初值
	cv::Mat_<uchar> m1 = (cv::Mat_<uchar>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

	// 复制构造
	cv::Mat_<uchar>m2(m1);

	// 静态构造
	cv::Mat_<cv::Vec3b> m3 = cv::Mat_<cv::Vec3b>::ones(3, 3);

	// 与Mat交流
	cv::Mat m4(3, 3, CV_64FC1, cv::Scalar(128));
	cv::Mat_<double>& m4_ = (cv::Mat_<double>&) m4;
	cv::Mat_<double> m4__(m4);
	
	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
	std::cout << "m3:\n" << m3 << std::endl;
	std::cout << "m4:\n" << m4 << std::endl;
	std::cout << "m4_:\n" << m4_ << std::endl;
	std::cout << "m4__:\n" << m4__ << std::endl;

	std::cout << "&m4:\n" << &m4 << std::endl;
	std::cout << "&m4_:\n" << &m4_ << std::endl;
	std::cout << "&m4__:\n" << &m4__ << std::endl;

	// 成员访问
	std::cout << "m2(1, 1) = " << (int)m2(1, 1) << std::endl;
	std::cout << "m2.at<uchar>(1, 1) = " << (int)m2.at<uchar>(1, 1) << std::endl;
}