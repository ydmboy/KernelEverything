#include <phgui.h>
#include <CommCtrl.h>
#include <wchar.h>

HWND KrEMainWindowHandle;
static HWND TabControlHandle;

static HWND ProcessListViewHandle;
static HWND ServiceListViewHandle;
static HWND NetworkListViewHandle;

static INT ProcessesTabIndex;
static INT ServiceTabIndex;
static INT NetworkTabIndex;


VOID KrEmainWndTabControlOnSelectionChanged();
VOID KrEMainWndControlOnNotify(__in LPNMHDR Header);
VOID FillProcessInfo(
	__inout PKRE_PROCESS_ITEM ProcessItem);

BOOLEAN KrEMainWndInitialization(__in INT ShowCommand)
{

	{
		HANDLE tokenHandle;
		if (NT_SUCCESS(KrEOpenProcessToken(
			&tokenHandle,
			TOKEN_ADJUST_PRIVILEGES,
			NtCurrentProcess()
		)))
		{
			KrESetTokenPrivilege(tokenHandle, L"SeDebugPrivilege", NULL, SE_PRIVILEGE_ENABLED);
		}
	}
	KrEMainWindowHandle = CreateWindow(KrEWindowClassName, KrE_APP_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, KrEInstanceHandle, NULL);
	if (!KrEMainWindowHandle)
		return FALSE;

	KrEInitializeFont(KrEMainWindowHandle);

	KrEMainWndCreateTab();
	KrEMainWndLayout();

	ShowWindow(KrEMainWindowHandle, ShowCommand);
	return TRUE;
}

VOID EnumerateProcesses()
{
	PVOID processes;
	PSYSTEM_PROCESS_INFORMATION process;

	if (!NT_SUCCESS(KrEEnumProcesses(&processes)))
		return;

	process = KRE_FIRST_PROCESS(processes);

	do
	{
		PKRE_PROCESS_ITEM processItem;
		INT lvItemIndex;
		if (process->UniqueProcessId == (HANDLE)0)
			RtlInitUnicodeString(&process->ImageName, L"System Idle Process");
		processItem = KrECreateProcessItem(process->UniqueProcessId);
		processItem->ProcessName = KrECreateStringEx(process->ImageName.Buffer, process->ImageName.Length);
		_snwprintf_s(processItem->ProcessIdString, KRE_INT_STR_LEN_1, KRE_INT_STR_LEN, L"%d", (int)processItem->ProcessId);


		// add the FillProcess
		FillProcessInfo(processItem);

		lvItemIndex = KrEAddListViewItem(
			ProcessListViewHandle,
			MAXINT,
			processItem->ProcessName->Buffer,
			processItem
		);

		// set the item value ;
		KrESetListViewSubItem(ProcessListViewHandle,lvItemIndex,1,processItem->ProcessIdString);
		KrESetListViewSubItem(ProcessListViewHandle,lvItemIndex,2,KrEGetString(processItem->UserName));
		KrESetListViewSubItem(ProcessListViewHandle,lvItemIndex,3,KrEGetString(processItem->FileName));
		KrESetListViewSubItem(ProcessListViewHandle, lvItemIndex, 4, KrEGetString(processItem->CommandLine));
		//KrESetListViewSubItem(ProcessListViewHandle,lvItemIndex,3,"123");
	} while (process = KRE_NEXT_PROCESS(process));
	KrEFree(processes);
}


VOID KrEMainWndCreateTab()
{

	// To create the main tab 
	TabControlHandle = KrECreateTabControl(KrEMainWindowHandle);
	if (!TabControlHandle)
		return;

	//add  the  subtab

	ProcessesTabIndex = KrEAddTabControlTab(TabControlHandle, 0, L"Processes");
	ServiceTabIndex = KrEAddTabControlTab(TabControlHandle, 1, L"Services");
	NetworkTabIndex = KrEAddTabControlTab(TabControlHandle, 2, L"NetWork");


	// First Tab of the process
	ProcessListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_PROCESSLV);
	ListView_SetExtendedListViewStyleEx(ProcessListViewHandle, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES, -1);
	KrEAddListViewColumn(ProcessListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"Name");
	KrEAddListViewColumn(ProcessListViewHandle, 1, 1, 1, LVCFMT_LEFT, 80, L"PID");
	KrEAddListViewColumn(ProcessListViewHandle, 2, 2, 2, LVCFMT_LEFT, 80, L"UserName");
	KrEAddListViewColumn(ProcessListViewHandle, 3,3, 3, LVCFMT_LEFT, 200, L"FileName");




	ServiceListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_SERVICELV);
	ListView_SetExtendedListViewStyleEx(ServiceListViewHandle, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES, -1);
	KrEAddListViewColumn(ServiceListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"Service");

	NetworkListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_NETWORKLV);
	ListView_SetExtendedListViewStyleEx(NetworkListViewHandle, LVS_EX_FULLROWSELECT |
		LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES , -1);


	//KrEAddListViewColumn(NetworkListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"NetWork");
	//KrEAddListViewColumn(ProcessListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"name");
	//KrEAddListViewColumn(ProcessListViewHandle, 1, 1, 1, LVCFMT_LEFT, 80, L"pid");




	EnumerateProcesses();
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
			//HDC hdc;
			//PAINTSTRUCT paintStruct;
			//hdc = BeginPaint(hWnd, &paintStruct);
			////TextOut(hdc, 10, 10, L"Hello, Windows!", 15);
			//EndPaint(hWnd,&paintStruct);
			//return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
		case WM_SIZE:
		{
			KrEMainWndLayout();
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
		case WM_NOTIFY:
		{
			LPNMHDR header = (LPNMHDR)lParam;
			if (header->hwndFrom == TabControlHandle)
				KrEMainWndControlOnNotify(header);
		}
		break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

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
	SetWindowPos(Handle, NULL, Left, Top, Right - Left, Bottom - Top, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOZORDER);
}

