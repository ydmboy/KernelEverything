#include <refp.h> 


PKRE_OBJECT_TYPE KrEObjectTypeObject = NULL;
PKRE_OBJECT_HEADER KrEObjectNextToFree = NULL;

NTSTATUS KrEInitializeRef()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = KrECreateObjectType(
		&KrEObjectTypeObject,
		0,
		NULL
	);

	if (!NT_SUCCESS(status))
		return status;

	KrEObjectToObjectHeader(KrEObjectTypeObject)->Type = KrEObjectTypeObject;
	KrEObjectTypeObject->NumberOfObject = 1;
	return status;

}


NTSTATUS KrECreateObjectType(
	__out PKRE_OBJECT_TYPE* ObjectType,
	__in ULONG Flags,	// Is Flags parameter is useful??
	__in PKRE_TYPE_DELETE_PROCEDURE DeleteProcedure
)
{
	NTSTATUS status = STATUS_SUCCESS;
	PKRE_OBJECT_TYPE objectType;
	if ((Flags & KRE_OBJECT_VALID_FLAGS) != Flags)
		return STATUS_INVALID_PARAMETER_3;
	status = KrECreateObject(
		&objectType,
		sizeof(KRE_OBJECT_TYPE),
		0,
		KrEObjectTypeObject,
		0
	);
	if (NT_SUCCESS(status))
		return status;

	objectType->Flags = Flags;
	objectType->DeleteProcedure = DeleteProcedure;
	objectType->NumberOfObject = 0;

	*ObjectType = objectType;

	return status;
}

PKRE_OBJECT_HEADER KrEAllocateObject(
	__in SIZE_T ObjectSize
)
{
	return KrEAllocate(KrEAddObjectHeaderSize(ObjectSize));
}


NTSTATUS KrECreateObject(
	__out PVOID* Object,
	__in  SIZE_T ObjectSize,
	__in  ULONG Flags,
	__in_opt PKRE_OBJECT_TYPE ObjectType,
	__in_opt LONG AdditionalReferences
)
{
	PKRE_OBJECT_HEADER objectHeader;


	/*
	 *	Flags = 0x00000001;  // 合法
	 *	if ((Flags & 0x00000001) != Flags)  // 0x00000001 == 0x00000001，不触发 if 语句
	 *	Flags = 0x00000000;  // 合法
	 */
	if ((Flags & KRE_OBJECT_VALID_FLAGS) != Flags)
		return STATUS_INVALID_PARAMETER_3;
	if (!ObjectType && KrEObjectTypeObject)
		return STATUS_INVALID_PARAMETER_4;
	if (AdditionalReferences < 0)
		return STATUS_INVALID_PARAMETER_5;

	/*
	 * include the object header and the object storage
	 */
	objectHeader = KrEAllocateObject(ObjectSize);
	if (!objectHeader)
	{
		if (Flags & KRE_OBJECT_RAISE_ON_FAIL)
			KrERaiseStatus(STATUS_INSUFFICIENT_RESOURCES);  //  exit the app
		else
			return STATUS_INSUFFICIENT_RESOURCES;
	}
	if (ObjectType)
	{
		InterlockedIncrement(&ObjectType->NumberOfObject);
	}

	objectHeader->RefCount = 1 + AdditionalReferences;
	objectHeader->Flags = Flags;
	objectHeader->Size = ObjectSize;
	objectHeader->Type = ObjectType;

	*Object = KrEObjectHeaderToObject(objectHeader);
	return STATUS_SUCCESS;

}

PKRE_OBJECT_HEADER KrEAllocateObejct(
	__in SIZE_T ObjectSize
)
{
	return KrEAllocate(KrEAddObjectHeaderSize(ObjectSize));
}

LONG KrEDereferenceObjectEx(
	__in PVOID Object,
	__in LONG RefCount,
	__in BOOLEAN DeferDelete
)
{
	PKRE_OBJECT_HEADER objectHeader;
	LONG oldRefCount;

	if (RefCount < 0)
		KrERaiseStatus(STATUS_INVALID_PARAMETER_2);

	objectHeader = KrEObjectToObjectHeader(Object);

	oldRefCount = InterlockedExchangeAdd(&objectHeader->RefCount, -RefCount);

	if (oldRefCount - RefCount == 0)
	{
		if (DeferDelete)
			KrEDeferDeleteObject(objectHeader);
		else
			KrEFreeObject(objectHeader);
	}

	return oldRefCount - RefCount;

}

VOID KrEDeferDeleteObject(__in PKRE_OBJECT_HEADER ObjectHeader)
{
	PKRE_OBJECT_HEADER nextToFree;

	while(TRUE)
	{
		nextToFree = KrEObjectNextToFree;
		ObjectHeader->NextToFree = nextToFree;

		if (InterlockedCompareExchangePointer(
			&KrEObjectNextToFree,
			ObjectHeader,
			nextToFree) == nextToFree)
			break; // success return
	}
	if (!nextToFree)
		QueueUserWorkItem(KrEDeferDeleteObjectRoutine, NULL, 0);
}

NTSTATUS KrEDeferDeleteObjectRoutine(
	__in PVOID Parameter		// unused
)
{
	PKRE_OBJECT_HEADER objectHeader = NULL;

	while(TRUE)
	{
		objectHeader = InterlockedExchangePointer(&KrEObjectNextToFree,objectHeader);
		if (objectHeader)
		{
			KrEFreeObject(objectHeader);
			objectHeader = objectHeader->NextToFree;
		}
		else
			break;
	}
	return STATUS_SUCCESS;
}

VOID KrEFreeObject(
	__in PKRE_OBJECT_HEADER ObjectHeader
)
{
	InterlockedDecrement(ObjectHeader->Type->NumberOfObject);

	if(ObjectHeader->Type->DeleteProcedure)
	{
		ObjectHeader->Type->DeleteProcedure(
			KrEObjectHeaderToObject(ObjectHeader),
			ObjectHeader->Flags
		);
	}
	KrEFree(ObjectHeader);
}

BOOLEAN KrEDereferenceObject(__in PVOID Object)
{
	return KrEDereferenceObjectEx(Object, 1, FALSE) == 0;
}

