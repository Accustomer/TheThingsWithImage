/*
	This source file contains dehazing construnctor, destructor, and
	dehazing functions.

	The detailed description of the algorithm is presented
	in "http://mcl.korea.ac.kr/projects/dehazing". See also
	J.-H. Kim, W.-D. Jang, Y. Park, D.-H. Lee, J.-Y. Sim, C.-S. Kim, "Temporally
	coherent real-time video dehazing," in Proc. IEEE ICIP, 2012.

	Last updated: 2013-02-06
	Author: Jin-Hwan, Kim.
 */
#include "dehazing.h"

dehazing::dehazing() {}

/*
	Constructor: dehazing constructor

	Parameters:
		nW - width of input image
		nH - height of input image
		bPrevFlag - boolean for temporal cohenrence of video dehazing
		bPosFlag - boolean for postprocessing.
*/
dehazing::dehazing(int nW, int nH, bool bPrevFlag, bool bPosFlag)
{
	m_nWid = nW;
	m_nHei = nH;

	// Flags for temporal coherence & post processing
	m_bPreviousFlag = bPrevFlag;
	m_bPostFlag = bPosFlag;

	m_fLambda1 = 5.0f;
	m_fLambda2 = 1.0f;

	// Transmission estimation block size
	m_nTBlockSize = 40;

	// Guided filter block size, step size(sampling step), & LookUpTable parameter
	m_nGBlockSize = 40;
	m_nStepSize = 2;
	m_fGSigma = 10.0f;

	// to specify the region of atmospheric light estimation
	m_nTopLeftX = 0;
	m_nTopLeftY = 0;
	m_nBottomRightX = m_nWid;
	m_nBottomRightY = m_nHei;

	m_pfSmallTransP = new float[320 * 240]; // previous trans. (video only)
	m_pfSmallTrans = new float[320 * 240]; // init trans.
	m_pfSmallTransR = new float[320 * 240]; // refined trans.
	m_pnSmallYImg = new int[320 * 240];
	m_pnSmallYImgP = new int[320 * 240];
	m_pfSmallInteg = new float[320 * 240];
	m_pfSmallDenom = new float[320 * 240];
	m_pfSmallY = new float[320 * 240];

	m_pfTransmission = new float[m_nWid * m_nHei];
	m_pfTransmissionR = new float[m_nWid * m_nHei];
	m_pfTransmissionP = new float[m_nWid * m_nHei];
	m_pnYImg = new int[m_nWid * m_nHei];
	m_pnYImgP = new int[m_nWid * m_nHei];
	m_pfInteg = new float[m_nWid * m_nHei];
	m_pfDenom = new float[m_nWid * m_nHei];
	m_pfY = new float[m_nWid * m_nHei];

	m_pfSmallPk_p = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfSmallNormPk = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfPk_p = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfNormPk = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfGuidedLUT = new float[m_nGBlockSize * m_nGBlockSize];
}


