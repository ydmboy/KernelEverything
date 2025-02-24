
C:\code\KernelEverything>type "C:\code\KernelEverything\combined_output.c" 

C:\code\KernelEverything>type "C:\code\KernelEverything\combined_output.c" 

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\basesup.c" 
#define BASESUP_PRIVATE	
#include <phbase.h>

PKRE_OBJECT_TYPE KrEStringType;

BOOLEAN KrEInitializeBase()
{
	return NT_SUCCESS(KrECreateObjectType(
		&KrEStringType,
		0,
		NULL
		));
}

 PVOID KrEAllocate(__in SIZE_T Size)
{
	return RtlAllocateHeap(KrEHeapHandle, 0, Size);
}

VOID KrEFree(__in PVOID Memory)
{
	return; RtlFreeHeap(KrEHeapHandle, 0, Memory);
}

PKRE_STRING KrECreateStringEx(
	__in PWSTR Buffer,
	__in SIZE_T Length
)
{
	PKRE_STRING string;

	if (!NT_SUCCESS(KrECreateObject(
		&string,
		FIELD_OFFSET(KRE_STRING, Buffer) + Length + sizeof(WCHAR),
		0,
		KrEStringType,
		0
	)))
		return NULL;
	string->us.MaximumLength = string->us.Length = (USHORT)Length;
	string->us.Buffer = string->Buffer;
	memcpy(string->Buffer, Buffer, Length);
	string->Buffer[Length / sizeof(WCHAR)] = 0;

	return string;


}

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\guisup.c" 
#include <phgui.h>

#define TCM_SETBKCOLOR          (TCM_FIRST + 1)
HWND KrECreateTabControl(HWND ParentHandle)
{
	HWND tabControlHandle = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE , 0, 0, 1, 1, ParentHandle, NULL, KrEInstanceHandle, NULL);

	COLORREF bgColor = RGB(200, 200, 255); // 淡蓝色
	SendMessage(tabControlHandle, CCM_SETBKCOLOR, 0, (LPARAM)(COLORREF)(bgColor));


	//HWND tabControlHandle = NULL;
	// set the tab fonts
	if(tabControlHandle)
	{
		SendMessage(tabControlHandle,WM_SETFONT,(WPARAM)KrEApplicationFont,FALSE);

	}
	return tabControlHandle;
}

INT KrEAddTabControlTab(
	HWND TabControlHandle,
	INT Index,
	PWSTR Text
)
{
	TCITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = Text;
	return TabCtrl_InsertItem(TabControlHandle, Index, &item);
}

// To Create the view for the tab
HWND KrECreateListViewControl(
	HWND ParentHandle,
	INT_PTR Id
)
{
	return CreateWindow(WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | WS_VISIBLE | WS_BORDER, 0, 0, 1, 1, ParentHandle, (HMENU)Id, KrEInstanceHandle, NULL);
}


INT KrEAddListViewColumn(
	HWND ListViewHandle,
	INT Index,
	INT DisplayIndex,
	INT SubItemIndex,
	INT Format,
	INT Width,
	PWSTR Text
)
{
	LVCOLUMN column;
	column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_ORDER;
	column.fmt = Format;
	column.cx = Width;
	column.pszText = Text;
	column.iSubItem = SubItemIndex;
	column.iOrder = DisplayIndex;

	return ListView_InsertColumn(ListViewHandle, Index, &column);
}


INT KrEAddListViewItem(
	HWND ListViewHandle,
	INT Index,
	PWSTR Text,
	PVOID Param

)
{
	LVITEM item;
	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.iItem = Index;
	item.iSubItem = 0;
	item.pszText = Text;
	item.lParam = (LPARAM)Param;
	return ListView_InsertItem(ListViewHandle, &item);


}
C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\importLib.c" 

#pragma comment(lib,"comctl32.lib")

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\main.c" 
#define MAIN_PRIVATE
#include <phgui.h>

#include "resource.h"


HINSTANCE KrEInstanceHandle;
PWSTR KrEWindowClassName = L"KernelEverything";
HFONT KrEApplicationFont;
HANDLE KrEHeapHandle;




