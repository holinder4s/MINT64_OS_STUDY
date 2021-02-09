#ifndef __LIST_H__
#define __LIST_H__

#include "Types.h"

// 구조체
// 1바이트로 정렬
#pragma pack(push, 1)

// 데이터를 연결하는 자료구조
// 반드시 데이터의 가장 앞부분에 위치해야 함
typedef struct kListLinkStruct {
    // 다음 데이터의 어드레스와 데이터를 구분하기 위한 ID
    void *pvNext;
    QWORD qwID;
} LISTLINK;

#pragma pack(pop)

#endif /*__LIST_H__*/