/*
	Constructor: dehazing constructor using various options

	Parameters:
		nW - width of input image
		nH - height of input image
		nTBLockSize - block size for transmission estimation
		bPrevFlag - boolean for temporal cohenrence of video dehazing
		bPosFlag - boolean for postprocessing
		fL1 - information loss cost parameter (regulating)
		fL2 - temporal coherence paramter
		nGBlock - guided filter block size
*/
dehazing::dehazing(int nW, int nH, int nTBlockSize, bool bPrevFlag, bool bPosFlag, float fL1, float fL2, int nGBlock)
{
	m_nWid = nW;
	m_nHei = nH;

	// Flags for temporal coherence & post processing
	m_bPreviousFlag = bPrevFlag;
	m_bPostFlag = bPosFlag;

	// parameters for each cost (loss cost, temporal coherence cost)
	m_fLambda1 = fL1;
	m_fLambda2 = fL2;

	// block size for transmission estimation
	m_nTBlockSize = nTBlockSize;

	// Guided filter block size, step size(sampling step), & LookUpTable parameter
	m_nGBlockSize = nGBlock;
	m_nStepSize = 2;
	m_fGSigma = 10.0f;

	// to specify the region of atmospheric light estimation
	m_nTopLeftX = 0;
	m_nTopLeftY = 0;
	m_nBottomRightX = m_nWid;
	m_nBottomRightY = m_nHei;

	m_pfSmallTransP = new float[320 * 240];
	m_pfSmallTrans = new float[320 * 240];
	m_pfSmallTransR = new float[320 * 240];
	m_pnSmallYImg = new int[320 * 240];
	m_pnSmallYImgP = new int[320 * 240];

	m_pnSmallRImg = new int[320 * 240];
	m_pnSmallRImgP = new int[320 * 240];
	m_pnSmallGImg = new int[320 * 240];
	m_pnSmallGImgP = new int[320 * 240];
	m_pnSmallBImg = new int[320 * 240];
	m_pnSmallBImgP = new int[320 * 240];

	m_pfSmallInteg = new float[320 * 240];
	m_pfSmallDenom = new float[320 * 240];
	m_pfSmallY = new float[320 * 240];

	m_pfTransmission = new float[m_nWid * m_nHei];
	m_pfTransmissionR = new float[m_nWid * m_nHei];
	m_pfTransmissionP = new float[m_nWid * m_nHei];
	m_pnYImg = new int[m_nWid * m_nHei];
	m_pnYImgP = new int[m_nWid * m_nHei];

	m_pnRImg = new int[m_nWid * m_nHei];
	m_pnGImg = new int[m_nWid * m_nHei];
	m_pnBImg = new int[m_nWid * m_nHei];

	m_pnRImgP = new int[m_nWid * m_nHei];
	m_pnGImgP = new int[m_nWid * m_nHei];
	m_pnBImgP = new int[m_nWid * m_nHei];

	m_pfInteg = new float[m_nWid * m_nHei];
	m_pfDenom = new float[m_nWid * m_nHei];
	m_pfY = new float[m_nWid * m_nHei];

	m_pfSmallPk_p = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfSmallNormPk = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfPk_p = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfNormPk = new float[m_nGBlockSize * m_nGBlockSize];
	m_pfGuidedLUT = new float[m_nGBlockSize * m_nGBlockSize];
}

dehazing::~dehazing(void)
{
	if (m_pfSmallTransP != NULL)
		delete[] m_pfSmallTransP;
	if (m_pfSmallTrans != NULL)
		delete[] m_pfSmallTrans;
	if (m_pfSmallTransR != NULL)
		delete[] m_pfSmallTransR;
	if (m_pnSmallYImg != NULL)
		delete[] m_pnSmallYImg;
	if (m_pfSmallY != NULL)
		delete[] m_pfSmallY;
	if (m_pnSmallYImgP != NULL)
		delete[] m_pnSmallYImgP;

	if (m_pnSmallRImg != NULL)
		delete[] m_pnSmallRImg;
	if (m_pnSmallRImgP != NULL)
		delete[] m_pnSmallRImgP;
	if (m_pnSmallGImg != NULL)
		delete[] m_pnSmallGImg;
	if (m_pnSmallGImgP != NULL)
		delete[] m_pnSmallGImgP;
	if (m_pnSmallBImg != NULL)
		delete[] m_pnSmallBImg;
	if (m_pnSmallBImgP != NULL)
		delete[] m_pnSmallBImgP;

	if (m_pfSmallInteg != NULL)
		delete[] m_pfSmallInteg;
	if (m_pfSmallDenom != NULL)
		delete[] m_pfSmallDenom;
	if (m_pfSmallNormPk != NULL)
		delete[] m_pfSmallNormPk;
	if (m_pfSmallPk_p != NULL)
		delete[] m_pfSmallPk_p;

	if (m_pnYImg != NULL)
		delete[] m_pnYImg;
	if (m_pnYImgP != NULL)
		delete[] m_pnYImgP;
	if (m_pnRImg != NULL)
		delete[] m_pnRImg;
	if (m_pnGImg != NULL)
		delete[] m_pnGImg;
	if (m_pnBImg != NULL)
		delete[] m_pnBImg;
	if (m_pnRImgP != NULL)
		delete[] m_pnRImgP;
	if (m_pnGImgP != NULL)
		delete[] m_pnGImgP;
	if (m_pnBImgP != NULL)
		delete[] m_pnBImgP;

	if (m_pfTransmission != NULL)
		delete[] m_pfTransmission;
	if (m_pfTransmissionR != NULL)
		delete[] m_pfTransmissionR;
	if (m_pfTransmissionP != NULL)
		delete[] m_pfTransmissionP;

	if (m_pfInteg != NULL)
		delete[] m_pfInteg;
	if (m_pfDenom != NULL)
		delete[] m_pfDenom;
	if (m_pfY != NULL)
		delete[] m_pfY;
	if (m_pfPk_p != NULL)
		delete[] m_pfPk_p;
	if (m_pfNormPk != NULL)
		delete[] m_pfNormPk;
	if (m_pfGuidedLUT != NULL)
		delete[] m_pfGuidedLUT;

	m_pfSmallTransP = NULL;
	m_pfSmallTrans = NULL;
	m_pfSmallTransR = NULL;
	m_pnSmallYImg = NULL;
	m_pnSmallYImgP = NULL;

	m_pnSmallRImg = NULL;
	m_pnSmallRImgP = NULL;
	m_pnSmallGImg = NULL;
	m_pnSmallGImgP = NULL;
	m_pnSmallBImg = NULL;
	m_pnSmallBImgP = NULL;

	m_pfSmallInteg = NULL;
	m_pfSmallDenom = NULL;
	m_pfSmallY = NULL;

	m_pfTransmission = NULL;
	m_pfTransmissionR = NULL;
	m_pfTransmissionP = NULL;
	m_pnYImg = NULL;
	m_pnYImgP = NULL;

	m_pnRImg = NULL;
	m_pnGImg = NULL;
	m_pnBImg = NULL;

	m_pnRImgP = NULL;
	m_pnGImgP = NULL;
	m_pnBImgP = NULL;

	m_pfInteg = NULL;
	m_pfDenom = NULL;
	m_pfY = NULL;

	m_pfSmallPk_p = NULL;
	m_pfSmallNormPk = NULL;
	m_pfPk_p = NULL;
	m_pfNormPk = NULL;
	m_pfGuidedLUT = NULL;
}

