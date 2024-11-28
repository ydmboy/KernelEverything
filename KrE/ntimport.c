#include <ntimport.h>
#include <ph.h>

#define InitProcReq(DllName, ProcName)                                \
    if (!initProc(DllName, ProcName))                                 \
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
	
}


