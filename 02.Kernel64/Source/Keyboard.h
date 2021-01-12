#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "Types.h"

// 구조체
#pragma pack(push, 1)

// 스캔 코드 테이블을 구성하는 항목
typedef struct kKeyMappingEntryStruct {
    // Shift 키나 Caps Lock 키와 조합되지 않는 ASCII 코드
    BYTE bNormalCode;

    // Shift 키나 Caps Lock 키와 조합된 ASCII 코드
    BYTE bCombinedCode;
} KEYMAPPINGENTRY;

#pragma pack(pop)

// 함수
BOOL kIsOutputBufferFull(void);
BOOL kIsInputBufferFull(void);
BOOL kActivateKeyboard(void);
BYTE kGetKeyboardScanCode(void);
BOOL kChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn);

#endif /*__KEYBOARD_H__*/