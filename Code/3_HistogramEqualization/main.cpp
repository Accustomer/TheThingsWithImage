
#include <iostream>
#include <immintrin.h>
#include "functions.hpp"


void test()
{
	const char* im_path = "C:/Users/acer/Pictures/saved_pictures/nangongwan.jpg";
	cv::Mat bgr, gray, heim1, heim2, lheim, laheim, cheim, claheim;
	bgr = cv::imread(im_path);
	cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
	he(gray, heim1);
	cv::equalizeHist(gray, heim2);
	lhe(gray, lheim, 200);
	//lahe(gray, laheim, 50);
	//cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
	//clahe->apply(gray, claheim);
	che(gray, cheim);
	clahe(gray, claheim);
	
	const std::string save_dir = "E:/Projects/2021/TheThingsWithImage/Data/";
	cv::imwrite(save_dir + "nangongwan_gray.png", gray);
	cv::imwrite(save_dir + "nangongwan_globalhe.png", heim1);
	cv::imwrite(save_dir + "nangongwan_lhe.png", lheim);
	//cv::imwrite(save_dir + "nangongwan_lahe.png", laheim);
	cv::imwrite(save_dir + "nangongwan_che.png", cheim);
	cv::imwrite(save_dir + "nangongwan_clahe.png", claheim);
}

void test1()
{
	const char* im_path = "C:/Users/acer/Pictures/saved_pictures/nangongwan.jpg";
	cv::Mat bgr, hsv, bgr2;
	cv::Mat p[3];
	bgr = cv::imread(im_path);
	cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);
	cv::split(hsv, p);
	//clahe(p[0], p[0]);
	//clahe(p[1], p[1]);
	clahe(p[2], p[2]);
	cv::merge(p, 3, hsv);
	cv::cvtColor(hsv, bgr2, cv::COLOR_HSV2BGR);

	const std::string save_dir = "E:/Projects/2021/TheThingsWithImage/Data/";
	cv::imwrite(save_dir + "nangongwan_bgr1.png", bgr2);
}


int main()
{
	//test();
	test1();
	
	system("pause");
	return 0;
}

