#include <phgui.h>

HWND KrECreateTabControl(HWND ParentHandle)
{
	HWND tabControlHandle = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE , 0, 0, 1, 1, ParentHandle, NULL, KrEInstanceHandle, NULL);
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