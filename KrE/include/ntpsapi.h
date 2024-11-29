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