INT WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR lpCmdLine,
    INT nCmdShow
)
{
    KrEInstanceHandle = hInstance;
    KrEHeapHandle = HeapCreate(0,0,0);

    if(!KrEHeapHandle)
    {
        return 1;
    }

    KrERegisterWindowClass();
    KrEInitializeCommonControls();

    KrEInitializeImport();

    KrEInitializeSystem();

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
BOOLEAN KrEInitializeSystem()
{
    if (!NT_SUCCESS(KrEInitializeRef()))
        return FALSE;
    if (!KrEInitializeBase())
        return FALSE;
    if (!KrEInitializeProcessItem())
        return FALSE;
    return TRUE;
}

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\mainwnd.c" 
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


VOID KrEmainWndTabControlOnSelectionChanged();
VOID KrEMainWndControlOnNotify(__in LPNMHDR Header);

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
		if (process->UniqueProcessId == (HANDLE)0)
			RtlInitUnicodeString(&process->ImageName, L"System Idle Process");
		processItem = KrECreateProcessItem(process->UniqueProcessId);
		processItem->ProcessName = KrECreateStringEx(process->ImageName.Buffer, process->ImageName.Length);

		KrEAddListViewItem(
			ProcessListViewHandle,
			MAXINT,
			processItem->ProcessName->Buffer,
			processItem
		);

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

	ProcessListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_PROCESSLV);
	ListView_SetExtendedListViewStyleEx(ProcessListViewHandle, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES, -1);
	KrEAddListViewColumn(ProcessListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"Process Name");
	ShowWindow(ProcessListViewHandle, SW_HIDE);

	ServiceListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_SERVICELV);
	ListView_SetExtendedListViewStyleEx(ServiceListViewHandle, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES, -1);
	KrEAddListViewColumn(ServiceListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"Service");

	NetworkListViewHandle = KrECreateListViewControl(KrEMainWindowHandle, ID_MAINWND_NETWORKLV);
	ListView_SetExtendedListViewStyleEx(NetworkListViewHandle, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES , -1);
	KrEAddListViewColumn(NetworkListViewHandle, 0, 0, 0, LVCFMT_LEFT, 100, L"NetWork");
	ShowWindow(NetworkListViewHandle, SW_NORMAL);
	//EnumerateProcesses();
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
			HDC hdc;
			PAINTSTRUCT paintStruct;
			hdc = BeginPaint(hWnd, &paintStruct);
			//TextOut(hdc, 10, 10, L"Hello, Windows!", 15);
			EndPaint(hWnd,&paintStruct);
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
}


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\ntimport.c" 

#define NTIMPORT_PRIVATE
#include <ntimport.h>
#include <ph.h>


#define GetProc(DllName,ProcName) GetProcAddress(GetModuleHandle(L##DllName),(ProcName))
#define InitProc(DllName,ProcName) ((ProcName) = (_##ProcName)GetProc(DllName,#ProcName))
#define InitProcReq(DllName, ProcName)                                \
    if (!InitProc(DllName, ProcName))                                 \
    {                                                                 \
        KrEShowError(                                                 \
            NULL,                                                     \
            L"Process Hacker cannot run on your operating system. "   \
            L"Unable to find %S",                                     \
            ProcName,                                                 \
            DllName                                                   \
        );                                                            \
        return FALSE;                                                 \
    }

BOOLEAN KrEInitializeImport()
{
	InitProcReq("ntdll.dll", NtAlertResumeThread);
    InitProcReq("ntdll.dll", NtAlertThread);
    InitProcReq("ntdll.dll", NtClose);
    InitProcReq("ntdll.dll", NtDuplicateObject);
    InitProc("ntdll.dll", NtGetNextProcess);
    InitProc("ntdll.dll", NtGetNextThread);
    InitProcReq("ntdll.dll", NtOpenProcess);
    InitProcReq("ntdll.dll", NtOpenThread);
    InitProcReq("ntdll.dll", NtQueryInformationProcess);
    InitProcReq("ntdll.dll", NtQueryInformationThread);
    InitProcReq("ntdll.dll", NtQueryObject);
    InitProcReq("ntdll.dll", NtQuerySystemInformation);
    InitProcReq("ntdll.dll", NtQueueApcThread);
    InitProcReq("ntdll.dll", NtResumeProcess);
    InitProcReq("ntdll.dll", NtResumeThread);
    InitProcReq("ntdll.dll", NtSetInformationProcess);
    InitProcReq("ntdll.dll", NtSetInformationThread);
    InitProcReq("ntdll.dll", NtSuspendProcess);
    InitProcReq("ntdll.dll", NtSuspendThread);
    InitProcReq("ntdll.dll", NtTerminateProcess);
    InitProcReq("ntdll.dll", NtTerminateThread);

    return TRUE;
}



C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\process.c" 
#include <ph.h>

PKRE_OBJECT_TYPE KrEProcessItemType;

VOID KrEProcessItemDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
);

