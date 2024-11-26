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

	buffer = KrEAllocate(1024);
	while(TRUE)
	{
		if(!buffer)
		{
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		

	}
	 

}
