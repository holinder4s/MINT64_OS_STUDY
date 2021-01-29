#include "ConsoleShell.h"
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"

// 커맨드 테이블 정의
SHELLCOMMANDENTRY gs_vstCommandTable[] = {
    {"help", "Show Help", kHelp},
    {"cls", "Clear Screen", kCls},
    {"totalram", "Show Total RAM Size", kShowTotalRAMSize},
    {"strtod", "String To Decimal/Hex Convert", kStringToDecimalHexTest},
    {"shutdown", "Shutdown And Reboot OS", kShutdown},
};

//=========================================================================
// 실제 쉘을 구성하는 코드
//=========================================================================
// 쉘의 메인 루프
void kStartConsoleShell(void) {
    char vcCommandBuffer[CONSOLESHELL_MAXCOMMANDBUFFERCOUNT];
    int iCommandBufferIndex = 0;
    BYTE bKey;
    int iCursorX, iCursorY;

    // 프롬프트 출력
    kPrintf(CONSOLESHELL_PROMPTMESSAGE);

    while(1) {
        // 키가 수신될 때까지 대기
        bKey = kGetCh();
        // Backspace 키 처리
        if(bKey == KEY_BACKSPACE) {
            if(iCommandBufferIndex > 0) {
                // 현재 커서 위치를 얻어서 한 문자 앞으로 이동한 다음 공백을 출력하고 커맨드 버퍼에서 마지막 문자 삭제
                kGetCursor(&iCursorX, &iCursorY);
                kPrintStringXY(iCursorX - 1, iCursorY, " ");
                kSetCursor(iCursorX - 1, iCursorY);
                iCommandBufferIndex--;
            }
        }
        // 엔터키 처리
        else if(bKey == KEY_ENTER) {
            kPrintf("\n");

            if(iCommandBufferIndex > 0) {
                // 커맨드 버퍼에 있는 명령을 실행
                vcCommandBuffer[iCommandBufferIndex] = '\0';
                kExecuteCommand(vcCommandBuffer);
            }

            // 프롬프트 출력 및 커맨드 버퍼 초기화
            kPrintf("%s", CONSOLESHELL_PROMPTMESSAGE);
            kMemSet(vcCommandBuffer, '\0', CONSOLESHELL_MAXCOMMANDBUFFERCOUNT);
            iCommandBufferIndex = 0;
        }
        // Shift 키, Caps Lock, Num Lock, Scroll Lock은 무시
        else if((bKey == KEY_LSHIFT) || (bKey == KEY_RSHIFT) || (bKey == KEY_CAPSLOCK) || (bKey == KEY_NUMLOCK) || (bKey == KEY_SCROLLLOCK)) {
            ;
        }else {
            // TAB은 공백으로 전환
            if(bKey == KEY_TAB) {
                bKey = " ";
            }

            // 버퍼에 공간이 남아 있을 때만 가능
            if(iCommandBufferIndex < CONSOLESHELL_MAXCOMMANDBUFFERCOUNT) {
                vcCommandBuffer[iCommandBufferIndex++] = bKey;
                kPrintf("%c", bKey);
            }
        }
    }
}

// 커맨드 버퍼에 있는 커맨드를 비교하여 해당 커맨드를 처리하는 함수를 수행
void kExecuteCommand(const char *pcCommandBuffer) {
    int i, iSpaceIndex;
    int iCommandBufferLength, iCommandLength;
    int iCount;

    // 공백으로 구분된 커맨드를 추출
    iCommandBufferLength = kStrLen(pcCommandBuffer);
    for(iSpaceIndex=0; iSpaceIndex<iCommandBufferLength; iSpaceIndex++) {
        if(pcCommandBuffer[iSpaceIndex] == ' ') {
            break;
        }
    }

    // 커맨드 테이블을 검사해서 같은 이름의 커맨드가 있는지 확인
    iCount = sizeof(gs_vstCommandTable) / sizeof(SHELLCOMMANDENTRY);
    for(i=0; i<iCount; i++) {
        iCommandLength = kStrLen(gs_vstCommandTable[i].pcCommand);
        // 커맨드의 길이와 내용이 완전히 일치하는지 검사
        if((iCommandLength == iSpaceIndex) && (kMemCmp(gs_vstCommandTable[i].pcCommand, pcCommandBuffer, iSpaceIndex) == 0)) {
            gs_vstCommandTable[i].pfFunction(pcCommandBuffer + iSpaceIndex + 1);
            break;
        }
    }

    // 리스트에서 찾을 수 없다면 에러 출력
    if(i >= iCount) {
        kPrintf("'%s' is not found.\n", pcCommandBuffer);
    }
}

//=========================================================================
// 커맨드를 처리하는 코드
//=========================================================================
// 쉘 도움말을 출력
void kHelp(const char *pcCommandBuffer) {
    int i;
    int iCount;
    int iCursorX, iCursorY;
    int iLength, iMaxCommandLength = 0;

    kPrintf("=========================================================\n");
    kPrintf("                     MINT64 Shell Help                   \n");
    kPrintf("=========================================================\n");

    iCount = sizeof(gs_vstCommandTable) / sizeof(SHELLCOMMANDENTRY);

    // 가장 긴 커맨드의 길이를 계산
    for(i=0; i<iCount; i++) {
        iLength = kStrLen(gs_vstCommandTable[i].pcCommand);
        if(iLength > iMaxCommandLength) {
            iMaxCommandLength = iLength;
        }
    }

    // 도움말 출력
    for(i=0; i<iCount; i++) {
        kPrintf("%s", gs_vstCommandTable[i].pcCommand);
        kGetCursor(&iCursorX, &iCursorY);
        kSetCursor(iMaxCommandLength, iCursorY);
        printf("  - %s\n", gs_vstCommandTable[i].pcHelp);
    }
}

// 화면을 지움
void kCls(const char *pcParameterBuffer) {
    // 맨 윗줄은 디버깅 용으로 사용하므로 화면을 지운 후 라인 1로 커서 이동
    kClearScreen();
    kSetCursor(0, 1);
}