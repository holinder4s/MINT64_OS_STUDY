#ifndef __TASK_H__
#define __TASK_H__

#include "Types.h"

// 구조체
// 1바이트로 정렬
#pragma pack(push, 1)

// 태스크의 상태를 관리하는 자료구조
typedef struct kTaskControlBlockStruct {
    // 콘텍스트
    CONTEXT stContext;

    // ID 및 플래그
    QWORD qwID;
    QWORD qwFlags;

    // 스택의 어드레스와 크기
    void *pvStackAddress;
    QWORD qwStackSize;
} TCB;

#pragma pack(pop)

#endif /*__TASK_H__*/