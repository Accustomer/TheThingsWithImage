/*
	This source file contains additional function for dehazing algorithm.

	The detailed description of the algorithm is presented
	in "http://mcl.korea.ac.kr/projects/dehazing". See also
	J.-H. Kim, W.-D. Jang, Y. Park, D.-H. Lee, J.-Y. Sim, C.-S. Kim, "Temporally
	coherent real-time video dehazing," in Proc. IEEE ICIP, 2012.

	Last updated: 2013-02-06
	Author: Jin-Hwan, Kim.
 */

#include "dehazing.h"

 /*
	 Function: IplImageToInt
	 Description: Convert the opencv type IplImage to integer

	 Parameters:
		 imInput - input IplImage
	 Return:
		 m_pnYImg - output integer array
 */
void dehazing::IplImageToInt(cv::Mat& imInput)
{
	int nY, nX;
	uchar* inptr = imInput.data;

	int n_pixel = 0, n_step = 0;
	for (nY = 0; nY < m_nHei; nY++)
	{
		for (nX = 0; nX < m_nWid; nX++)
		{
			// (1) IplImage 를 YUV의 Y채널로 변환 하여 int형 배열 m_pnYImg에 저장
			m_pnYImg[n_pixel] = inptr[n_step++] * 7471;
			m_pnYImg[n_pixel] += inptr[n_step++] * 38470;
			m_pnYImg[n_pixel] += inptr[n_step++] * 19595;
			m_pnYImg[n_pixel] = m_pnYImg[n_pixel] >> 16;	// divide 65536
			n_pixel++;
		}
	}
}

/*
	Function: IplImageToIntColor
	Description: Convert the opencv type IplImage to integer (3 arrays)

	Parameters:
		imInput - input IplImage
	Return:
		m_pnRImg - output integer arrays R
		m_pnGImg - output integer arrays G
		m_pnBImg - output integer arrays B
*/
void dehazing::IplImageToIntColor(cv::Mat& imInput)
{
	int nY, nX;
	uchar* inptr = imInput.data;

	int n_pixel = 0, n_step = 0;
	for (nY = 0; nY < m_nHei; nY++)
	{
		for (nX = 0; nX < m_nWid; nX++)
		{
			m_pnBImg[n_pixel] = inptr[n_step++];
			m_pnGImg[n_pixel] = inptr[n_step++];
			m_pnRImg[n_pixel] = inptr[n_step++];
			n_pixel++;
		}
	}
}

/*
	Function: DownsampleImage
	Description: Downsample the image to fixed sized image (320 x 240)

	Parameters:(hidden)
		m_pnYImg - input Y Image
	Return:
		m_pnSmallYImg - output down sampled image
*/
void dehazing::DownsampleImage()
{
	int nX, nY;

	float fRatioY, fRatioX;
	// 다운샘플링 비율 결정
	fRatioX = (float)m_nWid / (float)320;
	fRatioY = (float)m_nHei / (float)240;

	int n_pixel = 0;
	float fry = 0, frx = 0;
	for (nY = 0; nY < 240; nY++)
	{
		for (nX = 0; nX < 320; nX++)
		{
			// (1) 멤버 변수인 m_pnYImg를 m_pnSmallYImg로 다운샘플링(크기는 320x240)
			m_pnSmallYImg[n_pixel++] = m_pnYImg[(int)fry * m_nWid + (int)frx];
			frx += fRatioX;
		}
		fry += fRatioY;
		frx = 0;
	}
}

/*
	Function: DownsampleImageColor
	Description: Downsample the image to fixed sized image (320 x 240) ** for color

	Parameters:(hidden)
		m_pnRImg - input R Image
		m_pnGImg - input G Image
		m_pnBImg - input B Image
	Return:
		m_pnSmallRImg - output down sampled image
		m_pnSmallGImg - output down sampled image
		m_pnSmallBImg - output down sampled image
*/
void dehazing::DownsampleImageColor()
{
	int nX, nY;

	float fRatioY, fRatioX;
	// 다운샘플링 비율 결정
	fRatioX = (float)m_nWid / (float)320;
	fRatioY = (float)m_nHei / (float)240;

	int n_pixel = 0, n_step = 0;
	float fry = 0, frx = 0;
	for (nY = 0; nY < 240; nY++)
	{
		for (nX = 0; nX < 320; nX++)
		{
			// (1) 멤버 변수인 m_pnYImg를 m_pnSmallYImg로 다운샘플링(크기는 320x240)
			n_step = (int)fry * m_nWid + (int)frx;
			m_pnSmallRImg[n_pixel] = m_pnRImg[n_step];
			m_pnSmallGImg[n_pixel] = m_pnGImg[n_step];
			m_pnSmallBImg[n_pixel] = m_pnBImg[n_step];
			n_pixel++;
			frx += fRatioX;
		}
		fry += fRatioY;
		frx = 0;
	}
}