/*
	Function: AirlightEstimation
	Description: estimate the atmospheric light value in a hazy image.
				 it divides the hazy image into 4 sub-block and selects the optimal block,
				 which has minimum std-dev and maximum average value.
				 *Repeat* the dividing process until the size of sub-block is smaller than
				 pre-specified threshold value. Then, We select the most similar value to
				 the pure white.
				 IT IS A RECURSIVE FUNCTION.
	Parameter:
		imInput - input image (cv iplimage)
	Return:
		m_anAirlight: estimated atmospheric light value
 */
void dehazing::AirlightEstimation(cv::Mat& imInput)
{
	int nWid = imInput.cols;
	int nHei = imInput.rows;
	// compare to threshold(200) --> bigger than threshold, divide the block
	if (nHei * nWid > 200)
	{
		int nMaxIndex;
		double dpScore[3];
		cv::Scalar dpMean;
		cv::Scalar dpStds;
		float afMean = 0;
		float afScore = 0;
		float nMaxScore = 0;

		int w2 = nWid / 2;
		int h2 = nHei / 2;
		int rw = nWid - w2;
		int rh = nHei - h2;

		// 4 sub-block
		cv::Mat iplUpperLeft, iplUpperRight, iplLowerLeft, iplLowerRight;
		cv::Mat ipls[3];

		// divide 
		imInput.rowRange(0, h2).colRange(0, w2).copyTo(iplUpperLeft);
		imInput.rowRange(0, h2).colRange(rw, nWid).copyTo(iplUpperRight);
		imInput.rowRange(rh, nHei).colRange(0, w2).copyTo(iplLowerLeft);
		imInput.rowRange(rh, nHei).colRange(rw, nWid).copyTo(iplLowerRight);

		// compute the mean and std-dev in the sub-block
		// upper left sub-block
		cv::split(iplUpperLeft, ipls);
		for (int i = 0; i < 3; i++)
		{
			cv::meanStdDev(ipls[i], dpMean, dpStds);
			
			// dpScore: mean - std-dev
			dpScore[i] = dpMean[0] - dpStds[0];
		}
		afScore = (float)(dpScore[0] + dpScore[1] + dpScore[2]);
		nMaxScore = afScore;
		nMaxIndex = 0;

		// upper right sub-block
		cv::split(iplUpperRight, ipls);
		for (int i = 0; i < 3; i++)
		{
			cv::meanStdDev(ipls[i], dpMean, dpStds);

			// dpScore: mean - std-dev
			dpScore[i] = dpMean[0] - dpStds[0];
		}
		afScore = (float)(dpScore[0] + dpScore[1] + dpScore[2]);
		if (afScore > nMaxScore)
		{
			nMaxScore = afScore;
			nMaxIndex = 1;
		}

		// lower left sub-block
		cv::split(iplLowerLeft, ipls);
		for (int i = 0; i < 3; i++)
		{
			cv::meanStdDev(ipls[i], dpMean, dpStds);

			// dpScore: mean - std-dev
			dpScore[i] = dpMean[0] - dpStds[0];
		}
		afScore = (float)(dpScore[0] + dpScore[1] + dpScore[2]);
		if (afScore > nMaxScore)
		{
			nMaxScore = afScore;
			nMaxIndex = 2;
		}

		// lower right sub-block
		cv::split(iplLowerRight, ipls);
		for (int i = 0; i < 3; i++)
		{
			cv::meanStdDev(ipls[i], dpMean, dpStds);

			// dpScore: mean - std-dev
			dpScore[i] = dpMean[0] - dpStds[0];
		}
		afScore = (float)(dpScore[0] + dpScore[1] + dpScore[2]);
		if (afScore > nMaxScore)
		{
			nMaxScore = afScore;
			nMaxIndex = 3;
		}

		// select the sub-block, which has maximum score
		switch (nMaxIndex)
		{
		case 0:
			AirlightEstimation(iplUpperLeft); break;
		case 1:
			AirlightEstimation(iplUpperRight); break;
		case 2:
			AirlightEstimation(iplLowerLeft); break;
		case 3:
			AirlightEstimation(iplLowerRight); break;
		}
	}
	else
	{
		int nMinDistance = 65536;
		int nDistance;
		int nX, nY;
		uchar invr, invg, invb;

		// select the atmospheric light value in the sub-block
		for (nY = 0; nY < nHei; nY++)
		{
			uchar* imptr = imInput.ptr<uchar>(nY);
			for (nX = 0; nX < nWid; nX++)
			{
				// 255-r, 255-g, 255-b
				invb = 255 - imptr[0];
				invg = 255 - imptr[1];
				invr = 255 - imptr[2];
				nDistance = (int)sqrtf(float(invb * invb + invg * invg + invr * invr));
				if (nMinDistance > nDistance)
				{
					nMinDistance = nDistance;
					m_anAirlight[0] = imptr[0];
					m_anAirlight[1] = imptr[1];
					m_anAirlight[2] = imptr[3];
				}
				imptr += 3;
			}
		}
	}
}





