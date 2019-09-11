/*******************************************************************************

                          内存管理单元-只分配不释放方式实现
此文件移值至FreeRTOS中的heap_1.c
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
	unsigned long ulDummy;  //保证4字节对齐
	unsigned char ucHeap[ MEM_MNG_HEAP_SIZE ];
} xHeap;

size_t xNextFreeByte = ( size_t ) 0; //指已使用的内存CHAR个数
//unsigned short xWantedRamCount = 0;


//----------------------------内存分配-----------------------------------------
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
      MemMng_cbOvNotify(); //内存溢出通报
      //xWantedRamCount += xWantedSize;
    }
	}
	return pvReturn;
}

//----------------------------内存窃取函数-------------------------------------
//此函数将未使部用部空间用它作它，仅Heap_1模式，单线程调用
//注意可用空间为：MEM_MNG_HEAP_SIZE - xNextFreeByte;
void *MemMng_pvSteal(void)
{
  return  &( xHeap.ucHeap[ xNextFreeByte ] );
}

//----------------------------内存释放-----------------------------------------
//注意实现是否能够支持,不支持时不能够调用
void MemMng_Free(void *pv)
{
	/* Memory cannot be freed using this scheme.  See heap_2.c and heap_3.c 
	for alternative implementations, and the memory management pages of 
	http://www.FreeRTOS.org for more information. */
	( void ) pv;
}

//----------------------------模块初始化--------------------------------------
//开机时调用,保证堆空间数据为0
void MemMng_Init(void)
{
	/* Only required when static memory is not cleared. */
	xNextFreeByte = ( size_t ) 0;
  memset(xHeap.ucHeap, 0, MEM_MNG_HEAP_SIZE);
}


