#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "AssemblyUtility.h"
#include "PIC.h"
#include "Console.h"
#include "ConsoleShell.h"

// Main 함수
void Main(void) {
    int iCursorX, iCursorY;

    // 콘솔을 먼저 초기화한 후, 다음 작업을 수행
    kInitializeConsole(0, 10);
    kPrintf("Switch To IA-32e Mode Success~!!\n");
    kPrintf("IA-32e C Language Kernel Start..............[PASS]\n");
    kPrintf("Initialize Console..........................[PASS]\n");
    
    // 부팅 상황을 화면에 출력
    kGetCursor(&iCursorX, &iCursorY);
    kPrintf("GDT Initialize And Switch For IA-32e Mode...[    ]");
    kInitializeGDTTableAndTSS();
    kLoadGDTR(GDTR_STARTADDRESS);
    kSetCursor(45, iCursorY++);
    kPrintf("PASS\n");

    kPrintf("TSS Segment Load............................[    ]");
    kLoadTR(GDT_TSSSEGMENT);
    kSetCursor(45, iCursorY++);
    kPrintf("PASS\n");

    kPrintf("IDT Initialize..............................[    ]");
    kInitializeIDTTables();
    kLoadIDTR(IDTR_STARTADDRESS);
    kSetCursor(45, iCursorY++);
    kPrintf("PASS\n");

    kPrintf("Total RAM Size Check........................[    ]");
    kCheckTotalRAMSize();
    kSetCursor(45, iCursorY++);
    kPrintf("PASS], Size = %d MB\n", kGetTotalRAMSize());

    kPrintf("Keyboard Activate And Queue Initialize......[    ]");

    // 키보드를 활성화
    if(kInitializeKeyboard() == TRUE) {
        kSetCursor(45, iCursorY++);
        kPrintf("PASS\n");
        kChangeKeyboardLED(FALSE, FALSE, FALSE);
    }else {
        kSetCursor(45, iCursorY++);
        kPrintf("FAIL\n");
        while(1);
    }

    kPrintf("PIC Controller And Interrupt Initialize.....[    ]");
    // PIC 컨트롤러 초기화 및 모든 인터럽트 활성화
    kInitializePIC();
    kMaskPICInterrupt(0);
    kEnableInterrupt();
    kSetCursor(45, iCursorY++);
    kPrintf("PASS\n");

    // 쉘을 시작
    kStartConsoleShell();
}