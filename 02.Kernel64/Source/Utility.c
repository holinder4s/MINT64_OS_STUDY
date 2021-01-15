#include "Utility.h"

// 메모리를 특정 값으로 채움
void kMemSet(void *pvDestination, BYTE bData, int iSize) {
    int i;

    for(i=0; i<iSize; i++) {
        ((char *)pvDestination)[i] = bData;
    }
}

// 메모리 복사
int kMemCpy(void *pvDestination, const void *pvSource, int iSize) {
    int i;

    for(i=0; i<iSize; i++) {
        ((char *)pvDestination)[i] = ((char *)pvSource)[i];
    }

    return iSize;
}