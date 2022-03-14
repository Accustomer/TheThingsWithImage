#pragma once
#include <opencv2/imgproc.hpp>

/*
Implement Guided Filter.
@param guidance: Guidance image
@param src: Source image
@param dst: Output image
@param eps: Regularization parameter
*/

void guidedFilter(cv::Mat& guidiance, cv::Mat& src, cv::Mat& dst, const int r = 3, float eps = 25.f);



void guidedFilter(cv::Mat& guidiance, cv::Mat& src, cv::Mat& dst, const int r, float eps)
{
	// Convert to float
	cv::Mat p, I;
	src.convertTo(p, CV_32F);
	guidiance.convertTo(I, CV_32F);

	// Guided Filtering	
	const cv::Size wsz(r, r);
	cv::Mat mean_p, mean_I, mean_Ip, mean_II;
	cv::blur(p, mean_p, wsz);
	cv::blur(I, mean_I, wsz);
	cv::blur(p.mul(I), mean_Ip, wsz);
	cv::blur(I.mul(I), mean_II, wsz);

	cv::Mat a, b, am, bm;
	a = (mean_Ip - mean_p.mul(mean_I)) / (mean_II - mean_I.mul(mean_I) + eps);
	b = mean_p - a.mul(mean_I);
	cv::blur(a, am, wsz);
	cv::blur(b, bm, wsz);

	dst = am.mul(I) + bm;
	dst.convertTo(dst, CV_8U);
}