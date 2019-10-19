//////////////////////////////////////////////////////////////////////////
// �Ҷȱ任����ɫ�ռ�ת�� GrayTransformation.h
// 2014-11-16��by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Telephone:17761745857
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////
// updated 2016-12-25 by QQ

#ifndef __GRAY_TRANSFORMATION_H__
#define __GRAY_TRANSFORMATION_H__

#include"Mat.h"
#include"Histogram.h"

namespace QQ
{
	///////////////////////////////��ɫ�ռ�ת��///////////////////////////////////////////////
	// �ҶȻ�
	DLL_EXPORTS void RGB2Gray(const Mat<uchar> &srcImage, Mat<uchar> &dstImage);

	// ��Ҷ�ͼ��ƽ���Ҷ�
	DLL_EXPORTS int ComputeMeanGray(const Mat<uchar> &srcImage);


	///////////////////////////////�Ҷȱ任///////////////////////////////////////////////
	// ֱ��ͼ���⻯
	DLL_EXPORTS void EqualizeHistogram(const Mat<uchar> &srcImage, Mat<uchar> &dstImage);


}//namespace QQ



#endif