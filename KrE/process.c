#include <ph.h>


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
