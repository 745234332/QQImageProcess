// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// ��дBMPͼ

#ifndef __BMP_H__
#define __BMP_H__

#include "Mat.h"
#include <string>
using namespace std;

namespace QQ
{

// ��BMP
DLL_EXPORTS void ReadBmp(const string &fileName,Mat<uchar> &image);

// дBMP
DLL_EXPORTS void WriteBmp(const string &fileName, const Mat<uchar> &image);

// ���BMPͼ
DLL_EXPORTS void WriteMarkedBMP(const string &fileName, const Mat<uchar>& image);

}//namespace QQ
#endif
