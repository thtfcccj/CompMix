/*******************************************************************************

                          �ڴ����Ԫ-ֻ���䲻�ͷŷ�ʽʵ��
���ļ���ֵ��FreeRTOS�е�heap_1.c
*******************************************************************************/

#include "MemMng.h"

#include <string.h>

/* Setup the correct byte alignment mask for the defined byte alignment. */

#if MEMMNG_ALIGNMENT == 8
	#define MEMMNG_ALIGNMENT_MASK ( ( size_t ) 0x0007 )
#endif

#if MEMMNG_ALIGNMENT == 4
	#define MEMMNG_ALIGNMENT_MASK	( ( size_t ) 0x0003 )
#endif

#if MEMMNG_ALIGNMENT == 2
	#define MEMMNG_ALIGNMENT_MASK	( ( size_t ) 0x0001 )
#endif

#if MEMMNG_ALIGNMENT == 1 
	#define MEMMNG_ALIGNMENT_MASK	( ( size_t ) 0x0000 )
#endif


/* Allocate the memory for the heap.  The struct is used to force byte
alignment without using any non-portable code. */
static struct xRTOS_HEAP
{
	unsigned long ulDummy;  //��֤4�ֽڶ���
	unsigned char ucHeap[ MEM_MNG_HEAP_SIZE ];
} xHeap;

size_t xNextFreeByte = ( size_t ) 0; //ָ��ʹ�õ��ڴ�CHAR����
//unsigned short xWantedRamCount = 0;


//----------------------------�ڴ����-----------------------------------------
void *MemMng_pvMalloc(size_t xWantedSize)
{
	/* Ensure that blocks are always aligned to the required number of bytes. */
	#if MEMMNG_ALIGNMENT != 1
		if( xWantedSize & MEMMNG_ALIGNMENT_MASK )
		{
			/* Byte alignment required. */
			xWantedSize += ( MEMMNG_ALIGNMENT - ( xWantedSize & MEMMNG_ALIGNMENT_MASK ) );
		}
	#endif
  
  void *pvReturn = NULL; 
	{
		/* Check there is enough room left for the allocation. */
		if( ( ( xNextFreeByte + xWantedSize ) < MEM_MNG_HEAP_SIZE ) &&
			( ( xNextFreeByte + xWantedSize ) > xNextFreeByte )	)/* Check for overflow. */
		{
			/* Return the next free byte then increment the index past this
			block. */
			pvReturn = &( xHeap.ucHeap[ xNextFreeByte ] );
			xNextFreeByte += xWantedSize;			
		}
    else
    {
      MemMng_cbOvNotify(); //�ڴ����ͨ��
      //xWantedRamCount += xWantedSize;
    }
	}
	return pvReturn;
}

//----------------------------�ڴ���ȡ����-------------------------------------
//�˺�����δʹ���ò��ռ�������������Heap_1ģʽ�����̵߳���
//ע����ÿռ�Ϊ��MEM_MNG_HEAP_SIZE - xNextFreeByte;
void *MemMng_pvSteal(void)
{
  return  &( xHeap.ucHeap[ xNextFreeByte ] );
}

//----------------------------�ڴ��ͷ�-----------------------------------------
//ע��ʵ���Ƿ��ܹ�֧��,��֧��ʱ���ܹ�����
void MemMng_Free(void *pv)
{
	/* Memory cannot be freed using this scheme.  See heap_2.c and heap_3.c 
	for alternative implementations, and the memory management pages of 
	http://www.FreeRTOS.org for more information. */
	( void ) pv;
}

//----------------------------ģ���ʼ��--------------------------------------
//����ʱ����,��֤�ѿռ�����Ϊ0
void MemMng_Init(void)
{
	/* Only required when static memory is not cleared. */
	xNextFreeByte = ( size_t ) 0;
  memset(xHeap.ucHeap, 0, MEM_MNG_HEAP_SIZE);
}


