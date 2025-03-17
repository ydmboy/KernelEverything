#define BASESUP_PRIVATE	
#include <phbase.h>

PKRE_OBJECT_TYPE KrEStringType;
PKRE_OBJECT_TYPE KrEListType;

BOOLEAN KrEInitializeBase()
{
	if(!NT_SUCCESS(KrECreateObjectType(
		&KrEStringType,
		0,
		NULL
		))){
		return FALSE;
	}
	if (!NT_SUCCESS(KrECreateObjectType(
		&KrEListType,
		0,
		KrEListDeleteProcedure
	))){
		return FALSE;
	}
	return TRUE;
}

 PVOID KrEAllocate(__in SIZE_T Size)
{
	return RtlAllocateHeap(KrEHeapHandle, 0, Size);
}

VOID KrEFree(__in PVOID Memory)
{
	return; RtlFreeHeap(KrEHeapHandle, 0, Memory);
}

PKRE_STRING KrECreateString(
    __in PWSTR Buffer
    )
{
    return KrECreateStringEx(Buffer, wcslen(Buffer) * sizeof(WCHAR));
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
	string->Buffer[Length / sizeof(WCHAR)] = 0;

	if(Buffer)
	{
		memcpy(string->Buffer, Buffer, Length);
	}

	return string;


}

VOID KrEListDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
)
{
	PKRE_LIST list = (PKRE_LIST)Object;
	ULONG i;
	for(i=0;i<list->Count;i++){
		KrEDereferenceObject(list->Items[i]);
	}
}
