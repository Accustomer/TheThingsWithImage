#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>



/*
统计一张单通道图像的像素值直方图
@param image: 单通道图像
@return: 长度为256的直方图数组
*/
void statHist(const cv::Mat& image, int* hist);

/*
计算直方图均衡化的映射表
@param hist: 单通道图像素值的直方图
@param size: 图像所有的像素数
@return: 长度为256的映射数组
*/
void mapTable(const int* hist, const int size, uchar* table);

/*
映射
@param image: 单通道图像
@param table: 映射表
@return: 映射后的单通道图像
*/
void map(const cv::Mat& src, const uchar* table, cv::Mat& dst);

/*
返回大于等于a的数中，最小的b整数倍数

*/
int minInteger(const int a, const int b);

/*
直方图均衡化
@param src: 单通道图像
@param dst: 均衡化的单通道图像
*/
void he(const cv::Mat& src, cv::Mat& dst);

/*
局部直方图均衡化
@param src: 单通道图像
@param dst: 均衡化的单通道图像
@param m: 小块的尺寸
*/
void lhe(const cv::Mat& src, cv::Mat& dst, const int m=200);

/*
局部自适应直方图均衡化
@param src: 单通道图像
@param dst: 均衡化的单通道图像
@param m: 小块的尺寸
*/
void lahe(const cv::Mat& src, cv::Mat& dst, const int m = 50);


/*
限制对比度全局直方图均衡
@param src: 单通道图像
@param dst: 均衡化的单通道图像
@param clip_limit: 限制对比度的策略（均值的倍数）
*/
void che(const cv::Mat& src, cv::Mat& dst, const float clip_limit = 4.f);


/*
限制对比度自适应直方图均衡
@param src: 单通道图像
@param dst: 均衡化的单通道图像
@param clip_limit: 限制对比度的策略（均值的倍数）
@param m: 小块的尺寸
*/
void clahe(const cv::Mat& src, cv::Mat& dst, const float clip_limit = 4.f, const int m = 200);




void statHist(const cv::Mat& image, int* hist)
{
	int idx = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			idx = int(image.at<uchar>(i, j));
			hist[idx]++;
		}
	}
}


void mapTable(const int* hist, const int size, uchar* table)
{
	float factor = 256.f / size;
	int map_value, cumu_num = 0;
	for (int i = 0; i < 256; i++)
	{
		cumu_num += hist[i];
		map_value = int(cumu_num * factor);
		if (map_value > 255) map_value = 255;
		table[i] = uchar(map_value);
	}
}


void map(const cv::Mat& src, const uchar* table, cv::Mat& dst)
{
	src.copyTo(dst);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst.at<uchar>(i, j) = table[(int)src.at<uchar>(i, j)];
		}
	}
}


int minInteger(const int a, const int b)
{
	const int res = a % b;
	return res == 0 ? a : a + b - res;
}


void he(const cv::Mat& src, cv::Mat& dst)
{
	int hist[256] = { 0 };
	uchar table[256] = { 0 };
	statHist(src, hist);
	mapTable(hist, src.rows * src.cols, table);
	map(src, table, dst);
}


void lhe(const cv::Mat& src, cv::Mat& dst, const int m)
{
	// Padding
	cv::Mat im_pad;
	const int h = src.rows, w = src.cols;
	const int pdh = minInteger(h, m), pdw = minInteger(w, m);
	const int res_h = pdh - h, res_w = pdw - w;
	const int left = res_w / 2, top = res_h / 2;
	const int right = res_w - left, bottom = res_h - top;
	cv::copyMakeBorder(src, im_pad, top, bottom, left, right, cv::BORDER_DEFAULT);

	// Equalization
	int i = 0, j = 0, ni = m, nj = m;
	while (i < pdh)
	{
		ni = i + m;
		while (j < pdw)
		{
			nj = j + m;
			equalizeHist(im_pad.rowRange(i, ni).colRange(j, nj), im_pad.rowRange(i, ni).colRange(j, nj));
			j = nj;
		}
		i = ni;
		j = 0;
	}
	im_pad.rowRange(top, h + top).colRange(left, w + left).copyTo(dst);
}


