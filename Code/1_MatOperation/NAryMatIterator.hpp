/*
一些NAryMatIterator的示例
心得：NAryMatIterator可以将多个矩阵转换为不同的面，通过面来进行计算。但是我觉得并没什么实际用处。。。

Author: Alex
*/


#pragma once
#include <iostream>
#include <opencv2/highgui.hpp>


void sumOneNAry()
{
	/* 按面将一个多维数组相加 */
	const int n_mat_size = 5;
	const int n_mat_sz[] = { n_mat_size, n_mat_size, n_mat_size };
	cv::Mat n_mat(3, n_mat_sz, CV_32FC1);

	cv::RNG rng;
	rng.fill(n_mat, rng.UNIFORM, 0.f, 1.f);

	const cv::Mat* arrays[] = { &n_mat, 0 };
	cv::Mat my_planes[1];
	cv::NAryMatIterator it(arrays, my_planes);

	float s = 0.f;
	int n = 0;
	for (int p = 0; p < it.nplanes; p++, ++it)
	{
		std::cout << "Plane " << p << ": " << it.planes[0] << std::endl;
		s += cv::sum(it.planes[0])[0];
		n++;
	}

	std::cout << "Sum of n_mat: " << s << std::endl;
	std::cout << "Planes of it: " << n << std::endl;
}


void sumTwoNAry()
{
	/* 使用N-ary将两个数组相加 */
	const int n_mat_size = 5;
	const int n_mat_sz[] = { n_mat_size, n_mat_size, n_mat_size, n_mat_size };
	cv::Mat n_mat0(4, n_mat_sz, CV_32FC1);
	cv::Mat n_mat1(4, n_mat_sz, CV_32FC1);

	cv::RNG rng;
	rng.fill(n_mat0, rng.UNIFORM, 0.f, 1.f);
	rng.fill(n_mat1, rng.UNIFORM, 0.f, 1.f);

	const cv::Mat* arrays[] = { &n_mat0, &n_mat1, 0 };
	cv::Mat my_planes[2];
	cv::NAryMatIterator it(arrays, my_planes);

	float s = 0.f;
	int n = 0;
	for (int p = 0; p < it.nplanes; p++, ++it)
	{
		std::cout << "Size of a plane: " << it.planes[0].size() << std::endl;
		s += cv::sum(it.planes[0])[0];
		s += cv::sum(it.planes[1])[0];
		n++;
	}
	std::cout << "Sum of n_mat0 and n_mat1: " << s << std::endl;
	std::cout << "Planes of it: " << n << std::endl;
	std::cout << "Size of it: " << it.size << std::endl;
	std::cout << "Sum by cv::sum: " << cv::sum(n_mat0)[0] + cv::sum(n_mat1)[0] << std::endl;
}

