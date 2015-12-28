#pragma once
#include "opencv2/opencv.hpp"


//************************************
// Method:    matchPixel
// FullName:  matchPixel
// Access:    public 
// Returns:   int 0:�ɹ��޸� 1:������� -1:ʧ��
// Qualifier:
// Parameter: const cv::Mat & img ��ƥ��ͼ��
// Parameter: const cv::Mat & ref �ο�ͼ��
// Parameter: cv::Mat & rel ��Χ��ϵ����3*3
// Parameter: int x 
// Parameter: int y
// Parameter: int threshold ��ֵ
// Parameter: int pixelSize odd
// Introduce: Ѱ��ref����img(x, y)�����ƥ�䣬����޸�rel���ĵ�
//************************************
int matchPixel(const cv::Mat & img, const cv::Mat & ref, cv::Mat & rel, int x, int y, float threshold, int pixelSize = 7);
//************************************
// Method:    getCenter
// FullName:  getCenter
// Access:    public 
// Returns:   cv::Vec3f
// Qualifier:
// Parameter: cv::Mat rel ��Χ��ϵ����3*3
// Introduce: ��ȡ��ϵ�������ĵ�
//************************************
cv::Vec3f & getCenter(cv::Mat & rel);