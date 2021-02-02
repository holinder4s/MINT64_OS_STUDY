#include "PIT.h"

// PIT를 초기화
void kInitializePIT(WORD wCount, BOOL bPeriodic) {
    // PIT 컨트롤 레지스터(포트 0x43)에 값을 초기화하여 카운트를 멈춘 뒤에 모드 0에 바이너리 카운터로 설정
    kOutPortByte(PIT_PORT_CONTROL, PIT_COUNTER0_ONCE);

    // 만약 일정한 주기로 반복하는 타이머라면 모드 2로 설정
    if(bPeriodic == TRUE) {
        // PIT 컨트롤 레지스터(포트 0x43)에 모드 2에 바이너리 카운터로 설정
        kOutPortByte(PIT_PORT_CONTROL, PIT_COUNTER0_PERIODIC);
    }

    // 카운터 0(포트 0x40)에 LSB -> MSB 순으로 카운터 초기 값을 설정
    kOutPortByte(PIT_PORT_COUNTER0, wCount);
    kOutPortByte(PIT_PORT_COUNTER0, wCount >> 8);
}

// 카운터 0의 현재 값을 반환
WORD kReadCounter0(void) {
    BYTE bHighByte, bLowByte;
    WORD wTemp = 0;

    // PIT 컨트롤 레지스터(포트 0x43)에 래치 커맨드를 전송하여 카운터 0에 있는 현재 값을 읽음
    kOutPortByte(PIT_PORT_CONTROL, PIT_COUNTER0_LATCH);

    // 카운터 0(포트 0x40)에서 LSB -> MSB 순으로 카운터 값을 읽음
    bLowByte = kInPortByte(PIT_PORT_COUNTER0);
    bHighByte = kInPortByte(PIT_PORT_COUNTER0);

    // 읽은 값을 16비트로 합하여 반환
    wTemp = bHighByte;
    wTemp = (wTemp << 8) | bLowByte;

    return wTemp;
}