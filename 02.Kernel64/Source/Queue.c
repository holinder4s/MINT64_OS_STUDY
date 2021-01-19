#include "Queue.h"
#include "Utility.h"

// 큐를 초기화
void kInitializeQueue(QUEUE *pstQueue, void *pvQueueBuffer, int iMaxDataCount, int iDataSize) {
    // 큐의 최대 개수와 크기와 버퍼 어드레스를 지정
    pstQueue->iMaxDataCount = iMaxDataCount;
    pstQueue->iDataSize = iDataSize;
    pstQueue->pvQueueArray = pvQueueBuffer;

    // 큐의 삽입 위치와 제거 위치를 초기화하고 마지막으로 수행된 명령을 제거로 설정하여 큐를 빈 상태로 만듦
    pstQueue->iPutIndex = 0;
    pstQueue->iGetIndex = 0;
    pstQueue->bLastOperationPut = FALSE;
}

// 큐에 데이터를 삽입
BOOL kPutQueue(QUEUE *pstQueue, const void *pvData) {
    // 큐가 가득 찼으면 삽입할 수 없음
    if(kIsQueueFull(pstQueue) == TRUE) {
        return FALSE;
    }

    // 삽입 인덱스가 가리키는 위치에서 데이터의 크기만큼을 복사
    kMemCpy((char *)pstQueue->pvQueueArray + (pstQueue->iDataSize * pstQueue->iPutIndex), pvData, pstQueue->iDataSize);
    
    // 삽입 인덱스를 변경하고 삽입 동작을 수행했음을 기록
    pstQueue->iPutIndex = (pstQueue->iPutIndex + 1) % pstQueue->iMaxDataCount;
    pstQueue->bLastOperationPut = TRUE;

    return TRUE;
}