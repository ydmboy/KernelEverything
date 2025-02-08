#ifndef _REFP_H
#define _REFP_H

#if defined(_MSC_VER) &&(_MSC_VER>1020)
#pragma once
#endif

#include <ref.h>

#define KrEObjectToObjectHeader(Object) \
	((PKRE_OBJECT_HEADER)CONTAINING_RECORD((Object),KRE_OBJECT_HEADER,Body))
#define KrEAddObjectHeaderSize(Size) ((Size)+FIELD_OFFSET(KRE_OBJECT_HEADER,Body))
#define KrEObjectHeaderToObject(ObjectHeader) (&((PKRE_OBJECT_HEADER)(ObjectHeader))->Body)

typedef struct _KRE_OBJECT_HEADER *PKRE_OBJECT_HEADER;
typedef struct _KRE_OBJECT_TYPE
{
	ULONG Flags;
	PKRE_TYPE_DELETE_PROCEDURE	DeleteProcedure;
	ULONG NumberOfObject;
}KRE_OBJECT_TYPE,*PKRE_OBJECT_TYPE;

typedef struct _KRE_OBJECT_HEADER
{
	LONG RefCount;
	ULONG Flags;
	union
	{
		SIZE_T Size;
		PKRE_OBJECT_HEADER NextToFree;
	};
	PKRE_OBJECT_TYPE Type;

	QUAD Body;
}KRE_OBJECT_HEADER;


PKRE_OBJECT_HEADER KrEAllocateObject(
	__in SIZE_T ObjectSize
);

#endif