/*
	Function: RestoreImage
	Description: Dehazed the image using estimated transmission and atmospheric light.
	Parameter:
		imInput - Input hazy image.
	Return:
		imOutput - Dehazed image.
 */
void dehazing::RestoreImage(cv::Mat& imInput, cv::Mat& imOutput)
{
	int nX, nY;
	float fA_R, fA_G, fA_B;

	fA_B = (float)m_anAirlight[0];
	fA_G = (float)m_anAirlight[1];
	fA_R = (float)m_anAirlight[2];

	// post processing flag
	if (m_bPostFlag == true)
	{
		PostProcessing(imInput, imOutput);
	}
	else
	{
		//const int nStep = imInput.cols * 3;
//#pragma omp parallel for num_threads(8)
		// (2) I' = (I - Airlight)/Transmission + Airlight
		int n_pixel = 0;
		for (nY = 0; nY < m_nHei; nY++)
		{
			uchar* inptr = imInput.ptr<uchar>(nY);
			uchar* outptr = imOutput.ptr<uchar>(nY);
			for (nX = 0; nX < m_nWid; nX++)
			{
				// (3) Gamma correction using LUT
				//imOutput.data[nY * nStep + nX * 3] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput.data[nY * nStep + nX * 3 + 0]) - fA_B) / CLIP_Z(m_pfTransmissionR[nY * m_nWid + nX]) + fA_B))];
				//imOutput.data[nY * nStep + nX * 3 + 1] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput.data[nY * nStep + nX * 3 + 1]) - fA_G) / CLIP_Z(m_pfTransmissionR[nY * m_nWid + nX]) + fA_G))];
				//imOutput.data[nY * nStep + nX * 3 + 2] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput.data[nY * nStep + nX * 3 + 2]) - fA_R) / CLIP_Z(m_pfTransmissionR[nY * m_nWid + nX]) + fA_R))];
				outptr[0] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)inptr[0]) - fA_B) / CLIP_Z(m_pfTransmissionR[n_pixel]) + fA_B))];
				outptr[1] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)inptr[1]) - fA_G) / CLIP_Z(m_pfTransmissionR[n_pixel]) + fA_G))];
				outptr[2] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)inptr[2]) - fA_R) / CLIP_Z(m_pfTransmissionR[n_pixel]) + fA_R))];
				inptr += 3;
				outptr += 3;
				n_pixel++;
			}
		}
	}
}

