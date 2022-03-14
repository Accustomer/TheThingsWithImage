# 构造矩阵

opencv使用Mat对象来存储图像数据，并通过其对图像进行运算处理，学会Mat对象的操作对图像处理很有帮助。所以先从如何创建一个矩阵说起吧~



## 数据类型

opencv的图像数据类型主要无符号字节型、整型和浮点型三种，三种大类型下又有一些细分，这里总结如下：

| 数据类型 \ 通道数 | 0    | 1    | 2    | 3    | 4    |
| :---------------: | ---- | ---- | ---- | ---- | ---- |
|       CV_8U       | 0    | 0    | 8    | 16   | 24   |
|       CV_8S       | 1    | 1    | 9    | 17   | 25   |
|      CV_16U       | 2    | 2    | 10   | 18   | 26   |
|      CV_16S       | 3    | 3    | 11   | 19   | 27   |
|      CV_32S       | 4    | 4    | 12   | 20   | 28   |
|      CV_32F       | 5    | 5    | 13   | 21   | 29   |
|      CV_64F       | 6    | 6    | 14   | 22   | 30   |
|      CV_16F       | 7    | 7    | 15   | 23   | 31   |

当然，当通道数大于4时，可以通过括号方式进行指定，如：CV_8UC(5) ：32。

注：

