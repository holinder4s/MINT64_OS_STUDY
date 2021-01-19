#include "Queue.h"

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