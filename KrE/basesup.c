#define BASESUP_PRIVATE	
#include <phbase.h>

 PVOID KrEAllocate(__in SIZE_T Size)
{
	return RtlAllocateHeap(KrEHeapHandle, 0, Size);
}

PVOID KrEFree(__in PVOID Memory)
{
	HeapFree(KrEHeapHandle, 0, Memory);
}
