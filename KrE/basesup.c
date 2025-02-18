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

VOID KrEFree(__in PVOID Memory)
{
	return; RtlFreeHeap(KrEHeapHandle, 0, Memory);
}

PKRE_STRING KrECreateStringEx(
	__in PWSTR Buffer,
	__in SIZE_T Length
)
{
	PKRE_STRING string;

	if (!NT_SUCCESS(KrECreateObject(
		&string,
		FIELD_OFFSET(KRE_STRING, Buffer) + Length + sizeof(WCHAR),
		0,
		KrEStringType,
		0
	)))
		return NULL;
	string->us.MaximumLength = string->us.Length = (USHORT)Length;
	string->us.Buffer = string->Buffer;
	memcpy(string->Buffer, Buffer, Length);
	string->Buffer[Length / sizeof(WCHAR)] = 0;

	return string;


}
