#pragma once
#ifndef PH_H
#define PH_H

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

#endif
