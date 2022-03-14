/*
Implemnt GFDF: Guided filter-based multi-focus image fusion through focus region detection

Author: Alex
*/
#pragma once
#include <opencv2/imgproc.hpp>

/*
Parameters of GFDF
*/
struct GfdfParam
{
	double ar = 0.01;	// Area smaller than (ar * image size) will be removed
	int msz = 7;	// Mean filter size
	int gsz = 5;	// Guided filter size
	float eps = 0.3f;	// Guided filter regularization
};

/*
Guided filter.
@param guidiance: guidiance map
@param src: Source input image
@param dst: Filtered output image
@param r: Kernel size
@param eps: Regularization parameter
*/
void guidedFilter(cv::Mat& guidiance, cv::Mat& src, cv::Mat& dst, const int r = 3, float eps = 25.f);

/*
Focus region detection.
@param src: Source single-channel image
@param dst: Detected focus region
@param msz: Kernel size for mean filter
@param gsz: Kernel size for guided filter
@param eps: Regularization parameter
*/
void frd(cv::Mat &src, cv::Mat &dst, const int msz, const int gsz, const float eps);

/*
Filter small regions and return them.
@param src: Mask with many regions
@param dst: Mask without small regions
@param area_thresh: Min area allowed
@return: Regions be filtered
*/
cv::Mat filterSmallRegions(cv::Mat& src, cv::Mat& dst, const double area_thresh);

/*
GFDF for bgr-channels image.
@param srcs: The pointer of source images
@param dst: Fused image
@param num: Number of source images
@param GP: GFDF parameters
*/
void gfdf3(cv::Mat* srcs, cv::Mat& dst, const int num, GfdfParam& GP);







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
}


void frd(cv::Mat& src, cv::Mat& dst, const int msz, const int gsz, const float eps)
{
	cv::Mat src_blur, src_diff;
	cv::blur(src, src_blur, { msz, msz });
	cv::absdiff(src, src_blur, src_diff);
	guidedFilter(src, src_diff, dst, gsz, eps);
}


cv::Mat filterSmallRegions(cv::Mat& src, cv::Mat& dst, const double area_thresh)
{
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(src, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	std::vector<std::vector<cv::Point>> filtered_contours;
	for (int i = 0; i < contours.size(); i++)
	{
		const double area = cv::moments(contours[i]).m00;
		if (area < area_thresh)
			filtered_contours.push_back(contours[i]);
	}

	cv::Mat filtered_mask = cv::Mat::zeros(src.size(), CV_8UC1);
	cv::fillPoly(filtered_mask, filtered_contours, 255);
	
	src.copyTo(dst);
	dst.setTo(0, filtered_mask);
	return filtered_mask;
}


void gfdf3(cv::Mat* srcs, cv::Mat& dst, const int num, GfdfParam& GP)
{
	const double factor = 1.0 / 255;
	const int h = srcs[0].rows;
	const int w = srcs[0].cols;
	const double min_area = GP.ar * h * w;

	cv::Mat* _srcs = new cv::Mat[num];
	cv::Mat* _grays = new cv::Mat[num];
	cv::Mat* afms = new cv::Mat[num];
	cv::Mat maxfm = cv::Mat::zeros(h, w, CV_32FC1);
	cv::Mat idm = cv::Mat::zeros(h, w, CV_32SC1);
	for (int i = 0; i < num; i++)
	{
		// Preprocessing
		srcs[i].convertTo(_srcs[i], CV_32FC1, factor);
		cv::cvtColor(_srcs[i], _grays[i], cv::COLOR_BGR2GRAY);

		// Focus region detection
		frd(_grays[i], afms[i], GP.msz, GP.gsz, GP.eps);

		// Initial decision <-----> Maximum
		cv::Mat larger_mask;
		cv::compare(afms[i], maxfm, larger_mask, cv::CMP_GT);
		afms[i].copyTo(maxfm, larger_mask);
		idm.setTo(i, larger_mask);
	}

	// Filter small decision regions
	cv::Mat* idms = new cv::Mat[num];
	cv::Mat undecided_mask = cv::Mat::zeros(h, w, CV_8UC1);
	for (int i = 0; i < num; i++)
	{
		cv::compare(idm, i, idms[i], cv::CMP_EQ);
		cv::Mat one_undecide = filterSmallRegions(idms[i], idms[i], min_area);
		undecided_mask.setTo(255, one_undecide);
	}

	// Decide undecided regions by nearest decided region
	cv::Mat min_dist(h, w, CV_32FC1, cv::Scalar::all(h));
	for (int i = 0; i < num; i++)
	{
		cv::Mat inv_mask, dist, smaller_mask;
		cv::subtract(255, idms[i], inv_mask);
		cv::distanceTransform(inv_mask, dist, cv::DIST_L1, 3, CV_32FC1);
		
		cv::compare(dist, min_dist, smaller_mask, cv::CMP_LT);
		dist.copyTo(min_dist, smaller_mask);
		cv::bitwise_and(smaller_mask, undecided_mask, smaller_mask);
		idm.setTo(i, smaller_mask);
	}

	// Finally decide and Directly fusion
	cv::Mat imdfu(h, w, CV_32FC1);
	for (int i = 0; i < num; i++)
	{
		cv::compare(idm, i, idms[i], cv::CMP_EQ);
		_grays[i].copyTo(imdfu, idms[i]);
	}

	// Get fusion decision map
	cv::Mat* fdms = new cv::Mat[num];
	cv::Mat sum_fdms = cv::Mat::zeros(h, w, CV_32FC1);
	for (int i = 0; i < num; i++)
	{
		guidedFilter(imdfu, idms[i], fdms[i], GP.gsz, GP.eps);
		cv::add(fdms[i], sum_fdms, sum_fdms);
	}

	// Funsion
	cv::Mat imfu = cv::Mat::zeros(h, w, CV_32FC3);
	for (int i = 0; i < num; i++)
	{
		cv::divide(fdms[i], sum_fdms, fdms[i]);
		cv::Mat m3[3] = { fdms[i], fdms[i], fdms[i] };
		cv::merge(m3, 3, fdms[i]);

		cv::add(imfu, _srcs[i].mul(fdms[i]), imfu);
	}
	imfu.convertTo(dst, CV_8UC1, 255);

	delete[] _srcs;
	delete[] _grays;
	delete[] afms; 
	delete[] idms; 
	delete[] fdms;
}