#ifndef NT_SE_API_H
#define NT_SE_API_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif



typedef NTSTATUS(NTAPI* _NtOpenProcessToken)(
    __in HANDLE ProcessHandle,
    __in ACCESS_MASK DesiredAccess,
    __out PHANDLE TokenHandle
    );

typedef NTSTATUS(NTAPI* _NtQueryInformationToken)(
    __in HANDLE TokenHandle,    
    __in TOKEN_INFORMATION_CLASS TokenInformationClass,
    __out_bcount(TokenInformationLength) PVOID TokenInformation,
    __in ULONG TokenInformationLength,
    __out_opt PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* _NtSetInformationToken)(
    __in HANDLE TokenHandle,
    __in TOKEN_INFORMATION_CLASS TokenInformationClass,
    __in_bcount(TokenInformationLength) PVOID TokenInformation,
    __in ULONG TokenInformationLength
    );

#endif;
