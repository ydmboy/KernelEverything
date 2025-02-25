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

PKRE_STRING KrEGetSystemDirectory()
{
	PKRE_STRING systemDirectory;
	PVOID buffer;
	ULONG bufferSize;
	ULONG returnLength;

	bufferSize = 0x40;
	buffer = KrEAllocate(bufferSize*2);

	returnLength = GetSystemDirectory(buffer,bufferSize);

	if (returnLength > bufferSize)
	{
		KrEFree(buffer);
		bufferSize = returnLength;
		buffer = KrEAllocate(bufferSize*2);

		returnLength = GetSystemDirectory(buffer, bufferSize);
	}
	if (returnLength == 0)
	{
		KrEFree(buffer);
		return NULL;
	}

	systemDirectory = KrECreateString(buffer);
	KrEFree(buffer);

	return systemDirectory;

}





