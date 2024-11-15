#include <phgui.h>

HWND PhMainwindowHandle;


LRESULT CALLBACK PhMainWndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch(uMsg)
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
			
		}
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
}
