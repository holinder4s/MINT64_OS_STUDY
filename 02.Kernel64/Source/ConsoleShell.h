#ifndef __CONSOLESHELL_H__
#define __CONSOLESHELL_H__

#include "Types.h"

// 매크로
#define CONSOLESHELL_MAXCOMMANDBUFFERCOUNT  300
#define CONSOLESHELL_PROMPTMESSAGE          "MINT64>"

// 함수
// 실제 쉘 코드
void kStartConsoleShell(void);

#endif /*__CONSOLESHELL_H__*/