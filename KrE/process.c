#include <ph.h>

PKRE_OBJECT_TYPE KrEProcessItemType;
PWSTR KrEDosDeviceNames[26];

VOID KrEProcessItemDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
);

NTSTATUS KrEQueryProcessVariableSize(
	__in HANDLE ProcessHandle,
	__in PROCESS_INFORMATION_CLASS ProcessInformationClass,
	__out PPVOID Buffer
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
	__out PHANDLE TokenHandle,
	__in ACCESS_MASK DesiredAccess,
	__in HANDLE ProcessHandle
)
{
	return NtOpenProcessToken(
		ProcessHandle,
		DesiredAccess,
		TokenHandle
	);
}

BOOLEAN KrESetTokenPrivilege(
	__in HANDLE TokenHandle,
	__in_opt PWSTR PrivilegeName,
	__in_opt PLUID privilegeLuid,
	__in ULONG Attributes
)
{
	TOKEN_PRIVILEGES privileges = { 0 };

	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = Attributes;

	if (privilegeLuid)
	{
		privileges.Privileges[0].Luid = *privilegeLuid;
	}
	else if (PrivilegeName)
	{
		if (!LookupPrivilegeValue(
			NULL,
			PrivilegeName,
			&privileges.Privileges[0].Luid
		))
			return FALSE;
	}
	else
		return FALSE;
	if (!AdjustTokenPrivileges(
		TokenHandle,
		FALSE,
		&privileges,
		0,
		NULL,
		NULL
	))
		return FALSE;
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		return FALSE;
	return TRUE;
}

BOOLEAN KrELookupSid(
	__in PSID Sid,
	__out_opt PKRE_STRING *Name, 
	__out_opt PKRE_STRING *DomainName,
	__out_opt PSID_NAME_USE NameUse
)
{
	PVOID nameBuffer;
	ULONG nameBufferSize;
	PVOID domainNameBuffer;
	ULONG domainNameBufferSize;
	SID_NAME_USE nameUse;

	nameBufferSize = 0x40;
	nameBuffer = KrEAllocate(nameBufferSize * 2);
	domainNameBufferSize = 0x40;
	domainNameBuffer = KrEAllocate(domainNameBufferSize * 2);

	if (!LookupAccountSid(
		NULL,
		Sid,
		nameBuffer,
		&nameBufferSize,
		domainNameBuffer,
		&domainNameBufferSize,
		&nameUse
	))
	{
		KrEFree(nameBuffer);
		nameBuffer = KrEAllocate(nameBufferSize * 2);
		KrEFree(domainNameBuffer);
		domainNameBuffer = KrEAllocate(domainNameBufferSize*2);

		if (!LookupAccountSid(
			NULL,
			Sid,
			nameBuffer,
			&nameBufferSize,
			domainNameBuffer,
			&domainNameBufferSize,
			&nameUse
		))
		{
			KrEFree(nameBuffer);
			KrEFree(domainNameBuffer);
			return FALSE;
		}
	}
	if (Name)
		*Name = KrECreateString(nameBuffer);
	if (DomainName)
		*DomainName = KrECreateString(domainNameBuffer);
	if (NameUse)
		*NameUse = nameUse;

	KrEFree(nameBuffer);
	KrEFree(domainNameBuffer);

	return TRUE;


}

VOID KrEInitializeDosDeviceNames()
{
	ULONG i;
	for (i = 0; i < 26; i++)
		KrEDosDeviceNames[i] = KrEAllocate(64*sizeof(WCHAR));
}

VOID KrERefreshDosDeviceNames()
{
	WCHAR deviceName[3];
	ULONG i;

	deviceName[1] = ':';
	deviceName[2] = 0;

	for (i = 0; i < 26; i++)
	{
		deviceName[0] = (WCHAR)('A'+i);	//driver letter mapping
		if(KrEDosDeviceNames[i])
		{
			if (!QueryDosDevice(deviceName, KrEDosDeviceNames[i], 64))
			KrEDosDeviceNames[i][0] = 0;
		}
	}
}

PKRE_STRING KrEGetFileName(__in PKRE_STRING FileName)
{
	PKRE_STRING newFileName;
	newFileName = FileName;

	if (wcsncmp(FileName->Buffer, L"\\??\\", 4) == 0)
	{
		newFileName = KrECreateStringEx(NULL, FileName->Length - 8);
		memcpy(newFileName->Buffer, &FileName->Buffer[4], FileName->Length - 8);
	}
	else if (_wcsnicmp(FileName->Buffer, L"\\SystemRoot", 11) == 0)
	{
		PKRE_STRING systemDirectory = KrEGetSystemDirectory();
		if (systemDirectory)
		{
			ULONG indexOfLastBackslash = (ULONG)(wcsrchr(systemDirectory->Buffer, '\\') - systemDirectory->Buffer);


			newFileName = KrECreateStringEx(NULL, indexOfLastBackslash * 2 + FileName->Length - 11);
			memcpy(newFileName->Buffer, systemDirectory->Buffer, indexOfLastBackslash * 2);
			memcpy(&newFileName->Buffer[indexOfLastBackslash], &FileName->Buffer[11], FileName->Length - 22);
			KrEDereferenceObject(systemDirectory);

		}
	}
	else
    {
        ULONG i;

        for (i = 0; i < 26; i++)
        {
            PWSTR prefix = KrEDosDeviceNames[i];
            ULONG prefixLength = wcslen(prefix);

            if (prefixLength > 0)
            {
                if (_wcsnicmp(FileName->Buffer, prefix, prefixLength) == 0)
                {
                    newFileName = KrECreateStringEx(NULL, 4 + FileName->Length - prefixLength * 2);
                    newFileName->Buffer[0] = 'A' + i;
                    newFileName->Buffer[1] = ':';
                    memcpy(&newFileName->Buffer[2], &FileName->Buffer[prefixLength], FileName->Length - prefixLength * 2);

                    break;
                }
            }
        }
	}
	return newFileName;
}


NTSTATUS KrEGetProcessImageFileName(
	__in HANDLE ProcessHandle,
	__out PKRE_STRING * FileName
)
{
	NTSTATUS status;
	PVOID buffer;
	PUNICODE_STRING fileName;

	status = KrEQueryProcessVariableSize(
		ProcessHandle,
		27,
		&buffer
	);
	if (!NT_SUCCESS(status))
		return status;

	fileName = (PUNICODE_STRING)buffer;
	*FileName = KrECreateStringEx(fileName->Buffer,fileName->Length);
	return status;
}


NTSTATUS KrEQueryProcessVariableSize(
	__in HANDLE ProcessHandle,
	__in PROCESS_INFORMATION_CLASS ProcessInformationClass,
	__out PPVOID Buffer
)
{
	NTSTATUS status;
	ULONG bufferSize = 0;
	PVOID buffer = NULL;

	while(TRUE)
	{
		status = NtQueryInformationProcess(
			ProcessHandle,
			ProcessInformationClass,
			buffer,
			bufferSize,
			&bufferSize
		);

		if(
			status == STATUS_BUFFER_OVERFLOW ||
			status == STATUS_BUFFER_TOO_SMALL ||
			status == STATUS_INFO_LENGTH_MISMATCH
			)
		{
			if (buffer)
				KrEFree(buffer);
			buffer = KrEAllocate(bufferSize);
		}
		else
		{
			break;
		}
	}

	if(!NT_SUCCESS(status))
	{
		if (buffer)
			KrEFree(buffer);
		return status;
	}

	*Buffer = buffer;
	return status;
}
