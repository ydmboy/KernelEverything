#ifndef PH_H
#define PH_H



#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <phbase.h>
//some definition

#define PH_MAX_MESSAGE_SIZE 400
#define KrEShowError(hWnd,Format,...) KrEShowMessage_V(hWnd,MB_OK|MB_ICONERROR,Format,__VA_ARGS__)
#define KRE_FIRST_PROCESS(Processes) ((PSYSTEM_PROCESS_INFORMATION)(Processes))
#define KRE_NEXT_PROCESS(Process) (\
		((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset ? \
		((PSYSTEM_PROCESS_INFORMATION)((PCHAR)(Process)+			\
		((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset)): \
		(VOID*)NULL)

typedef struct _KRE_PROCESS_ITEM
{
	HANDLE ProcessId;
	HANDLE ParentProcessId;
	PKRE_STRING ProcessName;
	ULONG SessionId;

	HICON SmallIcon;
	HICON LargeIcon;

	PKRE_STRING FileName;
	PKRE_STRING CommandLine;

	LARGE_INTEGER CreateTime;

	PKRE_STRING UserName;
	ULONG IntegrityLevel;
	PKRE_STRING IntegrityString;

	ULONG HasParent : 1;
	ULONG IsBeingDebugged : 1;
	ULONG IsDotNet : 1;
	ULONG IsElevated : 1;
	ULONG IsInJob : 1;
	ULONG IsInSignificantJob : 1;
	ULONG IsPacked : 1;
	ULONG IsPosix : 1;
	ULONG IsWow64 : 1;

	FLOAT CpuUsage; // from 0 to 1
} KRE_PROCESS_ITEM, * PKRE_PROCESS_ITEM;


PKRE_PROCESS_ITEM KrECreateProcessItem(
	__in HANDLE ProcessId
);

INT KrEShowMessage_V(
	__in HWND hWnd,
	__in ULONG Type,
	__in PWSTR Format,
	...
);

typedef BOOLEAN(*PPH_ENUM_PROCESSES_CALLBACK)(__in PSYSTEM_PROCESS_INFORMATION Process);


VOID KrEFree(__in PVOID Memory);

NTSTATUS KrEEnumProcesses(
	__out PPVOID Processes
);

BOOLEAN KrEInitializeProcessItem();

#endif
