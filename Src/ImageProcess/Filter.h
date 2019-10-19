//////////////////////////////////////////////////////////////////////////
// �˲� Smooth.h
// 2014-12-10��by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Telephone:17761745857
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////
// updated 2017-1-11 by QQ,add Blur,MedianBlur,GaussianBlur,Convolution 

#ifndef __SMOOTH_H__
#define __SMOOTH_H__
#include "Mat.h"
#include"Utility.h"
#include"Histogram.h"

namespace QQ
{

// ��ֵ�˲�,V4.0 ��Ч��ֵ�˲�
DLL_EXPORTS void Blur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, Size kernelSize);

// ��ֵ�˲���V4.0 ��Ч��ֵ�˲�
// width_Aperture:�������ڴ�С
DLL_EXPORTS void MedianBlur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, int width_Aperture);
void GetMedianValueAndPixelCountLowerMedian(const Mat<uchar> &image, int &medianValue, int &pixelCountLowerMedian);

// ��˹�˲�
// sigma����׼�����ֵ��0.84089642
DLL_EXPORTS void GaussianBlur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, double sigma);

// Gabor �˲�,��ȡGabor�˲���
// D:\Library\OpenCV\OpenCV_2.4.12\sources\modules\imgproc\src\gabor.cpp
DLL_EXPORTS Mat<float> GetGaborKernel(Size ksize, double sigma, double theta, double lambd, double gamma, double psi = PI*0.5);


















}//end of namespace QQ

#endif