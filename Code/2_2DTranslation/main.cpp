

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


// ∑¬…‰
void affine(cv::Mat& src, cv::Mat& dst, const cv::Point2f* src_pts, const cv::Point2f* dst_pts)
{
	cv::Mat translator = cv::getAffineTransform(src_pts, dst_pts);
	cv::warpAffine(src, dst, translator, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
}

// Õ∂”∞
void project(cv::Mat& src, cv::Mat& dst, const cv::Point2f* src_pts, const cv::Point2f* dst_pts)
{
	cv::Mat translator = cv::getPerspectiveTransform(src_pts, dst_pts);
	cv::warpPerspective(src, dst, translator, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
}

// 2D±‰ªª≤‚ ‘
void twoDTransTest()
{
	std::string im_path = "C:/Users/acer/Pictures/saved_pictures/lazySheep.jpg";
	cv::Mat im = cv::imread(im_path);
	cv::Point2f src_pts[3] = { {0, 0}, {500, 0}, {500, 500} };
	cv::Point2f dst_pts[3] = { {0, 0}, {500, 0}, {500, 500} };
	cv::Mat translated_im = cv::Mat::zeros(im.rows * 2, im.cols * 3, CV_8UC3);
	im.copyTo(translated_im.rowRange(0, im.rows).colRange(0, im.cols));

	// Translate
	cv::Mat temp;
	dst_pts[0] = { 100, 100 };
	dst_pts[1] = { 600, 100 };
	dst_pts[2] = { 600, 600 };
	affine(im, temp, src_pts, dst_pts);
	temp.copyTo(translated_im.rowRange(0, im.rows).colRange(im.cols, 2 * im.cols));

	// Rotation
	dst_pts[0] = { 1000, 0 };
	dst_pts[1] = { 1000, 500 };
	dst_pts[2] = { 500, 500 };
	affine(im, temp, src_pts, dst_pts);
	temp.copyTo(translated_im.rowRange(0, im.rows).colRange(2 * im.cols, 3 * im.cols));

	// Scale
	dst_pts[0] = { 0, 0 };
	dst_pts[1] = { 250, 0 };
	dst_pts[2] = { 250, 250 };
	affine(im, temp, src_pts, dst_pts);
	temp.copyTo(translated_im.rowRange(im.rows, 2 * im.rows).colRange(0, im.cols));

	// Affine
	dst_pts[0] = { 250, 100 };
	dst_pts[1] = { 500, 0 };
	dst_pts[2] = { 750, 400 };
	affine(im, temp, src_pts, dst_pts);
	temp.copyTo(translated_im.rowRange(im.rows, 2 * im.rows).colRange(im.cols, 2 * im.cols));

	// Project
	cv::Point2f src_pjt[4] = { {0, 0}, {500, 0}, {500, 500}, {0, 500} };
	cv::Point2f dst_pjt[4] = { {100, 200}, {400, 100}, {400, 400}, {100, 400} };
	project(im, temp, src_pjt, dst_pjt);
	temp.copyTo(translated_im.rowRange(im.rows, 2 * im.rows).colRange(2 * im.cols, 3 * im.cols));

	cv::imwrite("C:/Users/acer/Pictures/saved_pictures/lazySheeps.png", translated_im);
}



int main(int argc, char** argv)
{
	twoDTransTest();

	system("pause");
	return 0;
}