1. 字节数：1个字节等于8位，即1 bytes = 8 bits，例如：CV_8U或CV_8S的矩阵中每个元素占据一个字节，CV_32S和CV_32F的矩阵中每个元素占据4个字节。
2. 数据范围：无符号型的数据范围是 [0, 2^bits-1)，有符号型的数据范围是[-2^(bits-1), 2^(bits-1)-1)。例如：CV_8U矩阵的数据范围是[0, 256)，CV_8S的是[-128, 128）。



## 构造Mat

构造Mat的方式多种多样，但大致可分为三类：非复制构造、复制构造、静态构造（名字全是我自己瞎取的，无参考依据 -_-）。



**非复制构造**

首先来总结一下非复制构造的方式：

```c++
void noCopyCreate()
{
	// 默认构造
	cv::Mat m0;

	// 指定尺寸和类型
	const int m_rows = 5; 
	const int m_cols = 5;
	const int m_type = CV_8UC1;
	cv::Mat m1(m_rows, m_cols, m_type);

	// 指定尺寸、类型和初始值
	const cv::Scalar m_init(3);
	cv::Mat m2(m_rows, m_cols, m_type, m_init);

	// 通过cv::Size指定尺寸
	const cv::Size m_sz(m_rows, m_cols);
	cv::Mat m3(m_sz, m_type);
	cv::Mat m4(m_sz, m_type, m_init);

	// 构造多通道矩阵
	const int m_type2 = CV_8UC3;
	cv::Mat m5(m_sz, m_type2);

	// 构造多维数组
	int m_dims = 5;
	const int mul_size[5] = { 5, 4, 3, 2, 1 };
	cv::Mat m6(m_dims, mul_size, m_type);

	std::cout << "m0.empty(): " << m0.empty() << "\tm0.size(): " << m0.size() << std::endl;
	std::cout << "\nm1.empty(): " << m1.empty() << "\tm1.size(): " << m1.size() << std::endl;
	std::cout << "m1: \n" << m1 << std::endl;

	std::cout << "\nm2.empty(): " << m2.empty() << "\tm2.size(): " << m2.size() << std::endl;
	std::cout << "m2: \n" << m2 << std::endl;

	std::cout << "\nm3.empty(): " << m3.empty() << "\tm3.size(): " << m3.size() << std::endl;
	std::cout << "m3: \n" << m3 << std::endl;

	std::cout << "\nm4.empty(): " << m4.empty() << "\tm4.size(): " << m4.size() << std::endl;
	std::cout << "m4: \n" << m4 << std::endl;

	std::cout << "\nm5.empty(): " << m5.empty() << "\tm5.size(): " << m5.size() << std::endl;
	std::cout << "m5: \n" << m5 << std::endl;

	std::cout << "\nDimension of m0 ~ m6: " << m0.dims << ", " << m1.dims << ", " << m2.dims << ", " <<
		m3.dims << ", " << m4.dims << ", " << m5.dims << ", " << m6.dims << std::endl;
    std::cout << "Size of m6: " << std::endl;
	for (int i = 0; i < 5; i++)
	{
		std::cout << "dimension " << i << ": " << m6.size[i] << std::endl;
	}
}
```

结果：

```C++
m0.empty(): 1   m0.size(): [0 x 0]

m1.empty(): 0   m1.size(): [5 x 5]
m1:
[205, 205, 205, 205, 205;
 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205]

m2.empty(): 0   m2.size(): [5 x 5]
m2:
[  3,   3,   3,   3,   3;
   3,   3,   3,   3,   3;
   3,   3,   3,   3,   3;
   3,   3,   3,   3,   3;
   3,   3,   3,   3,   3]

m3.empty(): 0   m3.size(): [5 x 5]
m3:
[205, 205, 205, 205, 205;
 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205]

m4.empty(): 0   m4.size(): [5 x 5]
m4:
[  3,   3,   3,   3,   3;
   3,   3,   3,   3,   3;
   3,   3,   3,   3,   3;
   3,   3,   3,   3,   3;
   3,   3,   3,   3,   3]

m5.empty(): 0   m5.size(): [5 x 5]
m5:
[205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205;
 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205]

Dimension of m0 ~ m6: 0, 2, 2, 2, 2, 2, 5
Size of m6:
dimension 0: 5
dimension 1: 4
dimension 2: 3
dimension 3: 2
dimension 4: 1
```

有几点需要注意：

1. 未指定尺寸时，构造的矩阵是空的；
2. 指定尺寸未指定初始值时，默认设为值205（不知道是有什么讲究，设为0不好吗？）
3. 尺寸只指宽和高，不包括通道，所以宽和高相同的三通道矩阵和单通道矩阵尺寸都是一样的
4. 三通道矩阵只有2维，不是3维，要构造更高维的矩阵可以通过用数组指定每个维度的尺寸进行构造。



**复制构造**

再来总结一下复制构造的相关方式：
```C++
void copyCreate()
{
	// 从数组中复制，也可看作是通过数组赋初值
	uchar p[2][3] = { {1, 2, 3}, {4, 5, 6} };
	cv::Mat m0(2, 3, CV_8UC1, p);

	// 从已有的矩阵中复制
	cv::Mat m1(m0);
	cv::Mat m2(m0, cv::Range(0, 2), cv::Range(1, 3));
	cv::Mat m3(m0, cv::Rect(0, 0, 2, 2));

	// 从其他数据类型复制
	cv::Vec<int, 3> vec0 = { 3, 4, 5 };
	cv::Mat m4(vec0, true);

	cv::Matx<int, 3, 3> mx = { 1, 2, 3, 4, 45, 5, 6, 7, 8 };
	cv::Mat m5(mx, true);

	std::vector<int> vec1 = { 5, 6, 7 };
	cv::Mat m6(vec1, true);

	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
	std::cout << "m3:\n" << m3 << std::endl;
	std::cout << "m4:\n" << m4 << std::endl;
	std::cout << "m5:\n" << m5 << std::endl;
	std::cout << "m6:\n" << m6 << std::endl;
}
```

结果：

```C++
m0:
[  1,   2,   3;
   4,   5,   6]
m1:
[  1,   2,   3;
   4,   5,   6]
m2:
[  2,   3;
   5,   6]
m3:
[  1,   2;
   4,   5]
m4:
[3;
 4;
 5]
m5:
[1, 2, 3;
 4, 45, 5;
 6, 7, 8]
m6:
[5;
 6;
 7]
```

我们可以通过数组、向量、矩阵等多种方式进行复制构造。其中需要注意的一点是：通过矩阵中某块区域进行复制构造时，构造的矩阵数据并不是连续的，如示例中的m2和m3，对不连续的矩阵进行元素操作时常常会出现错误。



**静态构造**

静态构造就是提供一些特殊的函数来创建一些常用的数组，如zeros()，ones()，eye()分别创建全0矩阵，全1矩阵和对角矩阵。创建示例如下：

```C++
void staticCreate()
{
	cv::Mat m0 = cv::Mat::zeros(3, 3, CV_8UC1);
	cv::Mat m1 = cv::Mat::ones(3, 3, CV_8UC2);
	cv::Mat m2 = cv::Mat::eye(3, 3, CV_8UC3);

	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;

}
```

输出：

```C++
m0:
[  0,   0,   0;
   0,   0,   0;
   0,   0,   0]
m1:
[  1,   0,   1,   0,   1,   0;
   1,   0,   1,   0,   1,   0;
   1,   0,   1,   0,   1,   0]
m2:
[  1,   0,   0,   0,   0,   0,   0,   0,   0;
   0,   0,   0,   1,   0,   0,   0,   0,   0;
   0,   0,   0,   0,   0,   0,   1,   0,   0]
```

注意：当通道数为1时，ones和eys创建的矩阵确实为全1矩阵和对角矩阵，但是当通道数大于1时，其余的通道实际是以0填充的！



**Matx**

Matx是固定矩阵类，为编译时就已知维度的矩阵打造的。因为它们内部所有数据都是再堆栈上分配的，所以它们的分配和清除都很快。Matx实际上是一个模板cv::Matx<>，通常通过以下格式进行分配：cv::Matx{1, 2, ...}{1,2,...}{f,d}，里面的数字被限制在1~6中。下面给出一些构造Matx的示例

```C++
void createMatx()
{
	// 构造一个固定矩阵类
	cv::Matx22d m0(1, 2, 3, 4);
	cv::Matx<double, 2, 2> m1(1, 2, 3, 4);
	
	// 复制构造
	cv::Matx22d m2(m0);

	// 静态构造
	cv::Matx22d m3 = cv::Matx22d::zeros();

	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
	std::cout << "m3:\n" << m3 << std::endl;

	// 成员访问
	std::cout << "m2(0, 0) = " << m2(0, 0) << std::endl;
}
```

输出：

```C++
m0:
[1, 2;
 3, 4]
m1:
[1, 2;
 3, 4]
m2:
[1, 2;
 3, 4]
m3:
[0, 0;
 0, 0]
m2(0, 0) = 1
```

固定矩阵类只能创建一些固定的格式，不如Mat灵活，但却更具效率，同时可以进行常见的矩阵线性代数运算。需要注意的是，固定矩阵类的成员访问方式和Mat差得比较多，详见上面的例子。



**Mat_**

Mat_ 是Mat矩阵的容器形式，其构造方式示例如下：

```C++
void createMat_()
{
	// 默认构造
	cv::Mat_<uchar> m0(3, 3);

	// 赋初值
	cv::Mat_<uchar> m1 = (cv::Mat_<uchar>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

	// 复制构造
	cv::Mat_<uchar>m2(m1);

	// 静态构造
	cv::Mat_<cv::Vec3b> m3 = cv::Mat_<cv::Vec3b>::ones(3, 3);

	// 与Mat交流
	cv::Mat m4(3, 3, CV_64FC1, cv::Scalar(128));
	cv::Mat_<double>& m4_ = (cv::Mat_<double>&) m4;
	cv::Mat_<double> m4__(m4);
	
	std::cout << "m0:\n" << m0 << std::endl;
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
	std::cout << "m3:\n" << m3 << std::endl;
	std::cout << "m4:\n" << m4 << std::endl;
	std::cout << "m4_:\n" << m4_ << std::endl;
	std::cout << "m4__:\n" << m4__ << std::endl;
	std::cout << "&m4:\n" << &m4 << std::endl;
	std::cout << "&m4_:\n" << &m4_ << std::endl;
	std::cout << "&m4__:\n" << &m4__ << std::endl;
    
    // 成员访问
	std::cout << "m2(1, 1) = " << (int)m2(1, 1) << std::endl;
	std::cout << "m2.at<uchar>(1, 1) = " << (int)m2.at<uchar>(1, 1) << std::endl;
}
```

输出：

```C++
m0:
[111,   0, 114;
   0, 101,   0;
  52,   0,  52]
m1:
[  1,   2,   3;
   4,   5,   6;
   7,   8,   9]
m2:
[  1,   2,   3;
   4,   5,   6;
   7,   8,   9]
m3:
[  1,   0,   0,   1,   0,   0,   1,   0,   0;
   1,   0,   0,   1,   0,   0,   1,   0,   0;
   1,   0,   0,   1,   0,   0,   1,   0,   0]
m4:
[128, 128, 128;
 128, 128, 128;
 128, 128, 128]
m4__:
[128, 128, 128;
 128, 128, 128;
 128, 128, 128]
&m4:
000000F56B58FC10
&m4_:
000000F56B58FC10
&m4__:
000000F56B58FCD0
m2(1, 1) = 5
m2.at<uchar>(1, 1) = 5
```

使用Mat_ 容器创建矩阵可以让我们更清楚的了解矩阵内每个元素的数据格式。有两点需要特别注意：

1. Mat_的赋初值方式：<< , () 等
2. 通过Mat进行矩阵构造的方式
3. Mat_既可如Mat一样访问成员（如at，ptr等），也可像Matx通过 () 来访问成员

