#ifndef HDR_H
#define HDR_H
#include "opencv2/opencv.hpp"
using namespace cv;
Mat addWeight(const Mat & src, const Mat & weight);
// laplacian�˲�,��ñ߽���Ϣ
Mat laplacian(const Mat& src);

// ͨ��RGB��׼����ɫ����������
Mat Saturation(const Mat& src);

// ����ع���������
Mat Wellexposedness(const Mat& src);

// ����߿�����
int border(int rows, int cols, int n);

// ����gaussian������
void gaussianPyramid(const Mat & src, Mat pyr[], int nlev, int borderType);
// �ӽ������ؽ�ͼƬ
cv::Mat reconstructLaplacianPyramid(cv::Mat pyr[], int nlev);	

// ����laplacian������
void laplacianPyramid(const Mat & src, Mat pyr[], int nlev, int borderType);
void sumTo1(cv::Mat src[], int num);
#endif // !HDR

