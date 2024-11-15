#define MAIN_PRIVATE
#include <phgui.h>

#include "resource.h"


HINSTANCE PhInstanceHandle;
PWSTR KrEWindowClassName = L"KernelEverything";

INT WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    INT nCmdShow
)
{
    PhInstanceHandle = hInstance;

    KrERegisterWindowClass();
    KrEInitializeCommonControls();

    //PhInitializeCommonControls();

    //if (!PhInitializeMainWindow(nCmdShow))
    //{
    //    return 1;
    //}

    //return PhMainMessageLoop();
}

VOID KrEInitializeCommonControls()
{
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
}

ATOM KrERegisterWindowClass()
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = KrEMainWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = PhInstanceHandle;
    wcex.hIcon = LoadIcon(PhInstanceHandle,MAKEINTRESOURCE(IDI_KRE));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_HIGHLIGHT + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAINWND);
    wcex.lpszClassName = KrEWindowClassName;
    wcex.hIconSm = (HICON)LoadImage(PhInstanceHandle, MAKEINTRESOURCE(IDI_KRE), IMAGE_ICON, 16, 16, 0);

    return RegisterClassEx(&wcex);

}