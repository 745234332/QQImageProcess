//////////////////////////////////////////////////////////////////////////
// ������ ImageProcessTest.h
// 2014-11-13��by QQ
//
// Contact:
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////


#ifndef __QQ_IMAGEPROCESS_TEST_H__
#define __QQ_IMAGEPROCESS_TEST_H__


class ImageProcessTest
{
public:
	void TestCreateImage();//���Դ���ͼ��
	void TestReadAndWrite();//���Զ�д
	void TestRotate();//������ת
	void TestResize();//����Resize
	void TestCvtColor();//����CvtColor
	void TestEqualizeHistogram();//����ֱ��ͼ���⻯
	void TestMemoryLeak();//�����Ƿ����ڴ�й¶
	void TestFilter();//�����˲�
	void TestMallocAndFree();// �����ڴ�ķ�����ͷ�
	void TestEdge();
};

#endif



