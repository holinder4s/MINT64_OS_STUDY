#include "InterruptHandler.h"
#include "PIC.h"

// 공통으로 사용하는 예외 핸들러
void kCommonExceptionHandler(int iVectorNumber, QWORD qwErrorCode) {
    char vcBuffer[3] = {0,};

    // 인터럽트 벡터를 화면 오른쪽 위에 2자리 정수로 출력
    vcBuffer[0] = '0' + iVectorNumber / 10;
    vcBuffer[1] = '0' + iVectorNumber % 10;

    kPrintString(0, 0, "==================================================");
    kPrintString(0, 1, "                 Exception Occur~!!!              ");
    kPrintString(0, 2, "                    Vector:                       ");
    kPrintString(27, 2, vcBuffer);
    kPrintString(0, 3, "==================================================");

    while(1);
}