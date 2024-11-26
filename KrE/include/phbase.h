#pragma once
#ifndef _PHBASE_H
#define _PHBASE_H

#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>

#include <ntimport.h>
#include <wchar.h>

#define KrE_APP_NAME (L"KernerlEverything")

extern PWSTR KrEWindowClassName;
extern HINSTANCE KrEInstanceHandle;
extern HFONT KrEApplicationFont;
extern HANDLE KrEHeapHandle;



#endif

