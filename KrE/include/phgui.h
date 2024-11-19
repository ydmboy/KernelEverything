#pragma once
#ifndef _PHGUI_H
#define _PHGUI_H


#include <phbase.h>
#include <ph.h>
#include <CommCtrl.h>
#include "resource.h"

INT KrEMainMessageLoop();
BOOLEAN KrEMainWndInitialization(__in INT ShowCommand);


VOID KrEInitializeFont(__in HWND hWnd);

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


// layt out 
VOID KrEMainWndLayout();
VOID KrEmainWndTabControlOnLayout();



HWND KrECreateListViewControl(
	HWND ParentHandle,
	INT_PTR Id
);


INT KrEAddListViewColumn(
	HWND ListViewHandle,
	INT Index,
	INT DisplayIndex,
	INT SubItemIndex,
	INT Format,
	INT Width,
	PWSTR Text
);

#endif

