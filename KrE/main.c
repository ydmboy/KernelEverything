#define MAIN_PRIVATE
#include <phgui.h>

#include "resource.h"


HINSTANCE KrEInstanceHandle;
PWSTR KrEWindowClassName = L"KernelEverything";
HFONT KrEApplicationFont;




INT WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR lpCmdLine,
    INT nCmdShow
)
{
    KrEInstanceHandle = hInstance;

    KrERegisterWindowClass();
    KrEInitializeCommonControls();

    if(!KrEMainWndInitialization(nCmdShow))
    {
        return 1;
    }


    return KrEMainMessageLoop();

}


INT KrEMainMessageLoop()
{
    BOOL result;
    MSG message;
    HACCEL acceleratorTable;
    acceleratorTable = LoadAccelerators(KrEInstanceHandle, MAKEINTRESOURCE(IDR_MAINWND));

    while(result = GetMessage(&message,NULL,0,0))
    {
        if (result == -1)
            return 1;
        if(!TranslateAccelerator(message.hwnd,acceleratorTable,&message))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

    }
    return (INT)message.wParam;

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
    wcex.style = 0;
    wcex.lpfnWndProc = KrEMainWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = KrEInstanceHandle;
    wcex.hIcon = LoadIcon(KrEInstanceHandle,MAKEINTRESOURCE(IDI_KRE));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW +1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
    //wcex.lpszMenuName = NULL;
    wcex.lpszClassName = KrEWindowClassName;
    wcex.hIconSm = (HICON)LoadImage(KrEInstanceHandle, MAKEINTRESOURCE(IDI_KRE), IMAGE_ICON, 16, 16, 0);

    return RegisterClassEx(&wcex);

}

VOID KrEInitializeFont(__in HWND hWnd)
{
    if (!(KrEApplicationFont = CreateFont(
        -MulDiv(8, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72),
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        L"Segoe UI"
    )))
    {
        NONCLIENTMETRICS metrics;
        metrics.cbSize = sizeof(NONCLIENTMETRICS);

        if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &metrics, 0))
        {
            KrEApplicationFont = CreateFontIndirect(&metrics.lfMessageFont);
        }
        else
        {
            KrEApplicationFont = NULL;
        }
    }
}

