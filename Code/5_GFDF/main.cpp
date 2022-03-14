#include "gfdf.hpp"
#include <opencv2/highgui.hpp>


void test3()
{
	std::string impath1 = "E:/Projects/2021/TheThingsWithImage/Data/Images/toy1.bmp";
	std::string impath2 = "E:/Projects/2021/TheThingsWithImage/Data/Images/toy2.bmp";
	std::string impath3 = "E:/Projects/2021/TheThingsWithImage/Data/Images/toy3.bmp";
	
	cv::Mat im1 = cv::imread(impath1);
	cv::Mat im2 = cv::imread(impath2);
	cv::Mat im3 = cv::imread(impath3);
	cv::Mat imv[3] = { im1, im2, im3 };

	cv::Mat imfu;
	GfdfParam GP{ 0.01, 7, 5, 0.3f };
	gfdf3(imv, imfu, 3, GP);

	cv::imwrite("E:/Projects/2021/TheThingsWithImage/Data/fuToy.png", imfu);
}


void test2()
{
	std::string impath1 = "E:/Projects/2021/TheThingsWithImage/Data/Images/lytro-05-A.jpg";
	std::string impath2 = "E:/Projects/2021/TheThingsWithImage/Data/Images/lytro-05-B.jpg";

	cv::Mat im1 = cv::imread(impath1);
	cv::Mat im2 = cv::imread(impath2);
	cv::Mat imv[2] = { im1, im2 };

	cv::Mat imfu;
	GfdfParam GP{ 0.01, 7, 5, 0.3f };
	gfdf3(imv, imfu, 2, GP);

	cv::imwrite("E:/Projects/2021/TheThingsWithImage/Data/fuLytro-05.png", imfu);
}




int main()
{
	test2();
	//test3();

	system("pause");
}


