//////////////////////////////////////////////////////////////////////////
// �ڴ�������ͷ�
// 2016-5-30,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

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
