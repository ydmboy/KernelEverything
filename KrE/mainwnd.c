#include <phgui.h>
#include <commctrl.h>
HWND KrEMainWindowHandle;
static HWND TabControlHandle;

static HWND ProcessListViewHandle;
static HWND ServiceListViewHandle;
static HWND NetworkListViewHandle;

static INT ProcessesTabIndex;
static INT ServiceTabIndex;
static INT NetworkTabIndex;



// rt : true is successful
BOOLEAN KrEMainWndInitialization(__in INT ShowCommand)
{
	KrEMainWindowHandle = CreateWindow(KrEWindowClassName, KrE_APP_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, KrEInstanceHandle, NULL);
	if (!KrEMainWindowHandle)
		return FALSE;


	KrEInitializeFont(KrEMainWindowHandle);


	KrEMainWndCreateTab();
	KrEMainWndLayout();

	ShowWindow(KrEMainWindowHandle, ShowCommand);
	return TRUE;
}

BOOLEAN ProcessEnumCallBack(PSYSTEM_PROCESS_INFORMATION Process)
{
	PWSTR buffer;
	buffer = KrEAllocate(Process->ImageName.Length + sizeof(WCHAR));
	memcpy(buffer, Process->ImageName.Buffer, Process->ImageName.Length);
	buffer[Process->ImageName.Length / sizeof(WCHAR)] = 0;

	KrEAddListViewItem(
		ProcessListViewHandle,
		MAXINT,
		buffer
	);

	


	return FALSE;  //  this is where bug is likely to occur. 
}


VOID KrEMainWndCreateTab()
{

	// To create the main tab 
	TabControlHandle = KrECreateTabControl(KrEMainWindowHandle);
	if (!TabControlHandle)
		return;

	//add  the  subtab
	KrEAddTabControlTab(TabControlHandle, 0, L"Processes");
	KrEAddTabControlTab(TabControlHandle, 1, L"Services");
	KrEAddTabControlTab(TabControlHandle, 2, L"NetWork");
	

	ProcessListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_PROCESSLV);
	ListView_SetExtendedListViewStyleEx(ProcessListViewHandle, LVS_EX_FULLROWSELECT, LVS_EX_DOUBLEBUFFER| LVS_EX_GRIDLINES,-1);


	KrEAddListViewColumn(ProcessListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"Process Name");



	ServiceListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_SERVICELV);
	ListView_SetExtendedListViewStyleEx(ServiceListViewHandle, LVS_EX_FULLROWSELECT, LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES,-1);
	KrEAddListViewColumn(ServiceListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"Service");



	NetworkListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_NETWORKLV);
	ListView_SetExtendedListViewStyle(NetworkListViewHandle, LVS_EX_FULLROWSELECT|
		LVS_EX_DOUBLEBUFFER,-1);
	KrEAddListViewColumn(NetworkListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"netWork");

	KrEEnumProcesses(ProcessEnumCallBack, NULL);

}

VOID KrEMainWndLayout()
{
	RECT rect;
	GetClientRect(KrEMainWindowHandle, &rect);
	KrESetControlPosition(TabControlHandle, rect.left, rect.top, rect.right, rect.bottom);
	KrEmainWndTabControlOnLayout();
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
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	break;
	case WM_SIZE:
	{
		KrEMainWndLayout();
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

VOID KrEEnumerateProcessList()
{
	//PVOID processes;
	//PSYSTEM_PROCESS_INFORMATION process;
	//if(!NT_SUCCESS(KrEEnumProcesses(processes)))
	//{
	//	return;
	//}
	//process = KRE_FIRST_PROCESS(processes);

	//do
	//{

	//	// add the value to the list item
	//	PKRE_PROCESS_ITEM processItem;


	//	if(process->UniqueProcessId == (HANDLE)0)
	//	{
	//		//RltInitUnicodeString();
	//	}


	//	
	//} while (process = KRE_NEXT_PROCESS(process));


}


// ** name:    KrESetControlPosition
// ** function:set all kinds of the window's the position include the Tab
VOID FORCEINLINE KrESetControlPosition(
	HWND Handle,
	INT Left,
	INT Top,
	INT Right,
	INT Bottom
)
{
	SetWindowPos(Handle, NULL, Left, Top, Right-Left, Bottom-Top, SWP_NOACTIVATE | SWP_NOREDRAW |SWP_NOZORDER);
}

VOID KrEmainWndTabControlOnLayout()
{
	RECT rect;
	INT selectedIndex;

	GetClientRect(KrEMainWindowHandle, &rect);
	TabCtrl_AdjustRect(TabControlHandle, FALSE, &rect);

	selectedIndex = TabCtrl_GetCurSel(TabControlHandle);

	if(selectedIndex == ProcessesTabIndex)
	{
		KrESetControlPosition(ProcessListViewHandle, rect.left, rect.top, rect.right, rect.bottom);
	}
	else if(selectedIndex == ServiceTabIndex)
	{
		KrESetControlPosition(ServiceListViewHandle, rect.left, rect.top, rect.right, rect.bottom);
	}
	else if(selectedIndex == NetworkTabIndex)
	{
			KrESetControlPosition(NetworkListViewHandle, rect.left, rect.top, rect.right, rect.bottom);
	}
}


