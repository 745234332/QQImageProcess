// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// �ڴ�������ͷ�

#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "CommonDefinition.h"

namespace QQ
{

// ʵ�������ֽڶ�����ڴ������ͷ�
DLL_EXPORTS void *AlignedMalloc(int size, int aligned); // aligned����16����32
DLL_EXPORTS void AlignedFree(void *data);



}//end of namespace QQ
#endif
