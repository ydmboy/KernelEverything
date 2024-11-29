#ifndef _NTOBAPI_H
#define _NTOBAPI_H

#include <ntbasic.h>

#if defined(_MSC_VER) &&(_MSC_VER >=1020)
#pragma once
#endif



typedef enum _OBJECT_INFORMATION_CLASS
{
    ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectTypesInformation,
	ObjectHandleFlagInformation,
	ObjectSessionInformation
} OBJECT_INFORMATION_CLASS;


typedef NTSTATUS(NTAPI* _NtMakeTemporaryObject)(
    __in HANDLE Handle
    );

typedef NTSTATUS(NTAPI* _NtMakePermanentObject)(
    __in HANDLE Handle
    );

typedef NTSTATUS(NTAPI* _NtSignalAndWaitForSingleObject)(
    __in HANDLE SignalHandle,
    __in HANDLE WaitHandle,
    __in BOOLEAN Alertable,
    __in_opt PLARGE_INTEGER Timeout
    );

typedef NTSTATUS(NTAPI* _NtWaitForSingleObject)(
    __in HANDLE Handle,
    __in BOOLEAN Alertable,
    __in_opt PLARGE_INTEGER Timeout
    );

typedef NTSTATUS(NTAPI* _NtWaitForMultipleObjects)(
    __in ULONG Count,
    __in_ecount(Count) PHANDLE Handles,
    __in WAIT_TYPE WaitType,
    __in BOOLEAN Alertable,
    __in_opt PLARGE_INTEGER Timeout
    );

typedef NTSTATUS(NTAPI* _NtSetSecurityObject)(
    __in HANDLE Handle,
    __in SECURITY_INFORMATION SecurityInformation,
    __in PSECURITY_DESCRIPTOR SecurityDescriptor
    );

typedef NTSTATUS(NTAPI* _NtQuerySecurityObject)(
    __in HANDLE Handle,
    __in SECURITY_INFORMATION SecurityInformation,
    __out_bcount_opt(Length) PSECURITY_DESCRIPTOR SecurityDescriptor,
    __in ULONG Length,
    __out PULONG LengthNeeded
    );

typedef NTSTATUS(NTAPI* _NtClose)(
    __in HANDLE Handle
    );

typedef NTSTATUS(NTAPI* _NtQueryObject)(
    __in HANDLE Handle,
    __in OBJECT_INFORMATION_CLASS ObjectInformationClass,
    __out_bcount_opt(ObjectInformationLength) PVOID ObjectInformation,
    __in ULONG ObjectInformationLength,
    __out_opt PULONG ReturnLength
    );

typedef NTSTATUS(NTAPI* _NtDuplicateObject)(
    __in HANDLE SourceProcessHandle,
    __in HANDLE SourceHandle,
    __in_opt HANDLE TargetProcessHandle,
    __out_opt PHANDLE TargetHandle,
    __in ACCESS_MASK DesiredAccess,
    __in ULONG HandleAttributes,
    __in ULONG Options
    );

#endif

