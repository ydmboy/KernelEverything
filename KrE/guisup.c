#include <phgui.h>

#define TCM_SETBKCOLOR          (TCM_FIRST + 1)
HWND KrECreateTabControl(HWND ParentHandle)
{
	HWND tabControlHandle = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE , 0, 0, 1, 1, ParentHandle, NULL, KrEInstanceHandle, NULL);

	COLORREF bgColor = RGB(200, 200, 255); // ����ɫ
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


INT KrEAddListViewItem(HWND ListViewHandle,
	INT Index,
	PWSTR Text
)
{
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = Index;
	item.iSubItem = 0;
	item.pszText = Text;
	return ListView_InsertItem(ListViewHandle, &item);


}