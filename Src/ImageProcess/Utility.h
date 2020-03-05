// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// ���ù���

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Mat.h"

namespace QQ
{

// �߽����䣬ʹ��0����
DLL_EXPORTS void CopyMakeBorder(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, int top, int bottom, int left, int right);

// �������ͼ(CalculateIntegralImage)
//dstImage:����ͼ
DLL_EXPORTS void ComputeIntegralImage(const Mat<uchar> &srcImage, Mat<int> &dstImage);

}//end of namespace QQ
#endif
