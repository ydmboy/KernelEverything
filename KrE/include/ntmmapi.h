#ifndef _NT_MM_API_H
#define _NT_MM_API_H 

#if defined(_MSC_VER)&&(_MSC_VER>1020)
#pragma once
#endif

typedef NTSTATUS (NTAPI *_NtAllocateVirtualMemory)(
    __in HANDLE ProcessHandle,
    __inout PVOID *BaseAddress,
    __in ULONG_PTR ZeroBits,
    __inout PSIZE_T RegionSize,
    __in ULONG AllocationType,
    __in ULONG Protect
    );

typedef NTSTATUS (NTAPI *_NtReadVirtualMemory)(
    __in HANDLE ProcessHandle,
    __in_opt PVOID BaseAddress,
    __out_bcount(BufferSize) PVOID Buffer,
    __in SIZE_T BufferSize,
    __out_opt PSIZE_T NumberOfBytesRead
    );




#endif

