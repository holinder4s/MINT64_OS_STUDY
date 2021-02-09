#include "List.h"

// 리스트에 데이터를 더함
void kAddListToTail(LIST *pstList, void *pvItem) {
    LISTLINK *pstLink;

    // 다음 데이터의 어드레스를 없음(NULL)으로 설정
    pstLink = (LISTLINK *)pvItem;
    pstLink->pvNext = NULL;

    // 리스트가 빈 상태이면 Header와 Tail을 추가한 데이터로 설정
    if(pstList->pvHeader == NULL) {
        pstList->pvHeader = pvItem;
        pstList->pvTail = pvItem;
        pstList->iItemCount = 1;

        return;
    }

    // 마지막 데이터의 LISTLINK의 위치를 구하여 다음 데이터를 추가한 데이터로 설정
    pstLink = (LISTLINK *)pstList->pvTail;
    pstLink->pvNext = pvItem;

    // 리스트의 마지막 데이터를 추가한 데이터로 변경
    pstList->pvTail = pvItem;
    pstList->iItemCount++;
}

// 리스트의 첫 부분에 데이터를 더함
void kAddListToHeader(LIST *pstList, void *pvItem) {
    LISTLINK *pstLink;

    // 다음 데이터의 어드레스를 Header로 설정
    pstLink = (LISTLINK *)pvItem;
    pstLink->pvNext = pstList->pvHeader;

    // 리스트가 빈 상태이면 Header와 Tail을 추가한 데이터로 설정
    if(pstList->pvHeader == NULL) {
        pstList->pvHeader = pvItem;
        pstList->pvTail = pvItem;
        pstList->iItemCount = 1;

        return;
    }

    // 리스트의 첫 번째 데이터를 추가한 데이터로 변경
    pstList->pvHeader = pvItem;
    pstList->iItemCount++;
}