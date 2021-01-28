#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdarg.h>
#include "Types.h"

// 함수
void kMemSet(void *pvDestination, BYTE bData, int iSize);
int kMemCpy(void *pvDestination, const void *pvSource, int iSize);
int kMemCmp(void *pvDestination, const void *pvSource, int iSize);
BOOL kSetInterruptFlag(BOOL bEnableInterrupt);
int kVSPrintf(char *pcBuffer, const char *pcFormatString, va_list ap);

#endif /*__UTILITY_H__*/