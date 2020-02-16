//////////////////////////////////////////////////////////////////////////
// ��дBMPͼ  Bmp.h
// 2014-11-12��by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __QQ_BMP_H__
#define __QQ_BMP_H__
#include "Mat.h"
#include "CommonDefinition.h"
#if (defined WIN32 || defined _WIN32)
#include "BmpDefinition_Windows.h" // ��Linux��ʹ�� BmpDefinition_Linux.h
#else
#include "BmpDefinition_Linux.h"
#endif
#include <string>
using namespace std;

namespace QQ
{

// C++�ӿڣ�֧��8λ��ͨ����3ͨ����
DLL_EXPORTS void ReadBmp(const string &fileName,Mat<uchar> &image);// ��BMP
DLL_EXPORTS void WriteBmp(const string &fileName, const Mat<uchar> &image);// дBMP
DLL_EXPORTS void WriteMarkedBMP(const string &fileName, const Mat<uchar>& image);// ���BMPͼ

// C�ӿ�
DLL_EXPORTS uchar *ReadBmp_C(const char * fileName, int &width, int &height, int &bitCount_PerPixel);
DLL_EXPORTS bool WriteBmp_C(uchar *image, int width, int height, const char * fileName, int bitCount_PerPixel);
DLL_EXPORTS bool WriteMarkedBmp_C(uchar *image, int width, int height, const char * filename);

}//namespace QQ
#endif