NTSTATUS KrEEnumProcesses(
	__out PPVOID Processes
)
{
	NTSTATUS status;
	PVOID buffer;
	ULONG bufferSize = 2048;

	buffer = KrEAllocate(bufferSize);
	while (TRUE)  // it may cause the hidden bug
	{
		if (!buffer)
		{
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		status = NtQuerySystemInformation(SystemProcessInformation, buffer, bufferSize, &bufferSize);
		if (NT_SUCCESS(status))
		{
			break;
		}
		if (status == STATUS_BUFFER_TOO_SMALL || status == STATUS_INFO_LENGTH_MISMATCH)
		{
			KrEFree(buffer);
			buffer = KrEAllocate(bufferSize);
		}
		else
		{
			KrEFree(buffer);
			return status;
		}
	}

	*Processes = buffer;

	return STATUS_SUCCESS;
}


BOOLEAN KrEInitializeProcessItem()
{
	return NT_SUCCESS(KrECreateObjectType(&KrEProcessItemType,
		0,
		KrEProcessItemDeleteProcedure
	));

	
}


PKRE_PROCESS_ITEM KrECreateProcessItem(
	__in HANDLE ProcessId
)
{
	PKRE_PROCESS_ITEM processItem;
	if (!NT_SUCCESS(KrECreateObject(
		&processItem,
		sizeof(KRE_PROCESS_ITEM),
		0,
		KrEProcessItemType,
		0
	)))
		return NULL;
	memset(processItem, 0, sizeof(KRE_PROCESS_ITEM));
	processItem->ProcessId = ProcessId;

	return processItem;
}

VOID KrEProcessItemDeleteProcedure(
	__in PVOID Object,
	__in ULONG Flags
	)
{
	PKRE_PROCESS_ITEM processItem = (PKRE_PROCESS_ITEM)Object;

	if (processItem->ProcessName)
		KrEDereferenceObject(processItem->ProcessName);
}

BOOLEAN KrEDereferenceObjectDeferDelete(__in PVOID Object)
{
	return KrEDereferenceObjectEx(Object,1,TRUE) == 0;
}

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\ref.c" 
#include <refp.h> 


PKRE_OBJECT_TYPE KrEObjectTypeObject = NULL;
PKRE_OBJECT_HEADER KrEObjectNextToFree = NULL;

NTSTATUS KrEInitializeRef()
{
	NTSTATUS status = STATUS_SUCCESS;
	status = KrECreateObjectType(
		&KrEObjectTypeObject,
		0,
		NULL
	);

	if (!NT_SUCCESS(status))
		return status;

	KrEObjectToObjectHeader(KrEObjectTypeObject)->Type = KrEObjectTypeObject;
	KrEObjectTypeObject->NumberOfObject = 1;
	return status;

}


NTSTATUS KrECreateObjectType(
	__out PKRE_OBJECT_TYPE* ObjectType,
	__in ULONG Flags,	// Is Flags parameter is useful??
	__in PKRE_TYPE_DELETE_PROCEDURE DeleteProcedure
)
{
	NTSTATUS status = STATUS_SUCCESS;
	PKRE_OBJECT_TYPE objectType;
	if ((Flags & KRE_OBJECT_VALID_FLAGS) != Flags)
		return STATUS_INVALID_PARAMETER_3;
	status = KrECreateObject(
		&objectType,
		sizeof(KRE_OBJECT_TYPE),
		0,
		KrEObjectTypeObject,
		0
	);
	if (!NT_SUCCESS(status))
		return status;

	objectType->Flags = Flags;
	objectType->DeleteProcedure = DeleteProcedure;
	objectType->NumberOfObject = 0;

	*ObjectType = objectType;

	return status;
}

PKRE_OBJECT_HEADER KrEAllocateObject(
	__in SIZE_T ObjectSize
)
{
	return KrEAllocate(KrEAddObjectHeaderSize(ObjectSize));
}


NTSTATUS KrECreateObject(
	__out PVOID* Object,
	__in  SIZE_T ObjectSize,
	__in  ULONG Flags,
	__in_opt PKRE_OBJECT_TYPE ObjectType,
	__in_opt LONG AdditionalReferences
)
{
	PKRE_OBJECT_HEADER objectHeader;


	/*
	 *	Flags = 0x00000001;  // 合法
	 *	if ((Flags & 0x00000001) != Flags)  // 0x00000001 == 0x00000001，不触发 if 语句
	 *	Flags = 0x00000000;  // 合法
	 */
	if ((Flags & KRE_OBJECT_VALID_FLAGS) != Flags)
		return STATUS_INVALID_PARAMETER_3;
	if (!ObjectType && KrEObjectTypeObject)
		return STATUS_INVALID_PARAMETER_4;
	if (AdditionalReferences < 0)
		return STATUS_INVALID_PARAMETER_5;

	/*
	 * include the object header and the object storage
	 */
	objectHeader = KrEAllocateObject(ObjectSize);
	if (!objectHeader)
	{
		if (Flags & KRE_OBJECT_RAISE_ON_FAIL)
			KrERaiseStatus(STATUS_INSUFFICIENT_RESOURCES);  //  exit the app
		else
			return STATUS_INSUFFICIENT_RESOURCES;
	}
	if (ObjectType)
	{
		InterlockedIncrement(&ObjectType->NumberOfObject);
	}

	objectHeader->RefCount = 1 + AdditionalReferences;
	objectHeader->Flags = Flags;
	objectHeader->Size = ObjectSize;
	objectHeader->Type = ObjectType;

	*Object = KrEObjectHeaderToObject(objectHeader);
	return STATUS_SUCCESS;

}

PKRE_OBJECT_HEADER KrEAllocateObejct(
	__in SIZE_T ObjectSize
)
{
	return KrEAllocate(KrEAddObjectHeaderSize(ObjectSize));
}

LONG KrEDereferenceObjectEx(
	__in PVOID Object,
	__in LONG RefCount,
	__in BOOLEAN DeferDelete
)
{
	PKRE_OBJECT_HEADER objectHeader;
	LONG oldRefCount;

	if (RefCount < 0)
		KrERaiseStatus(STATUS_INVALID_PARAMETER_2);

	objectHeader = KrEObjectToObjectHeader(Object);

	oldRefCount = InterlockedExchangeAdd(&objectHeader->RefCount, -RefCount);

	if (oldRefCount - RefCount == 0)
	{
		if (DeferDelete)
			KrEDeferDeleteObject(objectHeader);
		else
			KrEFreeObject(objectHeader);
	}

	return oldRefCount - RefCount;

}

VOID KrEDeferDeleteObject(__in PKRE_OBJECT_HEADER ObjectHeader)
{
	PKRE_OBJECT_HEADER nextToFree;

	while(TRUE)
	{
		nextToFree = KrEObjectNextToFree;
		ObjectHeader->NextToFree = nextToFree;

		if (InterlockedCompareExchangePointer(
			&KrEObjectNextToFree,
			ObjectHeader,
			nextToFree) == nextToFree)
			break; // success return
	}
	if (!nextToFree)
		QueueUserWorkItem(KrEDeferDeleteObjectRoutine, NULL, 0);
}

NTSTATUS KrEDeferDeleteObjectRoutine(
	__in PVOID Parameter		// unused
)
{
	PKRE_OBJECT_HEADER objectHeader = NULL;

	while(TRUE)
	{
		objectHeader = InterlockedExchangePointer(&KrEObjectNextToFree,objectHeader);
		if (objectHeader)
		{
			KrEFreeObject(objectHeader);
			objectHeader = objectHeader->NextToFree;
		}
		else
			break;
	}
	return STATUS_SUCCESS;
}

VOID KrEFreeObject(
	__in PKRE_OBJECT_HEADER ObjectHeader
)
{
	InterlockedDecrement(&ObjectHeader->Type->NumberOfObject);

	if(ObjectHeader->Type->DeleteProcedure)
	{
		ObjectHeader->Type->DeleteProcedure(
			KrEObjectHeaderToObject(ObjectHeader),
			ObjectHeader->Flags
		);
	}
	KrEFree(ObjectHeader);
}

BOOLEAN KrEDereferenceObject(__in PVOID Object)
{
	return KrEDereferenceObjectEx(Object, 1, FALSE) == 0;
}


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\support.c" 
#include <phgui.h>
#include <wchar.h>


INT KrEShowMessage_V(
	__in HWND hWnd,
	__in ULONG Type,
	__in PWSTR Format,
	...
)
{
	INT result;
	WCHAR message[PH_MAX_MESSAGE_SIZE];
	va_list ArgPtr;
	va_start(ArgPtr, Format);
	result = vswprintf(message, PH_MAX_MESSAGE_SIZE, Format, ArgPtr);
	va_end(ArgPtr);
	if (result == -1)
		return -1;
	return MessageBox(hWnd,message,KrE_APP_NAME,Type);

}





C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\resource.h" 
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 ProcessHacker.rc 使用
//
#define IDI_KRE                         101
#define IDR_MENU                        102
#define IDR_MAINWND                     103
#define IDI_ICON                        104
#define IDR_MENU1                       104
#define ID_MAINWND_PROCESSLV            2001
#define ID_MAINWND_SERVICELV            2002
#define ID_MAINWND_NETWORKLV            2003

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        105
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntbasic.h" 
#ifndef _NTBASIC_H
#define	_NTBASIC_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <ntwin.h>
// basic types

typedef PVOID* PPVOID;
typedef ULONG LOGICAL;
typedef LONG KPRIORITY;

typedef struct _QUAD
{
	double DoNotUseThisField;
}QUAD,*PQUAD,UQUAD,*PUQUAD;

#define NT_SUCCESS(Status)  (((NTSTATUS)(Status))>=0)



// Sychronization enumerations

typedef enum _WAIT_TYPE
{
	WaitAll,
	WaitAny
} WAIT_TYPE;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
}UNICODE_STRING,*PUNICODE_STRING;


typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING Object;
	ULONG Atrributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRUBUTES,*POBJECT_ATTRIBUTES;


typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID,*PCLIENT_ID;

#endif


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntexapi.h" 
#ifndef _NTEXAPI_H
#define _NTEXAPI_H


// some nt types

#include <ntbasic.h>

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation, // 10
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation, // 20
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemMirrorMemoryInformation, // 30
    SystemPerformanceTraceInformation,
    SystemCrashDumpInformation,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation, // used to be SystemLoadAndCallImage
    SystemPrioritySeparation,
    SystemVerifierAddDriverInformation, // 40
    SystemVerifierRemoveDriverInformation,
    SystemProcessorIdleInformation,
    SystemLegacyDriverInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation,
    SystemTimeSlipNotification,
    SystemSessionCreate,
    SystemSessionDetach,
    SystemSessionInformation,
    SystemRangeStartInformation, // 50
    SystemVerifierInformation,
    SystemVerifierThunkExtend,
    SystemSessionProcessInformation,
    SystemLoadGdiDriverInSystemSpace,
    SystemNumaProcessorMap,
    SystemPrefetcherInformation,
    SystemExtendedProcessInformation,
    SystemRecommendedSharedDataAlignment,
    SystemComPlusPackage,
    SystemNumaAvailableMemory, // 60
    SystemProcessorPowerInformation,
    SystemEmulationBasicInformation,
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation,
    SystemLostDelayedWriteInformation,
    SystemBigPoolInformation,
    SystemSessionPoolTagInformation,
    SystemSessionMappedViewInformation,
    SystemHotpatchInformation,
    SystemObjectSecurityMode, // 70
    SystemWatchdogTimerHandler, // doesn't seem to be implemented
    SystemWatchdogTimerInformation,
    SystemLogicalProcessorInformation,
    SystemWow64SharedInformation,
    SystemRegisterFirmwareTableInformationHandler,
    SystemFirmwareTableInformation,
    SystemModuleInformationEx,
    SystemVerifierTriageInformation,
    SystemSuperfetchInformation,
    SystemMemoryListInformation, // 80
    SystemFileCacheInformationEx,
    SystemNotImplemented19,
    SystemProcessorDebugInformation,
    SystemVerifierInformation2,
    SystemNotImplemented20,
    SystemRefTraceInformation,
    SystemSpecialPoolTag, // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
    SystemProcessImageName,
    SystemNotImplemented21,
    SystemBootEnvironmentInformation, // 90
    SystemEnlightenmentInformation,
    SystemVerifierInformationEx,
    SystemNotImplemented22,
    SystemNotImplemented23,
    SystemCovInformation,
    SystemNotImplemented24,
    SystemNotImplemented25,
    SystemPartitionInformation,
    SystemSystemDiskInformation, // this and SystemPartitionInformation both call IoQuerySystemDeviceName
    SystemPerformanceDistributionInformation, // 100
    SystemNumaProximityNodeInformation,
    SystemTimeZoneInformation2,
    SystemCodeIntegrityInformation,
    SystemNotImplemented26,
    SystemUnknownInformation, // No symbols for this case, very strange...
    SystemVaInformation, // 106, calls MmQuerySystemVaInformation
    MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;


