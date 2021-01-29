#ifndef __CONSOLESHELL_H__
#define __CONSOLESHELL_H__

#include "Types.h"

// 매크로
#define CONSOLESHELL_MAXCOMMANDBUFFERCOUNT  300
#define CONSOLESHELL_PROMPTMESSAGE          "MINT64>"

// 문자열포인터를 파라미터로 받는 함수 포인터 타입 정의
typedef void (*CommandFunction)(const char *pcParameter);

// 구조체
// 1바이트로 정렬
#pragma pack(push, 1)

// 쉘의 커맨드를 저장하는 자료구조
typedef struct kShellCommandEntryStruct {
    // 커맨드 문자열
    char *pcCommand;
    // 커맨드의 도움말
    char *pcHelp;
    // 커맨드를 수행하는 함수의 포인터
    CommandFunction pfFunction;
} SHELLCOMMANDENTRY;

#pragma pack(pop)

// 함수
// 실제 쉘 코드
void kStartConsoleShell(void);
void kExecuteCommand(const char *pcCommandBuffer);

// 커맨드를 처리하는 함수
void kHelp(const char *pcCommandBuffer);

#endif /*__CONSOLESHELL_H__*/