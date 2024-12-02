#ifndef PH_H
#define PH_H



#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <phbase.h>
//some definition

#define PH_MAX_MESSAGE_SIZE 400
#define KrEShowError(hWnd,Format,...) KrEShowMessage_V(hWnd,MB_OK|MB_ICONERROR,Format,__VA_ARGS__)

INT KrEShowMessage_V(
	__in HWND hWnd,
	__in ULONG Type,
	__in PWSTR Format,
	...
);

typedef BOOLEAN(*PPH_ENUM_PROCESSES_CALLBACK)(__in PSYSTEM_PROCESS_INFORMATION Process);

PVOID KrEAllocate(__in SIZE_T Size);

PVOID KrEFree(__in PVOID Memory);

NTSTATUS KrEEnumProcesses(
	__in PPH_ENUM_PROCESSES_CALLBACK CallBack,
	__out_opt PBOOLEAN Found);

#endif
