#include <stdarg.h>
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"

// 콘솔의 정보를 관리하는 자료구조
CONSOLEMANAGER gs_stConsoleManager = {0, };

// 커서의 위치를 설정
// 문자를 출력할 위치도 같이 설정
void kSetCursor(int iX, int iY) {
    int iLinearValue;

    // 커서의 위치를 계산
    iLinearValue = iY * CONSOLE_WIDTH + iX;

    // CRTC 컨트롤 어드레스 레지스터(포트 0x3D4)에 0x0E를 전송하여 상위 커서 위치 레지스터를 선택
    kOutPortByte(VGA_PORT_INDEX, VGA_INDEX_UPPERCURSOR);
    // CRTC 컨트롤 데이터 레지스터(포트 0x3D5)에 커서의 상위 바이트를 출력
    kOutPortByte(VGA_PORT_DATA, iLinearValue >> 8);

    // CRTC 컨트롤 어드레스 레지스터(포트 0x3D4)에 0x0F를 전송하여 하위 커서 위치 레지스터를 선택
    kOutPortByte(VGA_PORT_INDEX, VGA_INDEX_LOWERCURSOR);
    // CRTC 컨트롤 데이터 레지스터(포트 0x3D5)에 커서의 하위 바이트를 출력
    kOutPortByte(VGA_PORT_DATA, iLinearValue & 0xFF);
}

// printf 함수의 내부 구현
void kPrintf(const char *pcFormatString, ...) {
    va_list ap;
    char vcBuffer[1024];
    int iNextPrintOffset;
    
    // 가변 인자 리스트를 사용해서 vsprintf()로 처리
    va_start(ap, pcFormatString);
    kVSPrintf(vcBuffer, pcFormatString, ap);
    va_end(ap);

    // 포맷 문자열을 화면에 출력
    iNextPrintOffset = kConsolePrintString(vcBuffer);

    // 커서의 위치를 업데이트
    kSetCursor(iNextPrintOffset % CONSOLE_WIDTH, iNextPrintOffset / CONSOLE_WIDTH);
}

// \n, \t 같은 문자가 포함된 문자열을 출력한 후 화면상의 다음 출력할 위치를 반환
int kConsolePrintString(const char *pcBuffer) {
    CHARACTER *pstScreen = (CHARACTER *)CONSOLE_VIDEOMEMORYADDRESS;
    int i, j;
    int iLength;
    int iPrintOffset;

    // 문자열을 출력할 위치를 저장
    iPrintOffset = gs_stConsoleManager.iCurrentPrintOffset;

    // 문자열의 길이만큼 화면에 출력
    iLength = kStrLen(pcBuffer);
    for(i=0; i<iLength; i++) {
        // 줄바꿈 처리
        if(pcBuffer[i] == '\n') {
            // 출력할 위치를 80의 배수 컬럼으로 옮김
            // 현재 라인 남은 문자열 수만큼 더해서 다음 라인으로 위치시킴
            iPrintOffset += (CONSOLE_WIDTH - (iPrintOffset % CONSOLE_WIDTH));
        }
        // 탭 처리
        else if(pcBuffer[i] == '\t') {
            // 출력할 위치를 8의 배수 컬럼으로 옮김
            iPrintOffset += (8 - (iPrintOffset % 8));
        }
        // 일반 문자열 출력
        else {
            // 비디오 메모리에 문자와 속성을 설정하여 문자를 출력하고 출력할 위치를 다음으로 이동
            pstScreen[iPrintOffset].bCharactor = pcBuffer[i];
            pstScreen[iPrintOffset].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
            iPrintOffset++;
        }
        
        // 출력할 위치가 화면의 최대값(80 * 25)을 벗어났으면 스크롤 처리
        if(iPrintOffset >= (CONSOLE_HEIGHT * CONSOLE_WIDTH)) {
            // 가장 윗줄을 제외한 나머지를 한 줄 위로 복사
            kMemCpy(CONSOLE_VIDEOMEMORYADDRESS, CONSOLE_VIDEOMEMORYADDRESS + CONSOLE_WIDTH * sizeof(CHARACTER), (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH * sizeof(CHARACTER));

            // 가장 마지막 라인은 공백으로 채움
            for(j=(CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH; j<(CONSOLE_HEIGHT * CONSOLE_WIDTH); j++) {
                // 공백 출력
                pstScreen[j].bCharactor = ' ';
                pstScreen[j].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
            }

            // 출력할 위치를 가장 아래쪽 라인의 처음으로 설정
            iPrintOffset = (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH;
        }
    }
    return iPrintOffset;
}