typedef struct _SYSTEM_PROCESS_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER SpareLi1;
    LARGE_INTEGER SpareLi2;
    LARGE_INTEGER SpareLi3;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR PageDirectoryBase;
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

typedef NTSTATUS(NTAPI* _NtQuerySystemInformation)(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __out_bcount_opt(SystemInformationLength) PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
    );


#endif
C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntimport.h" 
#ifndef _PH_BASE_H
#define _PH_BASE_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <ntbasic.h>
#include <ntexapi.h>
#include <ntobapi.h>
#include <ntpsapi.h>
#include <ntrtl.h>

#ifdef NTIMPORT_PRIVATE
#define EXT
#define EQNULL = NULL
#else
#define EXT extern
#define EQNULL
#endif

EXT _NtAlertResumeThread NtAlertResumeThread EQNULL;
EXT _NtAlertThread NtAlertThread EQNULL;
EXT _NtClose NtClose EQNULL;
EXT _NtDuplicateObject NtDuplicateObject EQNULL;
EXT _NtGetNextProcess NtGetNextProcess EQNULL;
EXT _NtGetNextThread NtGetNextThread EQNULL;
EXT _NtOpenProcess NtOpenProcess EQNULL;
EXT _NtOpenThread NtOpenThread EQNULL;
EXT _NtQueryInformationProcess NtQueryInformationProcess EQNULL;
EXT _NtQueryInformationThread NtQueryInformationThread EQNULL;
EXT _NtQueryObject NtQueryObject EQNULL;
EXT _NtQuerySystemInformation NtQuerySystemInformation EQNULL;
EXT _NtQueueApcThread NtQueueApcThread EQNULL;
EXT _NtResumeProcess NtResumeProcess EQNULL;
EXT _NtResumeThread NtResumeThread EQNULL;
EXT _NtSetInformationProcess NtSetInformationProcess EQNULL;
EXT _NtSetInformationThread NtSetInformationThread EQNULL;
EXT _NtSuspendProcess NtSuspendProcess EQNULL;
EXT _NtSuspendThread NtSuspendThread EQNULL;
EXT _NtTerminateProcess NtTerminateProcess EQNULL;
EXT _NtTerminateThread NtTerminateThread EQNULL;

BOOLEAN KrEInitializeImport();

#endif

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntobapi.h" 
#ifndef _NTOBAPI_H
#define _NTOBAPI_H

#include <ntbasic.h>

#if defined(_MSC_VER) &&(_MSC_VER >=1020)
#pragma once
#endif



typedef enum _OBJECT_INFORMATION_CLASS
{
    ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectTypesInformation,
	ObjectHandleFlagInformation,
	ObjectSessionInformation
} OBJECT_INFORMATION_CLASS;


typedef NTSTATUS(NTAPI* _NtMakeTemporaryObject)(
    __in HANDLE Handle
    );

typedef NTSTATUS(NTAPI* _NtMakePermanentObject)(
    __in HANDLE Handle
    );