VOID KrEmainWndTabControlOnLayout()
{
	RECT rect;
	INT selectedIndex;

	GetClientRect(KrEMainWindowHandle, &rect);
	TabCtrl_AdjustRect(TabControlHandle, FALSE, &rect);

	selectedIndex = TabCtrl_GetCurSel(TabControlHandle);

	if (selectedIndex == ProcessesTabIndex)
	{
		KrESetControlPosition(ProcessListViewHandle, rect.left, rect.top, rect.right, rect.bottom);
	}
	else if (selectedIndex == ServiceTabIndex)
	{
		KrESetControlPosition(ServiceListViewHandle, rect.left, rect.top, rect.right, rect.bottom);
	}
	else if (selectedIndex == NetworkTabIndex)
	{
		KrESetControlPosition(NetworkListViewHandle, rect.left, rect.top, rect.right, rect.bottom);
	}
}

VOID KrEMainWndControlOnNotify(
	__in LPNMHDR Header
)
{
	if (Header->code == TCN_SELCHANGE)
		KrEmainWndTabControlOnSelectionChanged();
}

VOID KrEmainWndTabControlOnSelectionChanged()
{
	INT selectedIndex = TabCtrl_GetCurSel(TabControlHandle);
	ShowWindow(ProcessListViewHandle, selectedIndex == ProcessesTabIndex ? SW_SHOW : SW_HIDE);
	ShowWindow(ServiceListViewHandle, selectedIndex == ServiceTabIndex ? SW_SHOW : SW_HIDE);
	ShowWindow(NetworkListViewHandle, selectedIndex == NetworkTabIndex ? SW_SHOW : SW_HIDE);
	KrEmainWndTabControlOnLayout();
}


VOID FillProcessInfo(
	__inout PKRE_PROCESS_ITEM ProcessItem)
{
	NTSTATUS status;
	HANDLE	processHandle;
	status = KrEOpenProcess(&processHandle,PROCESS_QUERY_INFORMATION,ProcessItem->ProcessId);

	if (!NT_SUCCESS(status))
		return;

	PKRE_STRING fileName;

	status = KrEGetProcessImageFileName(
		processHandle,
		&fileName
	);
	if(NT_SUCCESS(status))
	{
		PKRE_STRING newFileName;

		newFileName = KrEGetFileName(fileName);
		KrESwapReference(&ProcessItem->FileName, newFileName);

		KrEDereferenceObject(fileName);
		KrEDereferenceObject(newFileName);
	}


	HANDLE processHandle2;
	status = KrEOpenProcess(
		&processHandle2,
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		ProcessItem->ProcessId
	);
	if(NT_SUCCESS(status))
	{
		PKRE_STRING commandLine;
		status = KrEGetProcessCommandLine(processHandle2,&commandLine);
	}

	HANDLE tokenHandle;
	status = KrEOpenProcessToken(&tokenHandle, TOKEN_QUERY, processHandle);

	if(NT_SUCCESS(status))
	{
		PTOKEN_USER user;
		status = KrEGetTokenUser(tokenHandle, &user);
		if(NT_SUCCESS(status))
		{
			PKRE_STRING userName;
			PKRE_STRING domainName;

			if(KrELookupSid(user->User.Sid,&userName,&domainName,NULL))
			{
				PKRE_STRING fullName;
				fullName = KrECreateStringEx(NULL, domainName->Length + 2 + userName->Length);
				memcpy(fullName->Buffer, domainName->Buffer, domainName->Length);
				fullName->Buffer[domainName->Length / 2] = L"\\";
				memcpy(&fullName->Buffer[domainName->Length / 2 + 1], userName->Buffer, userName->Length);

				KrESwapReference(&ProcessItem->UserName, fullName);

				KrEDereferenceObject(userName);
				KrEDereferenceObject(domainName);
				KrEDereferenceObject(fullName);
			}
			KrEFree(user);

		}
		if (tokenHandle)
			CloseHandle(tokenHandle);

	}

	if (tokenHandle)
		CloseHandle(processHandle);
}


