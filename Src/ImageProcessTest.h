//////////////////////////////////////////////////////////////////////////
// ������ ImageProcessTest.h
// 2014��11��13�� �����ģ�by QQ
//
// Contact://		Telephone:15366105857
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////


#ifndef __QQ_IMAGEPROCESS_TEST_H__
#define __QQ_IMAGEPROCESS_TEST_H__

#include "Mat.h"
using namespace QQ;

// ����ʾ��ͼ��
#ifdef _WIN32
#define LENA_GRAY		"../../../../Resource/Image/Gray/Lena512.bmp"
#define LENA_COLOR		"../../../../Resource/Image/Color/Lena800.bmp"
#define BEAUTY_COLOR	"../../../../Resource/Image/Color/Beauty.bmp"
#define BEAUTY_GRAY		"../../../../Resource/Image/Gray/Beauty.bmp"
#define CAMERA_GRAY		"../../../../Resource/Image/Gray/Camera256.bmp"
#define CAMERA_COLOR	"../../../../Resource/Image/Color/Camera256.bmp"
#else
#define LENA_GRAY		"../../../Resource/Image/Gray/Lena512.bmp"
#define LENA_COLOR		"../../../Resource/Image/Color/Lena800.bmp"
#define BEAUTY_COLOR	"../../../Resource/Image/Color/Beauty.bmp"
#define BEAUTY_GRAY		"../../../Resource/Image/Gray/Beauty.bmp"
#define CAMERA_GRAY		"../../../Resource/Image/Gray/Camera256.bmp"
#define CAMERA_COLOR	"../../../Resource/Image/Color/Camera256.bmp"
#endif

class ImageProcessTest
{
public:
	void TestCreateImage();//���Դ���ͼ��
	void TestReadAndWrite();//���Զ�д
	void TestRotate();//������ת
	void TestResize();//����Resize
	void TestCvtColor();//����CvtColor
	void TestAssignmentOperator(Mat<uchar> &srcImage);//�������ظ�ֵ������
	void TestEqualizeHistogram();//����ֱ��ͼ���⻯
	void TestMemoryLeak();//�����Ƿ����ڴ�й¶
	void TestFilter();//�����˲�
	void TestMallocAndFree();// �����ڴ�ķ�����ͷ�
	void TestEdge();
};

#endif