/*
	Function: PostProcessing
	Description: deblocking for blocking artifacts of mpeg video sequence.
	Parameter:
		imInput - Input hazy frame.
	Return:
		imOutput - Dehazed frame.
 */
void dehazing::PostProcessing(cv::Mat& imInput, cv::Mat& imOutput)
{
	const int nNumStep = 10;
	const int nDisPos = 20;
	const int nStep = imInput.cols * 3;

	float nAD0, nAD1, nAD2;
	int nS, nX, nY;
	float fA_R, fA_G, fA_B;

	fA_B = (float)m_anAirlight[0];
	fA_G = (float)m_anAirlight[1];
	fA_R = (float)m_anAirlight[2];

//#pragma omp parallel for private(nAD0, nAD1, nAD2, nS)
	int n_pixel = 0;
	const int dispos0 = 3 * nDisPos;
	const int dispos1 = dispos0 + 3;
	const int numstep0 = dispos1 + 3 * nNumStep;
	const int numstep1 = dispos1 - 3 * nNumStep;
	for (nY = 0; nY < m_nHei; nY++)
	{
		uchar* inptr = imInput.ptr<uchar>(nY);
		uchar* outptr = imOutput.ptr<uchar>(nY);
		for (nX = 0; nX < m_nWid; nX++)
		{
			// (1)  I' = (I - Airlight)/Transmission + Airlight
			//imOutput.data[nY * nStep + nX * 3] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput.data[nY * nStep + nX * 3 + 0]) - fA_B) / CLIP_Z(m_pfTransmissionR[nY * m_nWid + nX]) + fA_B))];
			//imOutput.data[nY * nStep + nX * 3 + 1] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput.data[nY * nStep + nX * 3 + 1]) - fA_G) / CLIP_Z(m_pfTransmissionR[nY * m_nWid + nX]) + fA_G))];
			//imOutput.data[nY * nStep + nX * 3 + 2] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)imInput.data[nY * nStep + nX * 3 + 2]) - fA_R) / CLIP_Z(m_pfTransmissionR[nY * m_nWid + nX]) + fA_R))];
			outptr[0] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)inptr[0]) - fA_B) / CLIP_Z(m_pfTransmissionR[n_pixel]) + fA_B))];
			outptr[1] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)inptr[1]) - fA_G) / CLIP_Z(m_pfTransmissionR[n_pixel]) + fA_G))];
			outptr[2] = (uchar)m_pucGammaLUT[(uchar)CLIP((((float)((uchar)inptr[2]) - fA_R) / CLIP_Z(m_pfTransmissionR[n_pixel]) + fA_R))];

			// if transmission is less than 0.4, we apply post processing because more dehazed block yields more artifacts
			if (nX > nDisPos + nNumStep && m_pfTransmissionR[n_pixel - nDisPos] < 0.4)
			{
				//nAD0 = (float)((int)((uchar)imOutput.data[nY * nStep + (nX - nDisPos) * 3]) - (int)((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1) * 3]));
				//nAD1 = (float)((int)((uchar)imOutput.data[nY * nStep + (nX - nDisPos) * 3 + 1]) - (int)((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1) * 3 + 1]));
				//nAD2 = (float)((int)((uchar)imOutput.data[nY * nStep + (nX - nDisPos) * 3 + 2]) - (int)((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1) * 3 + 2]));

				uchar* dpout = outptr - dispos1;
				nAD0 = (float)((int)((uchar)dpout[3]) - (int)((uchar)dpout[0]));
				nAD1 = (float)((int)((uchar)dpout[4]) - (int)((uchar)dpout[1]));
				nAD2 = (float)((int)((uchar)dpout[5]) - (int)((uchar)dpout[2]));

				//if (__max(__max(abs(nAD0), abs(nAD1)), abs(nAD2)) < 20
				//	&& abs((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1) * 3 + 0] - (uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 - nNumStep) * 3 + 0])
				//	+ abs((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1) * 3 + 1] - (uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 - nNumStep) * 3 + 1])
				//	+ abs((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1) * 3 + 2] - (uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 - nNumStep) * 3 + 2])
				//	+ abs((uchar)imOutput.data[nY * nStep + (nX - nDisPos) * 3 + 0] - (uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 + nNumStep) * 3 + 0])
				//	+ abs((uchar)imOutput.data[nY * nStep + (nX - nDisPos) * 3 + 1] - (uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 + nNumStep) * 3 + 1])
				//	+ abs((uchar)imOutput.data[nY * nStep + (nX - nDisPos) * 3 + 2] - (uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 + nNumStep) * 3 + 2]) < 30)

				uchar* nsout0 = outptr - numstep0;
				uchar* nsout1 = outptr - numstep1;
				if (__max(__max(abs(nAD0), abs(nAD1)), abs(nAD2)) < 20
					&& abs((uchar)dpout[0] - (uchar)nsout0[0])
					+ abs((uchar)dpout[1] - (uchar)nsout0[1])
					+ abs((uchar)dpout[2] - (uchar)nsout0[2])
					+ abs((uchar)dpout[3] - (uchar)nsout1[0])
					+ abs((uchar)dpout[4] - (uchar)nsout1[1])
					+ abs((uchar)dpout[5] - (uchar)nsout1[2]) < 30)
				{
					
					for (nS = 1; nS < nNumStep + 1; nS++)
					{
						//imOutput.data[nY * nStep + (nX - nDisPos - 1 + nS - nNumStep) * 3 + 0] = (uchar)CLIP((float)((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 + nS - nNumStep) * 3 + 0]) + (float)nS * nAD0 / (float)nNumStep);
						//imOutput.data[nY * nStep + (nX - nDisPos - 1 + nS - nNumStep) * 3 + 1] = (uchar)CLIP((float)((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 + nS - nNumStep) * 3 + 1]) + (float)nS * nAD1 / (float)nNumStep);
						//imOutput.data[nY * nStep + (nX - nDisPos - 1 + nS - nNumStep) * 3 + 2] = (uchar)CLIP((float)((uchar)imOutput.data[nY * nStep + (nX - nDisPos - 1 + nS - nNumStep) * 3 + 2]) + (float)nS * nAD2 / (float)nNumStep);

						nsout0 += 3;
						nsout0[0] = (uchar)CLIP((float)((uchar)nsout0[0]) + (float)nS * nAD0 / (float)nNumStep);
						nsout0[1] = (uchar)CLIP((float)((uchar)nsout0[1]) + (float)nS * nAD1 / (float)nNumStep);
						nsout0[2] = (uchar)CLIP((float)((uchar)nsout0[2]) + (float)nS * nAD2 / (float)nNumStep);
					}
				}
			}

			inptr += 3;
			outptr += 3;
			n_pixel++;
		}
	}
}


