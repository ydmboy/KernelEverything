#ifndef _PHBASE_H
#define _PHBASE_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <ntbasic.h>

#include <ntstatus.h>
#include <ntimport.h>
#include <wchar.h>
	
#define KrE_APP_NAME (L"KernerlEverything")

extern PWSTR KrEWindowClassName;
extern HINSTANCE KrEInstanceHandle;
extern HFONT KrEApplicationFont;
extern HANDLE KrEHeapHandle;

typedef struct _KRE_STRING
{
	UNICODE_STRING us;
	WCHAR Buffer[1];
}KRE_STRING,*PKRE_STRING ;



#endif

