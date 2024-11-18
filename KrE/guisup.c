#include <phgui.h>

HWND KrECreateTabControl(HWND ParentHandle)
{
	HWND tabControlHandle = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 3, 3, ParentHandle, NULL, KrEInstanceHandle, NULL);

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

HWND KrECreateListViewControl(
	HWND ParentHandle,
	INT_PTR Id
)
{
	return CreateWindow(WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | WS_VISIBLE | WS_BORDER, 0, 0, 3, 3, ParentHandle, (HMENU)Id, KrEInstanceHandle, NULL);
}