// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// �Ҷȱ任����ɫ�ռ�ת��

#ifndef __GRAY_TRANSFORMATION_H__
#define __GRAY_TRANSFORMATION_H__

#include"Mat.h"

namespace QQ
{

// �ҶȻ�
DLL_EXPORTS void RGB2Gray(const Mat<uchar> &srcImage, Mat<uchar> &dstImage);

// ��Ҷ�ͼ��ƽ���Ҷ�
DLL_EXPORTS int ComputeMeanGray(const Mat<uchar> &srcImage);

// ֱ��ͼ���⻯(ͨ���Ҷȱ任ʵ��)
DLL_EXPORTS void EqualizeHistogram(const Mat<uchar> &srcImage, Mat<uchar> &dstImage);


}//namespace QQ



#endif