typedef NTSTATUS(NTAPI* _NtSignalAndWaitForSingleObject)(
    __in HANDLE SignalHandle,
    __in HANDLE WaitHandle,
    __in BOOLEAN Alertable,
    __in_opt PLARGE_INTEGER Timeout
    );

typedef NTSTATUS(NTAPI* _NtWaitForSingleObject)(
    __in HANDLE Handle,
    __in BOOLEAN Alertable,
    __in_opt PLARGE_INTEGER Timeout
    );

typedef NTSTATUS(NTAPI* _NtWaitForMultipleObjects)(
    __in ULONG Count,
    __in_ecount(Count) PHANDLE Handles,
    __in WAIT_TYPE WaitType,
    __in BOOLEAN Alertable,
    __in_opt PLARGE_INTEGER Timeout
    );

typedef NTSTATUS(NTAPI* _NtSetSecurityObject)(
    __in HANDLE Handle,
    __in SECURITY_INFORMATION SecurityInformation,
    __in PSECURITY_DESCRIPTOR SecurityDescriptor
    );

typedef NTSTATUS(NTAPI* _NtQuerySecurityObject)(
    __in HANDLE Handle,
    __in SECURITY_INFORMATION SecurityInformation,
    __out_bcount_opt(Length) PSECURITY_DESCRIPTOR SecurityDescriptor,
    __in ULONG Length,
    __out PULONG LengthNeeded
    );

typedef NTSTATUS(NTAPI* _NtClose)(
    __in HANDLE Handle
    );

typedef NTSTATUS(NTAPI* _NtQueryObject)(
    __in HANDLE Handle,
    __in OBJECT_INFORMATION_CLASS ObjectInformationClass,
    __out_bcount_opt(ObjectInformationLength) PVOID ObjectInformation,
    __in ULONG ObjectInformationLength,
    __out_opt PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* _NtDuplicateObject)(
    __in HANDLE SourceProcessHandle,
    __in HANDLE SourceHandle,
    __in_opt HANDLE TargetProcessHandle,
    __out_opt PHANDLE TargetHandle,
    __in ACCESS_MASK DesiredAccess,
    __in ULONG HandleAttributes,
    __in ULONG Options
    );

#endif


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntpebteb.h" 
#ifndef _NTPEBTEB_H
#define _NTPEBTEB_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <ntbasic.h>

#endif


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntpsapi.h" 
#ifndef _NTPSAPI_H
#define _NTPSAPI_H


#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <ntbasic.h>
#include <ntpebteb.h>

// System calls

typedef NTSTATUS(NTAPI* _NtOpenProcess)(
    __out PHANDLE ProcessHandle,
    __in ACCESS_MASK DesiredAccess,
    __in POBJECT_ATTRIBUTES ObjectAttributes,
    __in_opt PCLIENT_ID ClientId
    );

typedef NTSTATUS(NTAPI* _NtTerminateProcess)(
    __in_opt HANDLE ProcessHandle,
    __in NTSTATUS ExitStatus
    );

#define NtCurrentProcess() ((HANDLE)(LONG_PTR)-1)
#define ZwCurrentProcess() NtCurrentProcess()
#define NtCurrentPeb() (NtCurrentTeb()->ProcessEnvironmentBlock)

typedef NTSTATUS(NTAPI* _NtQueryInformationProcess)(
    __in HANDLE ProcessHandle,
    __in PROCESS_INFORMATION_CLASS ProcessInformationClass,
    __out_bcount(ProcessInformationLength) PVOID ProcessInformation,
    __in ULONG ProcessInformationLength,
    __out_opt PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* _NtGetNextProcess)(
    __in HANDLE ProcessHandle,
    __in ACCESS_MASK DesiredAccess,
    __in ULONG HandleAttributes,
    __in ULONG Flags,
    __out PHANDLE NewProcessHandle
    );

typedef NTSTATUS(NTAPI* _NtGetNextThread)(
    __in HANDLE ProcessHandle,
    __in HANDLE ThreadHandle,
    __in ACCESS_MASK DesiredAccess,
    __in ULONG HandleAttributes,
    __in ULONG Flags,
    __out PHANDLE NewThreadHandle
    );

typedef NTSTATUS(NTAPI* _NtSetInformationProcess)(
    __in HANDLE ProcessHandle,
    __in PROCESS_INFORMATION_CLASS ProcessInformationClass,
    __in_bcount(ProcessInformationLength) PVOID ProcessInformation,
    __in ULONG ProcessInformationLength
    );

typedef NTSTATUS(NTAPI* _NtOpenThread)(
    __out PHANDLE ThreadHandle,
    __in ACCESS_MASK DesiredAccess,
    __in POBJECT_ATTRIBUTES ObjectAttributes,
    __in_opt PCLIENT_ID ClientId
    );

typedef NTSTATUS(NTAPI* _NtTerminateThread)(
    __in_opt HANDLE ThreadHandle,
    __in NTSTATUS ExitStatus
    );

typedef NTSTATUS(NTAPI* _NtSuspendThread)(
    __in HANDLE ThreadHandle,
    __out_opt PULONG PreviousSuspendCount
    );

