#ifndef _PHBASE_H
#define _PHBASE_H



#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <ntwin.h>
#include <ntimport.h>

#define KrE_APP_NAME (L"KernerlEverything")

#define KrERaiseStatus(Status) RaiseException(Status,0,0,NULL)

extern PWSTR KrEWindowClassName;
extern HINSTANCE KrEInstanceHandle;
extern HFONT KrEApplicationFont;
extern HANDLE KrEHeapHandle;


PVOID KrEAllocate(__in SIZE_T Size);
PVOID KrEFree(__in PVOID Memory);


#endif