void lahe(const cv::Mat& src, cv::Mat& dst, const int m)
{
	// Padding
	cv::Mat im_pad;
	const int psz = m / 2;
	const int hend = src.rows + psz;
	const int wend = src.cols + psz;
	cv::copyMakeBorder(src, im_pad, psz, psz, psz, psz, cv::BORDER_DEFAULT);
	src.copyTo(dst);

	// Equalization
	int hist[256] = { 0 };
	float factor = 256.f / (m * m);
	int pi = 0, pj = 0, val = 0, cumv = 0;
	for (int i = psz; i < hend; i++)
	{
		for (int j = psz; j < wend; j++)
		{
			if (j == psz)
			{
				// Reset
				for (int pi = 0; pi < 256; pi++)
				{
					hist[pi] = 0;
				}

				// Statistic block
				for (pi = i - psz; pi <= i + psz; pi++)
				{
					for (pj = j - psz; pj <= j + psz; pj++)
					{
						val = (int)im_pad.at<uchar>(pi, pj);
						hist[val]++;
					}
				}
			}
			else
			{
				// Pop left
				pj = j - psz - 1;
				for (pi = i - psz; pi <= i + psz; pi++)
				{
					val = (int)im_pad.at<uchar>(pi, pj);
					hist[val]--;
				}

				// Push right
				pj = j + psz;
				for (pi = i - psz; pi <= i + psz; pi++)
				{
					val = (int)im_pad.at<uchar>(pi, pj);
					hist[val]++;
				}
			}
			cumv = 0;
			val = (int)im_pad.at<uchar>(i, j);
			for (pi = 0; pi <= val; pi++)
			{
				cumv += hist[pi];
			}
			dst.at<uchar>(i - psz, j - psz) = uchar(cumv * factor);
		}
	}
}


void che(const cv::Mat& src, cv::Mat& dst, const float clip_limit)
{
	// Statistical histogram
	const int h = src.rows;
	const int w = src.cols;
	int hist[256] = { 0 };
	uchar table[256] = { 0 };
	statHist(src, hist);

	// Limitation
	int steal = 0;
	const int limit = h * w * clip_limit / 256;
	for (int k = 0; k < 256; k++)
	{
		if (hist[k] > limit)
		{
			steal += hist[k] - limit;
			hist[k] = limit;
		}
	}

	// Hand out the steals averagely
	const int bonus = steal / 256;
	for (int k = 0; k < 256; k++)
	{
		hist[k] += bonus;
	}

	// Get mapping table
	mapTable(hist, h*w, table);

	// Map
	map(src, table, dst);
}


