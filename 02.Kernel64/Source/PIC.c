#include "PIC.h"

// PIC를 초기화
void kInitializePIC(void) {
    // 마스터 PIC 컨트롤러를 초기화
    // ICW1(포트 0x20), IC4비트(비트 0) = 1
    kOutPortByte(PIC_MASTER_PORT1, 0x11);

    // ICW2(포트 0x21), 인터럽트 벡터(0x20)
    kOutPortByte(PIC_MASTER_PORT2, PIC_IRQSTARTVECTOR);

    // ICW3(포트 0x21), 슬레이브 PIC 컨트롤러가 연결 위치(비트로 표현)
    // 마스터 PIC 컨트롤러의 2번 핀에 연결되어 있으므로, 0x04(비트 2)로 설정
    kOutPortByte(PIC_MASTER_PORT2, 0x04);

    // ICW4(포트 0x21), uPM 비트(비트 0) = 1
    kOutPortByte(PIC_MASTER_PORT2, 0x01);

    // 슬레이브 PIC 컨트롤러를 초기화
    // ICW1(포트 0xA0), IC4비트(비트 0) = 1
    kOutPortByte(PIC_SLAVE_PORT1, 0x11);

    // ICW2(포트 0xA1), 인터럽트 벡터(0x20 + 8);
    kOutPortByte(PIC_SLAVE_PORT2, PIC_IRQSTARTVECTOR + 8);

    // ICW3(포트 0xA1), 마스터 PIC 컨트롤러에 연결된 위치(정수로 표현)
    // 마스터 PIC 컨트롤러의 2번 핀에 연결되어 있으므로 0x02로 설정
    kOutPortByte(PIC_SLAVE_PORT2, 0x02);

    // ICW4(포트 0xA1), uPM 비트(비트 0) = 1
    kOutPortByte(PIC_SLAVE_PORT2, 0x01);
}