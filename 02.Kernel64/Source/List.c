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

// 리스트에서 데이터를 제거한 후, 데이터의 포인터를 반환
void *kRemoveList(LIST *pstList, QWORD qwID) {
    LISTLINK *pstLink;
    LISTLINK *pstPreviousLink;

    pstPreviousLink = (LISTLINK *)pstList->pvHeader;
    for(pstLink = pstPreviousLink; pstLink != NULL; pstLink = pstLink->pvNext) {
        // 일치하는 ID가 있다면 제거
        if(pstLink->qwID == qwID) {
            // 만약 데이터가 하나밖에 없다면 리스트 초기화
            if((pstLink == pstList->pvHeader) && (pstLink == pstList->pvTail)) {
                pstList->pvHeader = NULL;
                pstList->pvTail = NULL;
            }
            // 만약 리스트의 첫 번째 데이터면 Header를 두 번째 데이터로 변경
            else if(pstLink == pstList->pvHeader) {
                pstList->pvHeader = pstLink->pvNext;
            }
            // 만약 리스트의 마지막 데이터면 Tail을 마지막 이전의 데이터로 변경
            else if(pstLink == pstList->pvTail) {
                pstList->pvTail = pstPreviousLink;
            }
            else {
                pstPreviousLink->pvNext = pstLink->pvNext;
            }

            pstList->iItemCount--;
            return pstLink;
        }
        pstPreviousLink = pstLink;
    }
    return NULL;
}