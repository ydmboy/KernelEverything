#define BASESUP_PRIVATE	
#include <phbase.h>

PKRE_OBJECT_TYPE KrEStringType;

BOOLEAN KrEInitializeBase()
{
	return NT_SUCCESS(KrECreateObjectType(
		&KrEStringType,
		0,
		NULL
		));
}

 PVOID KrEAllocate(__in SIZE_T Size)
{
	return RtlAllocateHeap(KrEHeapHandle, 0, Size);
}

PVOID KrEFree(__in PVOID Memory)
{
	HeapFree(KrEHeapHandle, 0, Memory);
}
