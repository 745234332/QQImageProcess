//////////////////////////////////////////////////////////////////////////
// �˲�
// 2014-12-10,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __FILTER_H__
#define __FILTER_H__

#include "Mat.h"

namespace QQ
{

// ��ֵ�˲�(V4.0)
DLL_EXPORTS void Blur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, Size kernelSize);

// ��ֵ�˲�(V4.0)
// width_Aperture:�������ڴ�С
DLL_EXPORTS void MedianBlur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, int width_Aperture);

// ��˹�˲�
// sigma����׼�����ֵ��0.84089642
DLL_EXPORTS void GaussianBlur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, double sigma);

// Gabor �˲�,��ȡGabor�˲���
// D:\Library\OpenCV\OpenCV_2.4.12\sources\modules\imgproc\src\gabor.cpp
DLL_EXPORTS Mat<float> GetGaborKernel(Size ksize, double sigma, double theta, double lambd, double gamma, double psi = PI*0.5);

// �������(Convolution22),ֻ֧�ֵ�ͨ��
DLL_EXPORTS void Convolution(const Mat<uchar> &srcImage, const Mat<float> &kernel, Mat<uchar> &dstImage);

















}//end of namespace QQ

#endif
