// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// ��Ե���

#ifndef __EDGE_H__
#define __EDGE_H__

#include "Mat.h"

namespace QQ
{

// Sobel ����
DLL_EXPORTS void Sobel(const Mat<uchar> &srcImage, Mat<uchar> &dstImage);

// Canny(Canny3),�Ƽ���ֵ:low:50 , high:150
DLL_EXPORTS void Canny(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, double lowThreshold, double highThreshold, bool L2 = false);









}// namesapce QQ




#endif
