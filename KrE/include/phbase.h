#ifndef _PHBASE_H
#define _PHBASE_H



#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <ntwin.h>
#include <ntimport.h>
#include <ref.h>

#define KrE_APP_NAME (L"KernerlEverything")

#define KrERaiseStatus(Status) RaiseException(Status,0,0,NULL)

#define KRE_INT_STR_LEN 10
#define KRE_INT_STR_LEN_1 (KRE_INT_STR_LEN+1)	

extern PWSTR KrEWindowClassName;
extern HINSTANCE KrEInstanceHandle;
extern HFONT KrEApplicationFont;
extern HANDLE KrEHeapHandle;

#define PTR_ADD_OFFSET(Pointer,Offset) ((PVOID)((ULONG_PTR)(Pointer)+(ULONG_PTR)(Offset)))

typedef struct _KRE_STRING
{
	union
	{
		UNICODE_STRING us;
		struct
		{
			USHORT Length;
			USHORT MaximumLength;
			PWSTR Pointer;
		};
	};
	WCHAR Buffer[1];
}KRE_STRING,*PKRE_STRING;

typedef struct _KRE_LIST{
	ULONG Count;
	ULONG AllocateCount;
	PPVOID Items;
}KRE_LIST,*PKRE_LIST;

BOOLEAN KrEInitializeBase();
PVOID KrEAllocate(__in SIZE_T Size);
VOID KrEFree(__in PVOID Memory);

PKRE_STRING KrECreateStringEx(
	__in PWSTR Buffer,
	__in SIZE_T Length
);


PKRE_STRING KrECreateString(
	__in PWSTR Buffer
);

PWSTR FORCEINLINE KrEGetString(
    __in_opt PKRE_STRING String
    )
{
    if (String)
        return String->Buffer;
    else
        return NULL;
}

VOID KrEListDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
);


#endif

