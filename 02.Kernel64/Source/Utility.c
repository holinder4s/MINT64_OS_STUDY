#include "Utility.h"

// 메모리를 특정 값으로 채움
void kMemSet(void *pvDestination, BYTE bData, int iSize) {
    int i;

    for(i=0; i<iSize; i++) {
        ((char *)pvDestination)[i] = bData;
    }
}