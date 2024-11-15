#pragma once
#ifndef _PHGUI_H
#define _PHGUI_H


#include <phbase.h>
#include <ph.h>
#include <CommCtrl.h>

INT KrEMainMessageLoop();
BOOLEAN KrEMainWndInitialization(__in INT ShowCommand);

VOID KrEInitializeCommonControls();

LRESULT  CALLBACK KrEMainWndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);


ATOM KrERegisterWindowClass();


HWND KrECreateTabControl(HWND ParentHandle);

INT KrEAddTabControlTab(
	HWND TabControlHandle,
	INT Index,
	PWSTR Text
);

VOID KrEMainWndCreateTab();



VOID FORCEINLINE KrESetControlPosition(
	HWND Handle,
	INT Left,
	INT Top,
	INT Right,
	INT Bottom
);

VOID KrEMainWndLayout();

#endif