/*
	Function: HazeRemoval
	Description: haze removal process

	Parameter:
		imInput - input image
		nFrame - frame number
	Return:
		imOutput - output image
 */
void dehazing::HazeRemoval(cv::Mat& imInput, cv::Mat& imOutput, int nFrame)
{
	if (nFrame == 0)
	{
		cv::Mat imAir;
		// initializing
		MakeExpLUT();
		GuideLUTMaker();
		GammaLUTMaker(0.7f);

		// specify the ROI region of atmospheric light estimation(optional)
		imInput.rowRange(m_nTopLeftY, m_nBottomRightY).colRange(m_nTopLeftX, m_nBottomRightX).copyTo(imAir);
		//cvSetImageROI(imInput, cvRect(m_nTopLeftX, m_nTopLeftY, m_nBottomRightX - m_nTopLeftX, m_nBottomRightY - m_nTopLeftY));
		//imAir = cvCreateImage(cvSize(m_nBottomRightX - m_nTopLeftX, m_nBottomRightY - m_nTopLeftY), IPL_DEPTH_8U, 3);
		//cvCopyImage(imInput, imAir);

		AirlightEstimation(imAir);

		// Y value of atmosperic light
		m_nAirlight = (((int)(uchar)m_anAirlight[0] * 25 + (int)(uchar)m_anAirlight[1] * 129 + (int)(uchar)m_anAirlight[2] * 66 + 128) >> 8) + 16;

		//cvReleaseImage(&imAir);
		//cvResetImageROI(imInput);
	}

	IplImageToInt(imInput);
	// down sampling to fast estimation
	DownsampleImage();

	// trnasmission estimation
	TransmissionEstimation(m_pnSmallYImg, m_pfSmallTrans, m_pnSmallYImgP, m_pfSmallTransP, nFrame, 320, 240);

	// store a data for temporal coherent processing
	memcpy(m_pfSmallTransP, m_pfSmallTrans, 320 * 240);
	memcpy(m_pnSmallYImgP, m_pnSmallYImg, 320 * 240);

	UpsampleTransmission();

	/*
	IplImage *test = cvCreateImage(cvSize(320, 240),IPL_DEPTH_8U, 1);
	for(int nK = 0; nK < 320*240; nK++)
		test->imageData[nK] = (uchar)(m_pnSmallYImg[nK]);
	cvNamedWindow("tests");
	cvShowImage("tests", test);
	cvWaitKey(-1);
	*/
	FastGuidedFilter();

	// (9) 영상 복원 수행
	RestoreImage(imInput, imOutput);
}

