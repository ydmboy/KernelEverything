#ifndef _REF_H
#define _REF_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <phbase.h>

#define KRE_OBJECT_TYPE_VALID_FLAGS 0x0000000

#define KRE_OBJECT_VALID_FLAGS		0x0000001
#define KRE_OBJECT_RAISE_ON_FAIL    0x0000001


struct _KRE_OBJECT_TYPE;
typedef struct _KRE_OBJECT_TYPE *PKRE_OBJECT_TYPE;


NTSTATUS KrEInitializeRef();

/*
 *	the pointer to the deletion procedure
 */
typedef VOID(NTAPI* PKRE_TYPE_DELETE_PROCEDURE)(
	__in PVOID Object,
	__in ULONG Flags	
	);

NTSTATUS KrECreateObjectType(
	__out PKRE_OBJECT_TYPE* ObjectType,
	__in ULONG Flags,
	__in PKRE_TYPE_DELETE_PROCEDURE DeleteProcedure
);

NTSTATUS KrECreateObject(
	__out PVOID* Object,
	__in SIZE_T ObjectSize,
	__in ULONG Flags,
	__in_opt PKRE_OBJECT_TYPE ObjectType,
	__in_opt LONG AdditionalReferences
);

BOOLEAN KrEDereferenceObject(__in PVOID Object);

LONG KrEDereferenceObjectEx(
	__in PVOID Object,
	__in LONG RefCount,
	__in BOOLEAN DeferDelete
);

#endif
