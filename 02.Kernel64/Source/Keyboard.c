#include "Types.h"
#include "AssemblyUtility.h"

///////////////////////////////////////////////////////////
// 키보드 컨트롤러와 키보드 제어에 관련된 함수
///////////////////////////////////////////////////////////
// 출력 버퍼(포트 0x60)에 수신된 데이터가 있는지 여부를 반환
BOOL kIsOutputBufferFull(void) {
    // 상태 레지스터(포트 0x64)에서 읽은 값에 출력 버퍼 상태 비트(비트 0)가
    // 1로 설정되어 있으면 출력 버퍼에 키보드가 전송한 데이터가 존재함
    if(kInPortByte(0x64) & 0x01) {
        return TRUE;
    }
    return FALSE;
}

// 입력 버퍼(포트 0x60)에 프로세서가 쓴 데이터가 남아있는지 여부를 반환
BOOL kIsInputBufferFull(void) {
    // 상태 레지스터(포트 0x64)에서 읽은 값에 입력 버퍼 상태 비트(비트 1)가
    // 1로 설정되어 있으면 아직 키보드가 데이터를 가져가지 않았음
    if(kInPortByte(0x64) & 0x02) {
        return TRUE;
    }
    return FALSE;
}

// 키보드를 활성화
BOOL kActivateKeyboard(void) {
    int i;
    int j;

    // 컨트롤 레지스터(포트 0x64)에 키보드 활성화 커맨드(0xAE)를 전달하여 키보드 컨트롤러에 키보드 디바이스 활성화
    kOutPortByte(0x64, 0xAE);

    // 입력 버퍼(포트 0x60)가 빌 때가지 기다렸다가 키보드에 활성화 커맨드를 전송
    // 0xFFFF만큼 루프를 수행할 시간이면 충분히 커맨드가 전송될 수 있음
    // 0xFFFF 루프를 수행한 이후에도 입력 버퍼(포트 0x60)가 비지 않으면 무시하고 전송
    for(i=0; i<0xFFFF; i++) {
        // 입력 버퍼(포트 0x60)가 비어있으면 키보드 커맨드 전송 가능
        if(kIsInputBufferFull() == FALSE) {
            break;
        }
    }
    // 입력 버퍼(포트 0x60)로 키보드 활성화(0xF4) 커맨드를 전달하여 키보드로 전송
    kOutPortByte(0x60, 0xF4);

    // ACK가 올 때까지 대기함
    // ACK가 오기 전에는 키보드 출력 버퍼(포트 0x60)에 키 데이터가 저장되어 있을 수 있으므로
    // 키보드에서 전달된 데이터를 최대 100개까지 수신하여 ACK를 확인
    for(j=0; j<100; j++) {
        // 0xFFFF만큼 루프를 수행할 시간이면 충분히 커맨드의 응답이 올 수 있음
        // 0xFFFF 루프를 수행한 이후에도 출력 버퍼(포트 0x60)가 차 있지 않으면 무시하고 읽음
        for(i=0; i<0xFFFF; i++) {
            // 출력 버퍼(포트 0x60)가 차있으면 데이터를 읽을 수 있음
            if(kIsOutputBufferFull() == TRUE) {
                break;
            }
        }
        // 출력 버퍼(포트 0x60)에서 읽은 데이터가 ACK(0xFA)이면 성공
        if(kInPortByte(0x60) == 0xFA) {
            return TRUE;
        }
    }

    return FALSE;
}

// 출력 버퍼(포트 0x60)에서 키를 읽음
BYTE kGetKeyboardScanCode(void) {
    // 출력 버퍼(포트 0x60)에 데이터가 있을 때가지 대기
    while(kIsOutputBufferFull() == FALSE) {
        ;
    }
    return kInPortByte(0x60);
}

// 키보드 LED의 ON/OFF를 변경
BOOL kChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn) {
    int i, j;

    // 키보드에 LED 변경 커맨드 전송하고 커맨드가 처리될 때까지 대기
    for(i=0; i<0xFFFF; i++) {
        // 입력 버퍼(포트 0x60)가 비었으면 커맨드 전송 가능
        if(kIsInputBufferFull() == FALSE) {
            break;
        }
    }

    // 출력 버퍼(포트 0x60)로 LED 상태 변경 커맨드(0xED) 전송
    kOutPortByte(0x60, 0xED);
    for(i=0; i<0xFFFF; i++) {
        // 입력 버퍼(포트 0x60)가 비어 있으면 키보드가 커맨드를 가져간 것임
        if(kIsInputBufferFull() == FALSE) {
            break;
        }
    }

    // 키보드가 LED 상태 변경 커맨드를 가져갔으므로 ACK가 올 때가지 대기
    for(j=0; j<100; j++) {
        for(i=0; i<0xFFFF; i++) {
            // 출력 버퍼(포트 0x60)가 차 있으면 데이터를 읽을 수 있음
            if(kIsOutputBufferFull() == TRUE) {
                break;
            }
        }

        // 출력 버퍼(포트 0x60)에서 읽은 데이터가 ACK(0xFA)이면 성공
        if(kInPortByte(0x60) == 0xFA) {
            break;
        }
    }
    if(j >= 100) {
        return FALSE;
    }

    // LED 변경 값을 키보드로 전송하고 데이터가 처리가 완료될 때까지 대기
    kOutPortByte(0x60, (bCapsLockOn << 2) | (bNumLockOn << 1) | bScrollLockOn);
    for(i=0; i<0xFFFF; i++) {
        // 입력 버퍼(포트 0x60)가 비어 있으면 키보드가 LED 데이터를 가져간 것임
        if(kIsInputBufferFull() == FALSE) {
            break;
        }
    }

    // 키보드가 LED 데이터를 가져갔으므로 ACK가 올 때까지 대기함
    for(j=0; j<100; j++) {
        for(i=0; i<0xFFFF; i++) {
            // 출력 버퍼(포트 0x60)가 차 있으면 데이터를 읽을 수 있음
            if(kIsOutputBufferFull() == TRUE) {
                break;
            }
        }

        // 출력 버퍼(포트 0x60)에서 읽은 데이터가 ACK(0xFA)이면 성공
        if(kInPortByte(0x60) == 0xFA) {
            break;
        }
    }
    if(j >= 100) {
        return FALSE;
    }

    return TRUE;
}