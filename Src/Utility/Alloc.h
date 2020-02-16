//////////////////////////////////////////////////////////////////////////
// �ڴ�������ͷ� Alloc.h
// 2016-5-30��by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __QQ_ALLOC_H__
#define __QQ_ALLOC_H__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "CommonDefinition.h"

namespace QQ
{

	// ʵ�������ֽڶ�����ڴ������ͷ�
	DLL_EXPORTS void *AlignedMalloc(size_t size, int aligned); // aligned����16����32
	DLL_EXPORTS void AlignedFree(void *data);



}//end of namespace QQ
#endif