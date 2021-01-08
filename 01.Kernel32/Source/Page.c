#include "Page.h"

// IA-32e 모드 커널을 위한 페이지 테이블 생성
void kInitializePageTables(void) {
    PML4TENTRY *pstPML4TEntry;
    PDPTENTRY *pstPDPTEntry;
    PDENTRY *pstPDEntry;
    DWORD dwMappingAddress;
    int i;

    // PML4 테이블 생성
    // 첫 번째 엔트리 외에 나머지는 모두 0으로 초기화
    pstPML4TEntry = (PML4TENTRY *)0x100000;
    kSetPageEntryData(&(pstPML4TEntry[0]), 0x00, 0x101000, PAGE_FLAGS_DEFAULT, 0);
    for(i=1; i<PAGE_MAXENTRYCOUNT; i++) {
        kSetPageEntryData(&(pstPML4TEntry[i]), 0, 0, 0, 0);
    }
}