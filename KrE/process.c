#include <ph.h>

PKRE_OBJECT_TYPE KrEProcessItemType;

VOID KrEProcessItemDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
);

NTSTATUS KrEEnumProcesses(
	__out PPVOID Processes
)
{
	NTSTATUS status;
	PVOID buffer;
	ULONG bufferSize = 2048;

	buffer = KrEAllocate(bufferSize);
	while (TRUE)  // it may cause the hidden bug
	{
		if (!buffer)
		{
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		status = NtQuerySystemInformation(SystemProcessInformation, buffer, bufferSize, &bufferSize);
		if (NT_SUCCESS(status))
		{
			break;
		}
		if (status == STATUS_BUFFER_TOO_SMALL || status == STATUS_INFO_LENGTH_MISMATCH)
		{
			KrEFree(buffer);
			buffer = KrEAllocate(bufferSize);
		}
		else
		{
			KrEFree(buffer);
			return status;
		}
	}

	*Processes = buffer;

	return STATUS_SUCCESS;
}


BOOLEAN KrEInitializeProcessItem()
{
	return NT_SUCCESS(KrECreateObjectType(&KrEProcessItemType,
		0,
		KrEProcessItemDeleteProcedure
	));

	
}


PKRE_PROCESS_ITEM KrECreateProcessItem(
	__in HANDLE ProcessId
)
{
	PKRE_PROCESS_ITEM processItem;
	if (!NT_SUCCESS(KrECreateObject(
		&processItem,
		sizeof(KRE_PROCESS_ITEM),
		0,
		KrEProcessItemType,
		0
	)))
		return NULL;
	memset(processItem, 0, sizeof(KRE_PROCESS_ITEM));
	processItem->ProcessId = ProcessId;

	return processItem;
}

VOID KrEProcessItemDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
	)
{
	PKRE_PROCESS_ITEM processItem = (PKRE_PROCESS_ITEM)Object;

	if (processItem->ProcessName)
		KrEDereferenceObject(processItem->ProcessName);
}

BOOLEAN KrEDereferenceObjectDeferDelete(__in PVOID Object)
{
	return KrEDereferenceObjectEx(Object,1,TRUE) == 0;
}
