/*
	The main function is an example of video dehazing
	The core algorithm is in "dehazing.cpp," "guidedfilter.cpp," and "transmission.cpp".
	You may modify the code to improve the results.

	The detailed description of the algorithm is presented
	in "http://mcl.korea.ac.kr/projects/dehazing". See also
	J.-H. Kim, W.-D. Jang, Y. Park, D.-H. Lee, J.-Y. Sim, C.-S. Kim, "Temporally
	coherent real-time video dehazing," in Proc. IEEE ICIP, 2012.

	Last updated: 2013-02-14
	Author: Jin-Hwan, Kim.
 */

#include "dehazing.h"
#include <time.h>
#include <conio.h>
#include <iostream>


void video_test(char** argv)
{
	const char* src_path = "E:/Projects/2021/TheThingsWithImage/Data/dehazing/hazeroad.avi";
	const char* dst_path = "E:/Projects/2021/TheThingsWithImage/Data/dehazing/dehazeroad.avi";

	cv::VideoCapture cvSequence(src_path);	// argv[1]

	int nWid = (int)cvSequence.get(cv::CAP_PROP_FRAME_WIDTH);  //atoi(argv[3]);
	int nHei = (int)cvSequence.get(cv::CAP_PROP_FRAME_HEIGHT); //atoi(argv[4]);

	cv::VideoWriter vwSequenceWriter(dst_path, 0, 25, cv::Size(nWid, nHei), true);	// argv[2]

	cv::Mat imInput;
	cv::Mat imOutput(cv::Size(nWid, nHei), CV_8UC3);

	int nFrame;

	dehazing dehazingImg(nWid, nHei, 16, false, false, 5.0f, 1.0f, 40);

	time_t start_t = clock();

	for (nFrame = 0; nFrame < cvSequence.get(cv::CAP_PROP_FRAME_COUNT); nFrame++)	//atoi(argv[3])
	{
		bool read_success = cvSequence.read(imInput);
		if (!read_success)
		{
			std::cout << "Fail to " << nFrame << "th frame!" << std::endl;
			continue;
		}

		dehazingImg.HazeRemoval(imInput, imOutput, nFrame);
		vwSequenceWriter.write(imOutput);
	}

	std::cout << nFrame << " frames " << (float)(clock() - start_t) / CLOCKS_PER_SEC << "secs" << endl;

	system("pause");

	cvSequence.release();
	imOutput.release();
}


void image_test()
{
	const char* image_path = "E:/Projects/2021/TheThingsWithImage/Data/dehazing/haze_city0.png";
	cv::Mat imInput = cv::imread(image_path);
	if (imInput.empty())
	{
		return;
	}

	cv::Mat imOutput(imInput.size(), CV_8UC3);
	dehazing dehazingImg(imInput.cols, imInput.rows, 16, false, false, 5.0f, 1.0f, 40);
	dehazingImg.HazeRemoval(imInput, imOutput, 0);
	
	const char* save_path = "E:/Projects/2021/TheThingsWithImage/Data/dehazing/dehaze_city0.png";
	cv::imwrite(save_path, imOutput);
}



int main(int argc, char** argv)
{
	video_test(argv);
	//image_test();

	return 0;
}
