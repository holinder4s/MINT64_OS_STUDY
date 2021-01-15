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