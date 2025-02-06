#ifndef _NT_WIN_H
#define _NT_WIN_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif


#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>

#endif

