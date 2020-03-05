//////////////////////////////////////////////////////////////////////////
// ������ Mat.h (Image��������)
// 2016-12-24��by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////
// Merry Christmas !

#ifndef __QQ_MAT_H__
#define __QQ_MAT_H__

#include "CommonDefinition.h"
#include"Alloc.h"

namespace QQ
{


template <typename T>
class  DLL_EXPORTS Mat
{
public:
	//���캯��
	Mat();
	Mat(int _rows, int _cols, int _numberOfChannels);
	Mat(int _rows, int _cols, int _numberOfChannels, Scalar scalar); 
	Mat(int _rows, int _cols, int _numberOfChannels, void *_data, bool needCopyData = false);// �ⲿ����_data��Ҫ�ⲿ�ͷ�
	
	//��������
	virtual ~Mat();//����Release()
	void Release();//���ü�����1
	void Deallocate();//�ͷ�����

	//�Զ������ڴ�
	void Create(int _rows, int _cols, int _numberOfChannels);
	void Create(Size _size, int _numberOfChannels);

	//���ظ�ֵ������
	Mat& operator = (const Mat &dstMat);//��������

	void SetTo(const Scalar &scalar);

	// ��ȡָ��λ�õ�Ԫ��
	template<typename _Tp>
	inline _Tp& At(int y, int x)
	{
		return ((_Tp*)(data + step*y))[x];
	}

	void InitEmpty();

public:
	int rows;
	int cols;
	int numberOfChannels;//ͨ����
	int step;// ����(ÿ���ֽ���)
	
	uchar *data;	

	//���ü���
	int *refCount;

};// Mat


//////////////////////////////Mat��ʵ��////////////////////////////////////////////

template <typename T>
inline Mat<T>::Mat()
{
	InitEmpty();

}

template <typename T>
inline void Mat<T>::InitEmpty()
{
	rows = cols = numberOfChannels = 0;
	data = 0;
	refCount = 0;

}

template <typename T>
inline Mat<T>::Mat(int _rows, int _cols, int _numberOfChannels)
{
	InitEmpty();
	Create(_rows, _cols, _numberOfChannels);
}

template <typename T>
inline Mat<T>::Mat(int _cols, int _rows, int _numberOfChannels, Scalar scalar)
{
	InitEmpty();
	Create(_cols, _rows, _numberOfChannels);

	SetTo(scalar);

}

//BYTE->Image,IplImage->Image
//Ĭ�ϲ���������,�ⲿ����_data��Ҫ�ⲿ�ͷ�
template <typename T>
inline Mat<T>::Mat(int _rows, int _cols, int _numberOfChannels, void *_data, bool needCopyData)
{
	rows = _rows;
	cols = _cols;
	numberOfChannels = _numberOfChannels;
	step = cols*numberOfChannels*sizeof(T);

	if (needCopyData == false)
	{
		data=(uchar *)_data;

		refCount = 0;
	}
	else
	{
		data = (uchar *)AlignedMalloc((step*rows + (int)sizeof(int)), 16);

		refCount = (int*)(data + step*rows);
		*refCount = 1;

		memcpy(data, _data, step*rows);//����
	}


}
template <typename T>
Mat<T>::~Mat()
{
	Release();//�ͷ�
}

// ���ü�����1��������ü���Ϊ0�ˣ�����Deallocate()
template <typename T>
inline void Mat<T>::Release()
{

	//���ü�����1,������ü���Ϊ0��˵��û�����ã��ͷ�����
	if (refCount && (*refCount)-- == 1)
	{
		Deallocate();
	}

	InitEmpty();

}
//�ͷ�����
template <typename T>
inline void Mat<T>::Deallocate()
{

	AlignedFree(data);


}

template <typename T>
inline void Mat<T>::Create(int _rows, int _cols, int _numberOfChannels)
{
	if (rows == _rows&&cols == _cols&&numberOfChannels == _numberOfChannels)
	{
		return;
	}
	else
	{
		//�����һ�£����ü�����1,��ʱ���ü���Ϊ0���ͷ����ݺ����ü���
		Release();

		rows = _rows;
		cols = _cols;
		numberOfChannels = _numberOfChannels;
		step = cols*numberOfChannels*sizeof(T);

		// �ڴ��ַ16�ֽڶ���(����ָ��Ż�)
		data = (uchar *)AlignedMalloc((step*rows + (int)sizeof(int)), 16);

		refCount = (int*)(data + step*rows);
		*refCount = 1;

	}
}

template <typename T>
inline void Mat<T>::Create(Size _size, int _numberOfChannels)
{
	int _rows = _size.height;
	int _cols = _size.width;

	Create(_rows, _cols, _numberOfChannels);
}

//���ز�����
// ע�ⷵ��ֵΪImage&(�������void��A=(B=C)����ôA��û��ֵ��)
template <typename T>
inline Mat<T>& Mat<T>::operator = (const Mat<T> &dstMat)
{
	if (this != &dstMat)
	{
		// ����this��release
		Release();

		rows = dstMat.rows;
		cols = dstMat.cols;
		numberOfChannels = dstMat.numberOfChannels;
		step = dstMat.step;

		data = dstMat.data;

		//���ü���
		refCount = dstMat.refCount;
		(*refCount)++;
	}

	return *this;

}

template <typename T>
void Mat<T>::SetTo(const Scalar &scalar)
{
	T *dataOfSrc = (T *)data;
	int numberOfElement = rows*cols;
	for (int i = 0; i <= numberOfElement - 1; ++i, dataOfSrc += numberOfChannels)
	{
		for (int k = 0; k <= numberOfChannels - 1; ++k)
			dataOfSrc[k] = scalar.val[k];
	}

}

}// namespace QQ
#endif