typedef NTSTATUS(NTAPI* _NtResumeThread)(
    __in HANDLE ThreadHandle,
    __out_opt PULONG PreviousSuspendCount
    );

typedef NTSTATUS(NTAPI* _NtSuspendProcess)(
    __in HANDLE ProcessHandle
    );

typedef NTSTATUS(NTAPI* _NtResumeProcess)(
    __in HANDLE ProcessHandle
    );

typedef NTSTATUS(NTAPI* _NtGetContextThread)(
    __in HANDLE ThreadHandle,
    __inout PCONTEXT ThreadContext
    );

typedef NTSTATUS(NTAPI* _NtSetContextThread)(
    __in HANDLE ThreadHandle,
    __in PCONTEXT ThreadContext
    );

typedef NTSTATUS(NTAPI* _NtQueryInformationThread)(
    __in HANDLE ThreadHandle,
    __in THREAD_INFORMATION_CLASS ThreadInformationClass,
    __out_bcount(ThreadInformationLength) PVOID ThreadInformation,
    __in ULONG ThreadInformationLength,
    __out_opt PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* _NtSetInformationThread)(
    __in HANDLE ThreadHandle,
    __in THREAD_INFORMATION_CLASS ThreadInformationClass,
    __in_bcount(ThreadInformationLength) PVOID ThreadInformation,
    __in ULONG ThreadInformationLength
    );

typedef NTSTATUS(NTAPI* _NtAlertThread)(
    __in HANDLE ThreadHandle
    );

typedef NTSTATUS(NTAPI* _NtAlertResumeThread)(
    __in HANDLE ThreadHandle,
    __out_opt PULONG PreviousSuspendCount
    );

typedef NTSTATUS(NTAPI* _NtTestAlert)();

typedef VOID(*PPS_APC_ROUTINE)(
    __in_opt PVOID ApcArgument1,
    __in_opt PVOID ApcArgument2,
    __in_opt PVOID ApcArgument3
    );

typedef NTSTATUS(NTAPI* _NtQueueApcThread)(
    __in HANDLE ThreadHandle,
    __in PPS_APC_ROUTINE ApcRoutine,
    __in_opt PVOID ApcArgument1,
    __in_opt PVOID ApcArgument2,
    __in_opt PVOID ApcArgument3
    );

#endif


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntrtl.h" 
#ifndef  _NTRTL_H
#define _NTRTL_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#define RtlAllocateHeap HeapAlloc
#define RtlFreeHeap		HeapFree

VOID FORCEINLINE RtlInitUnicodeString(
	__out PUNICODE_STRING DestinationString,
	__in PWSTR SourceString
)
{
	DestinationString->MaximumLength = DestinationString->Length = (USHORT)(wcslen(SourceString) * sizeof(WCHAR));
	DestinationString->Buffer = SourceString;
}


#endif

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ntwin.h" 
#ifndef _NT_WIN_H
#define _NT_WIN_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif


#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>

#endif


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ph.h" 
#ifndef PH_H
#define PH_H



#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <phbase.h>
//some definition