void clahe(const cv::Mat& src, cv::Mat& dst, const float clip_limit, const int m)
{
	// Padding
	cv::Mat im_pad;
	const int h = src.rows, w = src.cols;
	const int pdh = minInteger(h, m), pdw = minInteger(w, m);
	const int res_h = pdh - h, res_w = pdw - w;
	const int left = res_w / 2, top = res_h / 2;
	const int right = res_w - left, bottom = res_h - top;
	cv::copyMakeBorder(src, im_pad, top, bottom, left, right, cv::BORDER_DEFAULT);

	// Initialize histogram
	const int hnum = pdh / m;
	const int wnum = pdw / m;
	const int bnum = hnum * wnum;
	int** hists = new int*[bnum];
	float** table = new float* [bnum];
	for (int i = 0; i < bnum; i++)
	{
		hists[i] = new int[256];
		table[i] = new float[256];
		for (int j = 0; j < 256; j++)
		{
			hists[i][j] = 0;
			table[i][j] = 0.f;
		}
	}

	// Statistical histogram, Get mapping table
	const int benum = m * m;
	const int limit = benum * clip_limit / 256;
	int idx = 0, steal = 0, bonus = 0;
	for (int i = 0; i < hnum; i++)
	{
		for (int j = 0; j < wnum; j++)
		{
			// Statistical
			idx = i * wnum + j;
			statHist(im_pad.rowRange(i * m, (i + 1) * m).colRange(j * m, (j + 1) * m), hists[idx]);
			
			// Limitation
			steal = 0;
			for (int k = 0; k < 256; k++)
			{
				if (hists[idx][k] > limit)
				{
					steal += hists[idx][k] - limit;
					hists[idx][k] = limit;
				}
			}

			// Hand out the steals averagely
			bonus = steal / 256;
			for (int k = 0; k < 256; k++)
			{
				hists[idx][k] += bonus;
			}

			// Cumulative --> table
			table[idx][0] = hists[idx][0] * 255.f / benum;
			for (int k = 1; k < 256; k++)
			{
				table[idx][k] = table[idx][k - 1] + hists[idx][k] * 255.f / benum;
			}
		}
	}
	delete[] hists;

	// Equalization and Interpolation
	const int hm = m / 2;
	//im_pad.copyTo(im_pad);
	int hbi = 0, wbi = 0;
	int bidx[4] = { 0 };
	float p = 0.f, q = 0.f;
	for (int i = 0; i < pdh; i++)
	{
		for (int j = 0; j < pdw; j++)
		{
			//four coners  
			if (i <= hm && j <= hm)
			{
				idx = 0;
				im_pad.at<uchar>(i, j) = (uchar)(table[idx][im_pad.at<uchar>(i, j)]);
			}
			else if (i <= hm && j >= pdw - hm)
			{
				idx = wnum - 1;
				im_pad.at<uchar>(i, j) = (uchar)(table[idx][im_pad.at<uchar>(i, j)]);
			}
			else if (i >= pdh - hm && j <= hm)
			{
				idx = bnum - wnum;
				im_pad.at<uchar>(i, j) = (uchar)(table[idx][im_pad.at<uchar>(i, j)]);
			}
			else if (i >= pdh - hm && j >= pdw - hm)
			{
				idx = bnum - 1;
				im_pad.at<uchar>(i, j) = (uchar)(table[idx][im_pad.at<uchar>(i, j)]);
			}
			//four edges except coners -- linear interpolation
			else if (i <= hm)
			{
				// hbi = 0;
				wbi = (j - hm) / m;
				bidx[0] = wbi;
				bidx[1] = bidx[0] + 1;
				p = (float)(j - (wbi * m + hm)) / m;
				q = 1 - p;
				im_pad.at<uchar>(i, j) = (uchar)(q * table[bidx[0]][im_pad.at<uchar>(i, j)] + p * table[bidx[1]][im_pad.at<uchar>(i, j)]);
			}
			else if (i >= ((hnum - 1) * m + hm))
			{
				hbi = hnum - 1;
				wbi = (j - hm) / m;
				bidx[0] = hbi * wnum + wbi;
				bidx[1] = bidx[0] + 1;
				float p = (float)(j - (wbi * m + hm)) / m;
				float q = 1 - p;
				im_pad.at<uchar>(i, j) = (uchar)(q * table[bidx[0]][im_pad.at<uchar>(i, j)] + p * table[bidx[1]][im_pad.at<uchar>(i, j)]);
			}
			else if (j <= hm)
			{
				hbi = (i - hm) / m;
				//wbi = 0;
				bidx[0] = hbi * wnum;
				bidx[1] = bidx[0] + wnum;
				p = (float)(i - (hbi * m + hm)) / m;
				q = 1 - p;
				im_pad.at<uchar>(i, j) = (uchar)(q * table[bidx[0]][im_pad.at<uchar>(i, j)] + p * table[bidx[1]][im_pad.at<uchar>(i, j)]);
			}
			else if (j >= ((wnum - 1) * m + hm))
			{
				hbi = (i - hm) / m;
				wbi = wnum - 1;
				bidx[0] = hbi * wnum + wbi;
				bidx[1] = bidx[0] + wnum;
				p = (float)(i - (hbi * m + hm)) / m;
				q = 1 - p;
				im_pad.at<uchar>(i, j) = (uchar)(q * table[bidx[0]][im_pad.at<uchar>(i, j)] + p * table[bidx[1]][im_pad.at<uchar>(i, j)]);
			}
			// Double linear interpolation
			else
			{
				hbi = (i - hm) / m;
				wbi = (j - hm) / m;
				bidx[0] = hbi * wnum + wbi;
				bidx[1] = bidx[0] + 1;
				bidx[2] = bidx[0] + wnum;
				bidx[3] = bidx[1] + wnum;
				p = (float)(i - (hbi * m + hm)) / m;
				q = (float)(j - (wbi * m + hm)) / m;
				im_pad.at<uchar>(i, j) = (uchar)(
					(1 - p) * (1 - q) * table[bidx[0]][im_pad.at<uchar>(i, j)] +
					(1 - p) * q * table[bidx[1]][im_pad.at<uchar>(i, j)] +
					p * (1 - q) * table[bidx[2]][im_pad.at<uchar>(i, j)] +
					p * q * table[bidx[3]][im_pad.at<uchar>(i, j)]);
			}
			im_pad.at<uchar>(i, j) = im_pad.at<uchar>(i, j) + (im_pad.at<uchar>(i, j) << 8) + (im_pad.at<uchar>(i, j) << 16);
		}
	}
	im_pad.rowRange(top, top + h).colRange(left, left + w).copyTo(dst);

	delete[] table;
}