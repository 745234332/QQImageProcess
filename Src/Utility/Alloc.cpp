#define DLLAPI_EXPORTS
#include<assert.h>
#include "Alloc.h"

namespace QQ
{
	/////////////////////////////////////////// �ڴ�ķ������ͷ�///////////////////////////////////
	// ʵ�������ֽڶ�����ڴ������ͷ�
	void *AlignedMalloc(size_t size, int aligned)
	{
		// aligned is a power of 2
		assert((aligned&(aligned - 1)) == 0);

		// �����ڴ�ռ�
		void *data = malloc(sizeof(void *)+aligned + size);
		//printf("data:%d\n", data);

		// ��ַ���룬temp��ŵľ��ǵ�ַ
		void **temp = (void **)data + 1;
		void **alignedData = (void **)(((size_t)temp + aligned - 1)&-aligned);
		//printf("alignedData:%d\n", alignedData);

		// ����ԭʼ�ڴ��ַ
		alignedData[-1] = data;

		return alignedData;  // ��ת��Ϊһ��ָ��
	}

	void AlignedFree(void *data)
	{
		if (data)
		{
			//printf("data:%d\n", ((void **)data)[-1]);
			free(((void **)data)[-1]);
		}
	}
}//namespace QQ