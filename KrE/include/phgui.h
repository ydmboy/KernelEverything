#pragma once
#ifndef _PHGUI_H
#define _PHGUI_H

#include <ph.h>
#include <CommCtrl.h>



VOID KrEInitializeCommonControls();


LRESULT CALLBACK KrEMainWndProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

ATOM KrERegisterWindowClass();


#endif