#define PH_MAX_MESSAGE_SIZE 400
#define KrEShowError(hWnd,Format,...) KrEShowMessage_V(hWnd,MB_OK|MB_ICONERROR,Format,__VA_ARGS__)
#define KRE_FIRST_PROCESS(Processes) ((PSYSTEM_PROCESS_INFORMATION)(Processes))
#define KRE_NEXT_PROCESS(Process) (\
		((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset ? \
		((PSYSTEM_PROCESS_INFORMATION)((PCHAR)(Process)+			\
		((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset)): \
		(VOID*)NULL)

typedef struct _KRE_PROCESS_ITEM
{
	HANDLE ProcessId;
	HANDLE ParentProcessId;
	PKRE_STRING ProcessName;
	ULONG SessionId;

	HICON SmallIcon;
	HICON LargeIcon;

	PKRE_STRING FileName;
	PKRE_STRING CommandLine;

	LARGE_INTEGER CreateTime;

	PKRE_STRING UserName;
	ULONG IntegrityLevel;
	PKRE_STRING IntegrityString;

	ULONG HasParent : 1;
	ULONG IsBeingDebugged : 1;
	ULONG IsDotNet : 1;
	ULONG IsElevated : 1;
	ULONG IsInJob : 1;
	ULONG IsInSignificantJob : 1;
	ULONG IsPacked : 1;
	ULONG IsPosix : 1;
	ULONG IsWow64 : 1;

	FLOAT CpuUsage; // from 0 to 1
} KRE_PROCESS_ITEM, * PKRE_PROCESS_ITEM;


PKRE_PROCESS_ITEM KrECreateProcessItem(
	__in HANDLE ProcessId
);

INT KrEShowMessage_V(
	__in HWND hWnd,
	__in ULONG Type,
	__in PWSTR Format,
	...
);

typedef BOOLEAN(*PPH_ENUM_PROCESSES_CALLBACK)(__in PSYSTEM_PROCESS_INFORMATION Process);


VOID KrEFree(__in PVOID Memory);

NTSTATUS KrEEnumProcesses(
	__out PPVOID Processes
);

BOOLEAN KrEInitializeProcessItem();

#endif

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\phbase.h" 
#ifndef _PHBASE_H
#define _PHBASE_H



#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <ntwin.h>
#include <ntimport.h>
#include <ref.h>

#define KrE_APP_NAME (L"KernerlEverything")

#define KrERaiseStatus(Status) RaiseException(Status,0,0,NULL)

extern PWSTR KrEWindowClassName;
extern HINSTANCE KrEInstanceHandle;
extern HFONT KrEApplicationFont;
extern HANDLE KrEHeapHandle;


typedef struct _KRE_STRING
{
	UNICODE_STRING us;
	WCHAR Buffer[1];
}KRE_STRING,*PKRE_STRING;

BOOLEAN KrEInitializeBase();
PVOID KrEAllocate(__in SIZE_T Size);
VOID KrEFree(__in PVOID Memory);

PKRE_STRING KrECreateStringEx(
	__in PWSTR Buffer,
	__in SIZE_T Length
);




#endif


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\phgui.h" 
#ifndef _PHGUI_H
#define _PHGUI_H



#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <phbase.h>
#include <ph.h>
#include <CommCtrl.h>
#include "resource.h"


INT KrEMainMessageLoop();
BOOLEAN KrEMainWndInitialization(__in INT ShowCommand);


VOID KrEInitializeFont(__in HWND hWnd);
BOOLEAN KrEInitializeSystem();



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


INT KrEAddListViewItem(
	HWND ListViewHandle,
	INT Index,
	PWSTR Text,
	PVOID Param
);




#endif


C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\ref.h" 
#ifndef _REF_H
#define _REF_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#include <phbase.h>

#define KRE_OBJECT_TYPE_VALID_FLAGS 0x0000000

#define KRE_OBJECT_VALID_FLAGS		0x0000001
#define KRE_OBJECT_RAISE_ON_FAIL    0x0000001


struct _KRE_OBJECT_TYPE;
typedef struct _KRE_OBJECT_TYPE *PKRE_OBJECT_TYPE;


NTSTATUS KrEInitializeRef();

/*
 *	the pointer to the deletion procedure
 */
typedef VOID(NTAPI* PKRE_TYPE_DELETE_PROCEDURE)(
	__in PVOID Object,
	__in ULONG Flags	
	);

NTSTATUS KrECreateObjectType(
	__out PKRE_OBJECT_TYPE* ObjectType,
	__in ULONG Flags,
	__in PKRE_TYPE_DELETE_PROCEDURE DeleteProcedure
);

NTSTATUS KrECreateObject(
	__out PVOID* Object,
	__in SIZE_T ObjectSize,
	__in ULONG Flags,
	__in_opt PKRE_OBJECT_TYPE ObjectType,
	__in_opt LONG AdditionalReferences
);

BOOLEAN KrEDereferenceObject(__in PVOID Object);

LONG KrEDereferenceObjectEx(
	__in PVOID Object,
	__in LONG RefCount,
	__in BOOLEAN DeferDelete
);

#endif

C:\code\KernelEverything>type "C:\code\KernelEverything\KrE\include\refp.h" 
#ifndef _REFP_H
#define _REFP_H

#if defined(_MSC_VER) &&(_MSC_VER>1020)
#pragma once
#endif

#include <ref.h>

#define KrEObjectToObjectHeader(Object) \
	((PKRE_OBJECT_HEADER)CONTAINING_RECORD((Object),KRE_OBJECT_HEADER,Body))
#define KrEAddObjectHeaderSize(Size) ((Size)+FIELD_OFFSET(KRE_OBJECT_HEADER,Body))
#define KrEObjectHeaderToObject(ObjectHeader) (&((PKRE_OBJECT_HEADER)(ObjectHeader))->Body)

typedef struct _KRE_OBJECT_HEADER *PKRE_OBJECT_HEADER;
typedef struct _KRE_OBJECT_TYPE
{
	ULONG Flags;
	PKRE_TYPE_DELETE_PROCEDURE	DeleteProcedure;
	ULONG NumberOfObject;
}KRE_OBJECT_TYPE,*PKRE_OBJECT_TYPE;

typedef struct _KRE_OBJECT_HEADER
{
	LONG RefCount;
	ULONG Flags;
	union
	{
		SIZE_T Size;
		PKRE_OBJECT_HEADER NextToFree;
	};
	PKRE_OBJECT_TYPE Type;

	QUAD Body;
}KRE_OBJECT_HEADER;


PKRE_OBJECT_HEADER KrEAllocateObject(
	__in SIZE_T ObjectSize
);

NTSTATUS KrEDeferDeleteObjectRoutine(
	__in PVOID Parameter
);

VOID KrEFreeObject(
	__in PKRE_OBJECT_HEADER ObjectHeader
);

VOID KrEDeferDeleteObject(__in PKRE_OBJECT_HEADER ObjectHeader);

#endif;