/*
	Function: ImageHazeRemoval
	Description: haze removal process for a single image

	Parameter:
		imInput - input image
	Return:
		imOutput - output image
 */
void dehazing::ImageHazeRemoval(cv::Mat& imInput, cv::Mat& imOutput)
{
	cv::Mat imAir;
	cv::Mat imSmallInput;
	// look up table creation
	MakeExpLUT();
	GuideLUTMaker();
	GammaLUTMaker(0.7f);

	// specify the ROI region of atmospheric light estimation(optional)
	//cvSetImageROI(imInput, cvRect(m_nTopLeftX, m_nTopLeftY, m_nBottomRightX - m_nTopLeftX, m_nBottomRightY - m_nTopLeftY));

	//imAir = cvCreateImage(cvSize(m_nBottomRightX - m_nTopLeftX, m_nBottomRightY - m_nTopLeftY), IPL_DEPTH_8U, 3);
	//imSmallInput = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
	//cvCopyImage(imInput, imAir);
	imSmallInput = cv::Mat(cv::Size(320, 240), CV_8UC3);
	imInput.rowRange(m_nTopLeftY, m_nBottomRightY).colRange(m_nTopLeftX, m_nBottomRightX).copyTo(imAir);

	AirlightEstimation(imAir);

	//cvReleaseImage(&imAir);
	//cvResetImageROI(imInput);
	imAir.release();

	// iplimage to int
	IplImageToIntColor(imInput);

	TransmissionEstimationColor(m_pnRImg, m_pnGImg, m_pnBImg, m_pfTransmission, m_pnRImg, m_pnGImg, m_pnBImg, m_pfTransmission, 0, m_nWid, m_nHei);

	GuidedFilter(m_nWid, m_nHei, 0.001);
	//GuidedFilterShiftableWindow(0.001);
	/*
	IplImage *test = cvCreateImage(cvSize(m_nWid, m_nHei),IPL_DEPTH_8U, 1);
	for(int nK = 0; nK < m_nWid*m_nHei; nK++)
		test->imageData[nK] = (uchar)(m_pfTransmissionR[nK]*255);
	cvNamedWindow("tests");
	cvShowImage("tests", test);
	cvWaitKey(-1);
	*/
	// Restore image
	RestoreImage(imInput, imOutput);
	//cvReleaseImage(&imSmallInput);
	imSmallInput.release();
}

/*
	Function:GetAirlight
	Return: air light value
 */
int* dehazing::GetAirlight()
{
	// (1) airlight값 주소 리턴
	return m_anAirlight;
}

/*
	Function:GetYImg
	Return: get y image array
 */
int* dehazing::GetYImg()
{
	// (1) Y영상 주소 리턴
	return m_pnYImg;
}

/*
	Function:GetTransmission
	Return: get refined transmission array
 */
