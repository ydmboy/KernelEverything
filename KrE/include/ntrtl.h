#ifndef  _NTRTL_H
#define _NTRTL_H

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#pragma once
#endif

#define RtlAllocateHeap HeapAlloc
#define RtlFreeHeap		HeapFree

#endif
