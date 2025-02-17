#ifndef  _NTRTL_H
#define _NTRTL_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#define RtlAllocateHeap HeapAlloc
#define RtlFreeHeap		HeapFree

VOID FORCEINLIEN RtlInitUnicodeString(
	__out PUNICODE_STRING DestinationString,
	__in PWSTR SourceString
)
{
	DestinationString->MaximumLength = DestinationString->Length = wcslen(SourceString) * sizeof(WCHAR);
	DestinationString->Buffer = SourceString;
}


#endif