float* dehazing::GetTransmission()
{
	// (1) 전달량 주소 리턴
	return m_pfTransmissionR;
}

/*
	Function:LambdaSetting
		chnage labmda values
	Parameter:
		fLambdaLoss - new weight coefficient for loss cost
		fLambdaTemp - new weight coefficient for temp cost
 */
void dehazing::LambdaSetting(float fLambdaLoss, float fLambdaTemp)
{
	// (1) 람다값을 재정의 할 때 사용하는 함수
	m_fLambda1 = fLambdaLoss;
	if (fLambdaTemp > 0)
		m_fLambda2 = fLambdaTemp;
	else
		m_bPreviousFlag = false;
}

/*
	Function:PreviousFlag
		change boolean value
	Parameter
		bPrevFlag - flag
 */
void dehazing::PreviousFlag(bool bPrevFlag)
{
	// (1) 이전 데이터 사용 유무 결정
	m_bPreviousFlag = bPrevFlag;
}

/*
	Function:TransBlockSize
		change the block size of transmission estimation
	Parameter:
		nBlockSize - new block size
 */
void dehazing::TransBlockSize(int nBlockSize)
{
	// (1) 전달량 계산의 블록 크기 결정
	m_nTBlockSize = nBlockSize;
}

/*
	Function:FilterBlockSize
		change the block size of guided filter
	Parameter:
		nBlockSize - new block size
 */
void dehazing::FilterBlockSize(int nBlockSize)
{
	// (1) 전달량 계산의 블록 크기 결정
	m_nGBlockSize = nBlockSize;
}

/*
	Function:SetFilterStepSize
		change the step size of guided filter
	Parameter:
		nStepSize - new step size
 */
void dehazing::SetFilterStepSize(int nStepSize)
{
	// (1) guided filter의 step size 수정
	m_nStepSize = nStepSize;
}

/*
	Function: AirlightSearchRange
	Description: Specify searching range (block shape) by user
	Parameter:
		pointTopLeft - the top left point of searching block
		pointBottomRight - the bottom right point of searching block.
	Return:
		m_nTopLeftX - integer x point
		m_nTopLeftY - integer y point
		m_nBottomRightX - integer x point
		m_nBottomRightY - integer y point.
 */
void dehazing::AirlightSerachRange(cv::Point pointTopLeft, cv::Point pointBottomRight)
{
	m_nTopLeftX = pointTopLeft.x;
	m_nTopLeftY = pointTopLeft.y;
	m_nBottomRightX = pointBottomRight.x;
	m_nBottomRightY = pointBottomRight.y;
}

/*
	Function: FilterSigma
	Description: change the gaussian sigma value
	Parameter:
		nSigma
 */
void dehazing::FilterSigma(float nSigma)
{
	m_fGSigma = nSigma;
}

/*
	Function: Decision
	Description: Decision function for re-estimation of atmospheric light
		in this file, we just implement the decision function and don't
		apply the decision algorithm in the dehazing function.
	Parameter:
		imSrc1 - first frame
		imSrc2 - second frame
		nThreshold - threshold value
	Return:
		boolean value
 */
bool dehazing::Decision(cv::Mat& imSrc1, cv::Mat& imSrc2, int nThreshold)
{
	int nX, nY;
	int nStep;

	int nMAD;

	nMAD = 0;

	//nStep = imSrc1->widthStep;
	if (imSrc2.size() != imSrc1.size() || imSrc1.channels() != imSrc2.channels())
	{
		//std::cout << "Size of src1 and src2 is not the same" << endl;
		printf("Size of src1 and src2 is not the same\n");
		exit(1);
	}

	for (nY = 0; nY < m_nHei; nY++)
	{
		uchar* ptr1 = imSrc1.ptr<uchar>(nY);
		uchar* ptr2 = imSrc2.ptr<uchar>(nY);
		for (nX = 0; nX < m_nWid; nX++)
		{
			nMAD += abs((int)ptr1[0] - (int)ptr2[0]);
			nMAD += abs((int)ptr1[1] - (int)ptr2[1]);
			nMAD += abs((int)ptr1[2] - (int)ptr2[2]);
			ptr1 += 3;
			ptr2 += 3;
		}
	}

	nMAD /= (m_nWid * m_nHei);
	if (nMAD > nThreshold)
		return true;
	else
		return false;
}
