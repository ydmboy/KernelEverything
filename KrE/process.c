#include <ph.h>

PKRE_OBJECT_TYPE KrEProcessItemType;

VOID KrEProcessItemDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
);

NTSTATUS KrEEnumProcesses(
	__in PPH_ENUM_PROCESSES_CALLBACK CallBack,
	__out_opt PBOOLEAN Found
)
{
	NTSTATUS status;
	PVOID buffer;
	ULONG bufferSize = 2048;
	PSYSTEM_PROCESS_INFORMATION procInfo;

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
			return status;
		}
	}

	procInfo = (PSYSTEM_PROCESS_INFORMATION)buffer;

	while(TRUE)
	{
		if(CallBack(procInfo))
		{
			if (Found)
				*Found = TRUE;
			return STATUS_SUCCESS;
		}
		CallBack("1");
		if (procInfo->NextEntryOffset == 0)
			break;
		procInfo = (PSYSTEM_PROCESS_INFORMATION)((PCHAR)procInfo+procInfo->NextEntryOffset);
	}

	KrEFree(buffer);
	if (Found)
		*Found = FALSE;
	return STATUS_SUCCESS;
}


BOOLEAN KrEInitializeProcessItem()
{
	return NT_SUCCESS(KrECreateObjectType(&KrEProcessItemType,
		0,
		KrEProcessItemDeleteProcedure
	));

	
}


BOOLEAN KrECreateProcessItem(
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
	return 
}
