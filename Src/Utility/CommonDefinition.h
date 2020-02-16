//////////////////////////////////////////////////////////////////////////
// ͨ�ö��� CommonDefinition.h
// 2014-11-13��by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact://		Telephone:15366105857
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////


#ifndef __QQ_COMMON_DEFINITION_H__
#define __QQ_COMMON_DEFINITION_H__
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//////////////////////////////Common macros////////////////////////////////////////////
#ifdef DLLAPI_EXPORTS
#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
#  define DLL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#  define DLL_EXPORTS __attribute__((visibility ("default")))
#endif
#endif

#ifndef DLL_EXPORTS
#define DLL_EXPORTS
#endif

#ifndef MAX
#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#define PI   3.1415926535897932384626433832795
#define LOG2 0.69314718055994530941723212145818


//�Ƕ�ת��
#define DEGREE2RADIAN(x) (x*PI/180)//�Ƕ�ת����
#define RADIAN2DEGREE(x) (x*180/PI)//����ת�Ƕ�

#define  DESCALE(x,n)  (((x)+(1 << ((n)-1))) >> (n))
//#define  DESCALE(x,n)  ((x) >> (n)) 

// ��������
#define  saturate(x)   (uchar)(((x) & ~255) == 0 ? (x) : ~((x)>>31))

// memory deallocation
#define SAFE_DELETE(p)  { if ((p)) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p)  { if ((p)) { delete[] (p); (p) = NULL; } }


///////////////////////////////Common type definitions///////////////////////////////////////////////

typedef unsigned char uchar;
typedef unsigned short ushort; // 2 uchars
typedef unsigned int uint;  // 4 uchars

//Resize:interpolation methods��ֵ��ʽ
enum
{
	NEAREST   =0,//����ڲ�ֵ
	LINEAR    =1,//˫���Բ�ֵ,ͼ����Сʱ�����׳��ֲ���
	AREA      =2//���ع�ϵ�ز���
};


////////////////////////////namespace QQ//////////////////////////////////////////////
namespace QQ
{



/////////////////////////////Scalar/////////////////////////////////////////////
// ��ʾ����ֵ
class Scalar
{
public:
	//��Ա����
	//���캯��
	Scalar(double val0=0, double val1=0,double val2=0, double val3=0);//Ĭ�϶�Ϊ0
	Scalar(const Scalar &scalar);
	
	//��Ա����
	double val[4];
};
inline Scalar::Scalar(double val0, double val1,double val2, double val3)
{
	val[0]=val0;
	val[1]=val1;
	val[2]=val2;
	val[3]=val3;
}
inline Scalar::Scalar(const Scalar &scalar)
{
	val[0]=scalar.val[0];
	val[1]=scalar.val[1];
	val[2]=scalar.val[2];
	val[3]=scalar.val[3];
}

///////////////////////////Size///////////////////////////////////////////////
class Size
{
public:
	//��Ա����
	//���캯��
	Size();
	Size(int _width,int _height);
	Size(const Size &size);

	//��Ա����
	int width;
	int height;

};
inline Size::Size():width(0),height(0){}
inline Size::Size(int _width,int _height):width(_width),height(_height){}
inline Size::Size(const Size &size):width(size.width),height(size.height){}


///////////////////////////Vec3b///////////////////////////////////////////////
class Vec3b
{
public:
	//���캯��
	Vec3b(uchar val0 = 0, uchar val1 = 0, uchar val2 = 0);//Ĭ�϶�Ϊ0
	Vec3b(const Vec3b &vec3b);

	inline uchar& operator[](int i);

	//��Ա����
	uchar val[3];
};
inline Vec3b::Vec3b(uchar val0, uchar val1, uchar val2)
{
	val[0] = val0;
	val[1] = val1;
	val[2] = val2;
}
inline Vec3b::Vec3b(const Vec3b &vec3b)
{
	val[0] = vec3b.val[0];
	val[1] = vec3b.val[1];
	val[2] = vec3b.val[2];
}

inline uchar& Vec3b::operator[](int i)
{
	return val[i];
}

///////////////////////////Point///////////////////////////////////////////////
class Point
{
public:
	//���캯��
	Point() :x(0), y(0){}
	Point(int _x, int _y) :x(_x), y(_y){}
	//Point(const Point &point);

	//��Ա����
	int x, y;
};




}//namespace QQ

#endif
