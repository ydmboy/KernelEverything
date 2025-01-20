#include <ph.h>
#include <ntstatus.h>


NTSTATUS KrEEnumProcesses(
	__out PPVOID buffer
)
{
	NTSTATUS status;
	ULONG bufferSize = 2048;

	PSYSTEM_PROCESS_INFORMATION procInfo;


	do
	{
		buffer = KrEAllocate(bufferSize);
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
		}
		else
		{
			return status;
		}
	} while (TRUE);


	return STATUS_SUCCESS;
}
