#ifndef _NTBASIC_H
#define	_NTBASIC_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <phbase.h>

// basic types

typedef PVOID* PPVOID;
typedef ULONG LOGICAL;
typedef LONG KPRIORITY;



#define NT_SUCCESS(Status)  (((NTSTATUS)(Status))>=0)



// Sychronization enumerations

typedef enum _WAIT_TYPE
{
	WaitAll,
	WaitAny
} WAIT_TYPE;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
}UNICODE_STRING,*PUNICODE_STRING;


typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING Object;
	ULONG Atrributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRUBUTES,*POBJECT_ATTRIBUTES;


typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID,*PCLIENT_ID;

#endif

