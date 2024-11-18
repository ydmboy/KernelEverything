#include <phgui.h>

HWND KrEMainwindowHandle;
static HWND TabControlHandle;

static HWND ProcessListViewHandle;
static HWND ServiceListViewHandle;
static HWND NetworkListViewHandle;

static INT ProcessesTabIndex;
static INT NetworkTabIndex;
static INT NetworkTabIndex;



// rt : true is successful
BOOLEAN KrEMainWndInitialization(__in INT ShowCommand)
{
	KrEMainwindowHandle = CreateWindow(KrEWindowClassName, KrE_APP_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, KrEInstanceHandle, NULL);
	if (!KrEMainwindowHandle)
		return FALSE;


	KrEInitializeFont(KrEMainwindowHandle);

	ShowWindow(KrEMainwindowHandle, ShowCommand);

	KrEMainWndCreateTab();
	KrEMainWndLayout();

	return TRUE;

}

VOID KrEMainWndCreateTab()
{
	TabControlHandle = KrECreateTabControl(KrEMainwindowHandle);
	KrEAddTabControlTab(TabControlHandle, 0, L"Processes");
	KrEAddTabControlTab(TabControlHandle, 1, L"Services");
	KrEAddTabControlTab(TabControlHandle, 2, L"NetWork");

	ProcessListViewHandle = KrECreateListViewControl(KrEMainwindowHandle,)


}

VOID KrEMainWndLayout()
{
	RECT rect;
	GetClientRect(KrEMainwindowHandle, &rect);
	KrESetControlPosition(TabControlHandle, 0, 0, rect.right, rect.bottom);
}

LRESULT CALLBACK KrEMainWndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{

	}
	break;
	case WM_PAINT:
	{

	}
	break;
	case WM_SIZE:
	{

	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

VOID FORCEINLINE KrESetControlPosition(
	HWND Handle,
	INT Left,
	INT Top,
	INT Right,
	INT Bottom
)
{
	SetWindowPos(Handle, NULL, Left, Top, Right, Bottom, SWP_NOZORDER);
}
