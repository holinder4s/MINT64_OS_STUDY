#include "Descriptor.h"

//===================================================================================
// GDT 및 TSS
//===================================================================================

// GDT 테이블을 초기화
void kInitializeGDTTableAndTSS(void) {
    GDTR *pstGDTR;
    GDTENTRY8 *pstEntry;
    TSSSEGMENT *pstTSS;
    int i;

    // GDTR 설정
    pstGDTR = (GDTR *)GDTR_STARTADDRESS;
    pstEntry = (GDTENTRY8 *)(GDTR_STARTADDRESS + sizeof(GDTR));
    pstGDTR->wLimit = GDT_TABLESIZE - 1;
    pstGDTR->qwBaseAddress = (QWORD)pstEntry;
    // TSS 영역 설정
    pstTSS = (TSSSEGMENT *)((QWORD)pstEntry + GDT_TABLESIZE);

    // NULL, 64비트 Code/Data, TSS를 위해 총 4개의 세그먼트를 생성한다.
    kSetGDTEntry8(&(pstEntry[0]), 0, 0, 0, 0, 0);
    kSetGDTEntry8(&(pstEntry[1]), 0, 0xFFFFF, GDT_FLAGS_UPPER_CODE, GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE);
    kSetGDTEntry8(&(pstEntry[2]), 0, 0xFFFFF, GDT_FLAGS_UPPER_DATA, GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA);
    kSetGDTEntry16((GDTENTRY16 *)&(pstEntry[3]), (QWORD)pstTSS, sizeof(TSSSEGMENT) - 1, GDT_FLAGS_UPPER_TSS, GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS);

    // TSS 초기화 GDT 이하 영역을 사용함
    kInitializeTSSSegment(pstTSS);
}

// 8바이트 크기의 GDT 엔트리에 값을 설정
// 코드와 데이터 세그먼트 디스크립터를 설정하는데 사용
void kSetGDTEntry8(GDTENTRY8 *pstEntry, DWORD dwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType) {
    pstEntry->wLowerLimit = dwLimit & 0xFFFF;
    pstEntry->wLowerBaseAddress = dwBaseAddress & 0xFFFF;
    pstEntry->bUpperBaseAddress1 = (dwBaseAddress >> 16) & 0xFF;
    pstEntry->bTypeAndLowerFlag = bLowerFlags | bType;
    pstEntry->bUpperLimitAndUpperFlag = ((dwLimit >> 16) & 0x0F) | bUpperFlags;
    pstEntry->bUpperBaseAddress2 = (dwBaseAddress >> 24) & 0xFF;
}

// 16바이트 크기의 GDT 엔트리에 값을 설정
// TSS 세그먼트 디스크립터를 설정하는데 사용
void kSetGDTEntry16(GDTENTRY16 *pstEntry, QWORD qwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType) {
    pstEntry->wLowerLimit = dwLimit & 0xFFFF;
    pstEntry->wLowerBaseAddress = qwBaseAddress & 0xFFFF;
    pstEntry->bMiddleBaseAddress1 = (qwBaseAddress >> 16) & 0xFF;
    pstEntry->bTypeAndLowerFlag = bLowerFlags | bType;
    pstEntry->bUpperLimitAndUpperFlag = ((dwLimit >> 16) & 0x0F) | bUpperFlags;
    pstEntry->bMiddleBaseAddress2 = (qwBaseAddress >> 24) & 0xFF;
    pstEntry->dwUpperBaseAddress = qwBaseAddress >> 32;
    pstEntry->dwReserved = 0;
}