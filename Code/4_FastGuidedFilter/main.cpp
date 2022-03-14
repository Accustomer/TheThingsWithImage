#include <immintrin.h>
#include <stdio.h>
#include <thread>
#include <time.h>
#include "GuidedFilter.hpp"
#include <opencv2/highgui.hpp>

#define MATRIX_SIZE 1920
#define NUM_THREAD 4

float matA[MATRIX_SIZE][MATRIX_SIZE];
float matB[MATRIX_SIZE][MATRIX_SIZE];
float matC[MATRIX_SIZE][MATRIX_SIZE];
int step = 0;


void multiplicationAVX(int n)
{
	__m256 vecA, vecB, vecC;

	for (int i = n * MATRIX_SIZE / NUM_THREAD; i < (n + 1) * MATRIX_SIZE / NUM_THREAD; i++) 
	{
		for (int j = 0; j < MATRIX_SIZE; j++) 
		{
			matC[i][j] = 0.0f;
		}
		for (int j = 0; j < MATRIX_SIZE; j++) 
		{
			vecA = _mm256_set1_ps(matA[i][j]);
			for (int k = 0; k < MATRIX_SIZE; k += 8)
			{
				vecB = _mm256_loadu_ps(&matB[j][k]);
				vecC = _mm256_loadu_ps(&matC[i][k]);
				vecC = _mm256_fmadd_ps(vecA, vecB, vecC);
				_mm256_storeu_ps(&matC[i][k], vecC);
			}
		}
	}
}


void multiplicationNormal(int n)
{
	for (int i = n * MATRIX_SIZE / NUM_THREAD; i < (n + 1) * MATRIX_SIZE / NUM_THREAD; i++) 
	{
		for (int j = 0; j < MATRIX_SIZE; j++) 
		{
			for (int k = 0; k < MATRIX_SIZE; k++) 
			{
				matC[i][j] += matA[i][k] * matB[k][j];
			}
		}
	}
}


void createMatrix() {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			matA[i][j] = i + j * 2;
			matB[i][j] = i * 2 + j;
			matC[i][j] = 0.0f;
		}
	}
}


void printMatrix() {
	if (MATRIX_SIZE <= 16) {
		printf("Matriz A");
		for (int i = 0; i < MATRIX_SIZE; i++) {
			printf("\n");
			for (int j = 0; j < MATRIX_SIZE; j++) {
				printf("%f ", matA[i][j]);
			}
		}
		printf("\n\n");
		printf("Matriz B");
		for (int i = 0; i < MATRIX_SIZE; i++) {
			printf("\n");
			for (int j = 0; j < MATRIX_SIZE; j++) {
				printf("%f ", matB[i][j]);
			}
		}
		printf("\n\n");
		printf("Multiplying matrix A with B");
		for (int i = 0; i < MATRIX_SIZE; i++) {
			printf("\n");
			for (int j = 0; j < MATRIX_SIZE; j++) {
				printf("%f ", matC[i][j]);
			}
		}
	}
}


void testAVX()
{
	std::thread threads[NUM_THREAD];
	clock_t start, end;

	createMatrix();

	start = clock();
	for (int i = 0; i < NUM_THREAD; i++) {
		threads[i] = std::thread(multiplicationAVX, i + 1);
		//threads[i] = std::thread(multiplicationNormal, i + 1);
	}
	for (int i = 0; i < NUM_THREAD; i++) {
		threads[i].join();
	}

	end = clock();

	printMatrix();
	printf("\n\n使用的线程数 -> %d\n", NUM_THREAD);
	printf("\n矩阵大小 -> %d\n", MATRIX_SIZE);
	printf("\n程序运行时间（毫秒） -> %f\n\n", (float)(end - start) * 1000 / CLOCKS_PER_SEC);


	cv::UMat m1 = cv::UMat(MATRIX_SIZE, MATRIX_SIZE, CV_32FC1);
	cv::UMat m2 = cv::UMat(MATRIX_SIZE, MATRIX_SIZE, CV_32FC1);
	cv::UMat m;

	cv::RNG rng;
	rng.fill(m1, rng.UNIFORM, 0, 255, true);
	rng.fill(m2, rng.UNIFORM, 0, 255, true);

	start = clock();

	cv::gemm(m1, m2, 1, NULL, 0, m);

	//m = m1.dot(m2);
	end = clock();

	printf("\n程序运行时间（毫秒） -> %f\n\n", (float)(end - start) * 1000 / CLOCKS_PER_SEC);
}


void testGF()
{
	cv::Mat im1 = cv::imread("E:/Projects/2021/TheThingsWithImage/Data/adobe_image_4.jpg");
	cv::Mat im2 = cv::imread("E:/Projects/2021/TheThingsWithImage/Data/adobe_gt_4.jpg");
	//cv::Mat im2 = im1.clone();

	cv::Mat im3;
	guidedFilter(im2, im1, im3, 9, 20.f);
	//cv::blur(im1, im3, cv::Size(5, 5));

	cv::imwrite("E:/Projects/2021/TheThingsWithImage/Data/adobeguided.png", im3);
}


int main() 
{
	//testAVX();
	testGF();

	return 0;
}