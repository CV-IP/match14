#include "hdr.h"
using namespace cv;
using namespace std;

Mat addWeight(const Mat & src,const Mat & weight)
{
	int r = src.rows;
	int c = src.cols;
	Mat temp = src.clone();
	float * pI;
	const float * pW;
	for (int j = 0; j < temp.rows; j++)
	{
		pI = temp.ptr<float>(j);
		pW = weight.ptr<float>(j);
		for (int i = 0; i < temp.cols; i++)
		{
			for (int k = 0; k < 3; k++)
			{
				*pI++ *= *pW;
			}
			pW++;
		}
	}
	return temp;
}



// laplacian�˲�,��ñ߽���Ϣ
Mat laplacian (const Mat& src)
{
	Mat dst;
	Mat FILTER = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
	filter2D(src, dst, -1, FILTER);
	return dst;
}

// ͨ��RGB��׼����ɫ����������
Mat Saturation(const Mat& src)
{
	int rows = src.rows;
	int cols = src.cols;
	Mat dst = Mat::zeros(rows, cols, CV_32F);	//�����������ͼ��	
	const float * p;
	float * p2;
	double r, g, b, m;
	for (int j = 0; j < rows; j++)
	{
		p = src.ptr<float>(j);
		p2 = dst.ptr<float>(j);
		for (int i = 0; i < cols; i++)
		{
			b = *p++;
			g = *p++;
			r = *p++;
			m = (r + g + b) / 3.0;
			*p2++ = sqrt(((r-m)*(r-m)+(g-m)*(g-m)+(b-m)*(b-m)) / 3.0);
		}
	}
	return dst;
}

// ����ع���������
Mat Wellexposedness (const Mat& src)
{
	Mat s[3];
	split(src, s);
	int rows = src.rows;
	int cols = src.cols;
	Mat dst = Mat::ones(rows, cols, CV_32F);	//�����������ͼ��
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				dst.at<float>(i, j) *= exp(-(s[k].at<float>(i, j) - 0.5) *
					(s[k].at<float>(i, j) - 0.5) / 0.08);
			}
		}
	}
	return dst;
}

// ����gaussian������
void gaussianPyramid (const Mat & src, Mat pyr[], int nlev, int borderType)
{
	Mat img;
	int rows = src.rows;
	int cols = src.cols;
	copyMakeBorder(src, img, border(rows, cols, 1), border(rows, cols, 2), border(rows, cols, 3), border(rows, cols, 4), borderType);	//�ӱ߿�
	pyr[1] = img;
	for (int i = 2; i <= nlev; i++)
	{
		pyrDown(pyr[i-1], pyr[i], Size( pyr[i - 1].cols / 2, pyr[i - 1].rows / 2));		//���²���,��߱�����2^n
	}
}

// ����laplacian������
void laplacianPyramid (const Mat & src, Mat pyr[], int nlev, int borderType)
{
	int rows = src.rows;
	int cols = src.cols;
	Mat J, I, temp;
	copyMakeBorder(src, J, border(rows, cols, 1), border(rows, cols, 2), border(rows, cols, 3), border(rows, cols, 4), borderType);
	for (int i = 1; i < nlev; i++)
	{
		pyrDown(J, I, Size( J.cols/2, J.rows/2 ));		// ���²���,��߱�����2^n
		pyrUp(I, temp, Size( I.cols * 2, I.rows * 2));	
		pyr[i] = J - temp;	// ����ͼƬ�����¡����ϲ�����ͬ�Ĳ���
		J = I;
	}
	pyr[nlev] = J;
}

// ����߿�����,ʹͼƬ���Ϊ2^n,��ʹ��cv::pyrUp,cv::pyrDown
int border (int rows, int cols, int n)
{
	int up, down, left, right;
	int a = 1;
	int b = 1;
	while (a < rows)
	{
		a *= 2;
	}
	while (b < cols)
	{
		b *= 2;
	}
	up = down = (a - rows) / 2;
	left = right = (b - cols) / 2;
	if (rows % 2 == 1) up++;
	if (cols % 2 == 1) left++;
	switch (n)
	{
	case 1: 
		return up;
	case 2: 
		return down;
	case 3: 
		return left;
	case 4: 
		return right;
	default:
		return 0;
		break;
	}
}
// �ӽ������ؽ�ͼƬ
Mat reconstructLaplacianPyramid(Mat pyr[], int nlev)
{
	Mat I = pyr[nlev];
	Mat temp;
	for (int i = nlev - 1; i >= 1; i--)
	{
		pyrUp(I, temp, Size(I.cols * 2, I.rows * 2));
		I = temp.clone();
		I += pyr[i];
	}
	return I;
}
//��32F����Mat�����һ��
void sumTo1(Mat src[], int num)
{
	int r = src[0].rows;
	int c = src[0].cols;
	float m;
	float * data[20];
	for (int j = 0; j < r; j++)
	{
		for (int k = 0; k < num; k++)
		{
			data[k] = src[k].ptr<float>(j);
		}
		for (int i = 0; i < c; i++)
		{
			m = 0;
			for (int k = 0; k < num; k++)
			{
				*data[k] += 0.00001;
				m += *data[k];
			}
			if (m == 0.0)
			{
				m += 0.00001;
			}
			for (int k = 0; k < num; k++)
			{
				*data[k]++ /= m;
			}
		}
	}
}
