//////////////////////////////////////////////////////////////////////////
// ���ù��� Utility.h
// 2016-12-25��by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Telephone:17761745857
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __UTILITY_H__
#define __UTILITY_H__
#include "CommonDefinition.h"
#include"Mat.h"
namespace QQ
{

	// �߽����䣬ʹ��0����
	DLL_EXPORTS void CopyMakeBorder(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, int top, int bottom, int left, int right);

	// �������ͼ
	//dstImage:����ͼ
	DLL_EXPORTS void ComputeIntegralImage(const Mat<uchar> &srcImage, Mat<int> &dstImage);//�Ľ��ķ���




}//end of namespace QQ
#endif