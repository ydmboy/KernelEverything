#include <refp.h> 


PKRE_OBJECT_TYPE g_KrEObjectTypeObject = NULL;

NTSTATUS KrEInitializeRef()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = KrECreateObjectType(&g_KrEObjectTypeObject,0,NULL);
	if (!NT_SUCCESS(status))
		return status;

	KrEObjectToObjectHeader(g_KrEObjectTypeObject)->Type = g_KrEObjectTypeObject;
	g_KrEObjectTypeObject->NumberOfObject = 1;

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
		g_KrEObjectTypeObject,
		0	
	);
	if (!NT_SUCCESS(status))
		return status;

	objectType->Flags = Flags;
	objectType->DeleteProcedure = DeleteProcedure;
	objectType->NumberOfObject = 0;

	*ObjectType = objectType;

	return status;
}

NTSTATUS KrECreateObject(
	__out PVOID* Object,
	__in SIZE_T ObjectSize,
	__in ULONG Flags,
	__in_opt PKRE_OBJECT_TYPE ObjectType,
	__in_opt LONG AdditionalReferences
)
{
	PKRE_OBJECT_HEADER objectHeader;

	// 1 | 0  allow passeage 
	if ((Flags & KRE_OBJECT_VALID_FLAGS) != Flags)
		return STATUS_INVALID_PARAMETER_3;
	if (!ObjectType && g_KrEObjectTypeObject)
		return STATUS_INVALID_PARAMETER_4;
	if (AdditionalReferences < 0)
		return STATUS_INVALID_PARAMETER_5;
	objectHeader = KrEAllocateObject(ObjectSize);
	if(!objectHeader)
	{
		if (Flags & KRE_OBJECT_RAISE_ON_FAIL)
			KrERaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
		else
			return STATUS_INSUFFICIENT_RESOURCES;
	}
	if (ObjectType)	// gg variable
		InterlockedIncrement(&ObjectType->NumberOfObject);

	objectHeader->RefCount = 1 + AdditionalReferences;
	objectHeader->Flags = Flags;
	objectHeader->Size = ObjectSize;
	objectHeader->Type = ObjectType;

	*Object = KrEObjectHeaderToObject(objectHeader);
}

PKRE_OBJECT_HEADER KrEAllocateObject(
	__in SIZE_T ObjectSize
)
{
	return KrEAllocate(KrEAddObjectHeaderSize(ObjectSize));
}


NTSTATUS KrECreateObject(
	__out PVOID * Object,
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
	if(!objectHeader)
	{
		if (Flags & KRE_OBJECT_RAISE_ON_FAIL)
			KrERaiseStatus(STATUS_INSUFFICIENT_RESOURCES);  //  exit the app
		else
			return STATUS_INSUFFICIENT_RESOURCES;
	}
	if(ObjectType)
	{
		InterlockedIncrement(&ObjectType->NumberOfObject);
	}

	objectHeader->RefCount = 1 + AdditionalReferences;
	objectHeader->Flags = Flags;
	objectHeader->Size = ObjectSize;
	objectHeader->Type = ObjectType;

	*Object = KrE



}

PKRE_OBJECT_HEADER KrEAllocateObejct(
	__in SIZE_T ObjectSize
)
{
	return KrEAllocate(KrEAddObjectHeaderSize(ObjectSize));
}



BOOLEAN KrEDereferenceObject(__in PVOID Object)
{
	return KrEDereferenceObjectEx();
}

LONG KrEDereferenceObjectEx(
	__in PVOID Object,
	__in LONG RefCount,
	__in BOOLEAN DeferDelete
)
{
	KRE_OBJECT_HEADER objectHeader;
	LONG oldRefCount;

	if (RefCount < 0)
		KrERaiseStatus(STATUS_INVALID_PARAMETER_2);
	o


}


