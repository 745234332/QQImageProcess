// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// ֱ��ͼ���

#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "Mat.h"

namespace QQ
{

// ����ֱ��ͼ
DLL_EXPORTS void CalculateHistogram(const Mat<uchar> &srcImage,Mat<int> &histogram);


}//namespace QQ



#endif