/*
	Function: UpsampleImage
	Description: upsample the fixed sized transmission to original size

	Parameters:(hidden)
		m_pfSmallTransR - input transmission (320 x 240)
	Return:
		m_pfTransmission - output transmission

*/
void dehazing::UpsampleTransmission()
{
	int nX, nY;

	float fRatioY, fRatioX;
	// 업샘플링 비율 결정
	fRatioX = (float)320 / (float)m_nWid;
	fRatioY = (float)240 / (float)m_nHei;

	int n_pixel = 0;
	float fry = 0, frx = 0;
	for (nY = 0; nY < m_nHei; nY++)
	{
		for (nX = 0; nX < m_nWid; nX++)
		{
			// (1) 멤버 변수인 m_pfSmallTransR를 m_pfTransmission로 업샘플링
			m_pfTransmission[n_pixel++] = m_pfSmallTrans[(int)fry * 320 + (int)frx];
			frx += fRatioX;
		}
		fry += fRatioY;
		frx = 0;
	}
}

/*
	Function: MakeExpLUT
	Description: Make a Look Up Table(LUT) for applying previous information.

	Return:
		m_pfExpLUT - output table

*/
void dehazing::MakeExpLUT()
{
	int nIdx;

	for (nIdx = 0; nIdx < 256; nIdx++)
	{
		m_pfExpLUT[nIdx] = expf(-(float)(nIdx * nIdx) / 10.0f);
	}
}

/*
	Function: GuideLUTMaker
	Description: Make a Look Up Table(LUT) for guided filtering

	Return:
		m_pfGuidedLUT - output table

*/
void dehazing::GuideLUTMaker()
{
	int nX, nY;
	const int msz = m_nGBlockSize / 2;	
	int* xd2 = new int[msz];
	int* yd2 = new int[msz];
	int dist = -msz + 1;
	for (nX = 0; nX < msz; nX++)
	{
		xd2[nX] = dist * dist;
		yd2[nX] = xd2[nX];
		dist++;
	}

	int midx = m_nGBlockSize - 1;
	int prowi = 0;
	int nrowi = midx * m_nGBlockSize;
	const float denom = 1 / (2 * m_fGSigma * m_fGSigma);
	int tl, tr, bl, br;
	for (nY = 0; nY < msz; nY++)
	{
		for (nX = 0; nX < msz; nX++)
		{
			tl = prowi + nX;
			tr = prowi + midx - nX;
			bl = nrowi + nX;
			br = nrowi + midx - nX;
			m_pfGuidedLUT[tl] = expf(-(xd2[nX] + yd2[nY]) * denom);
			m_pfGuidedLUT[tr] = m_pfGuidedLUT[tl];
			m_pfGuidedLUT[bl] = m_pfGuidedLUT[tl];
			m_pfGuidedLUT[br] = m_pfGuidedLUT[tl];
		}
		prowi += m_nGBlockSize;
		nrowi -= m_nGBlockSize;
	}

	delete[] xd2;
	delete[] yd2;
}
/*
	Function: GammaLUTMaker
	Description: Make a Look Up Table(LUT) for gamma correction

	parameter:
		fParameter - gamma value.
	Return:
		m_pfGuidedLUT - output table

*/
void dehazing::GammaLUTMaker(float fParameter)
{
	int nIdx;

	for (nIdx = 0; nIdx < 256; nIdx++)
	{
		m_pucGammaLUT[nIdx] = (uchar)(powf((float)nIdx / 255, fParameter) * 255.0f);
	}
}