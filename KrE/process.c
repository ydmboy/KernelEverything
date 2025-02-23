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

NTSTATUS KrEOpenProcess(
	__out PHANDLE ProcessHandle,
	__in ACCESS_MASK DesiredAccess,
	__in HANDLE ProcessId
)
{
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	CLIENT_ID clientId;

	clientId.UniqueProcess = ProcessId;
	clientId.UniqueThread = NULL;

	return NtOpenProcess(
		ProcessHandle,
		DesiredAccess,
		&objectAttributes,
		&clientId
	);
}

NTSTATUS KrEOpenThread(
	__out PHANDLE ThreadHandle,
	__in ACCESS_MASK DesiredAccess,
	__in HANDLE ThreadId
)
{
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	CLIENT_ID clientId;

	clientId.UniqueProcess = NULL;
	clientId.UniqueThread = ThreadId;

	return NtOpenThread(
		ThreadHandle,
		DesiredAccess,
		&objectAttributes,
		&clientId);
}

NTSTATUS KrEGetTokenUser(
	__in HANDLE TokenHandle,
	__out PTOKEN_USER* User
)
{
	NTSTATUS status;
	PTOKEN_USER user;
	ULONG returnLength;

	status = NtQueryInformationToken(
		TokenHandle,
		TokenUser,//typedef enum _TOKEN_INFORMATION_CLASS ->    TokenUser = 1,
		NULL,
		0,
		&returnLength);
	user = KrEAllocate(returnLength);
	status = NtQueryInformationToken(
		TokenHandle,
		TokenUser,//typedef enum _TOKEN_INFORMATION_CLASS ->    TokenUser = 1,
		user,
		returnLength,
		&returnLength);

	if (NT_SUCCESS(status))
		*User = user;
	else
	{
		if (user)
			KrEFree(user);
	}
	return status;
}

NTSTATUS KrEOpenProcessToken(
	__out PHANDLE 
)
{

}

