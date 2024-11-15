#include <phgui.h>

HWND KrECreateTabControl(HWND ParentHandle)
{
	return CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 3, 3, ParentHandle, NULL, KrEInstanceHandle, NULL);
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