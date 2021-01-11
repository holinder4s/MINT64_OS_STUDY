[BITS 32]              ; 이하의 코드는 32비트 코드로 설정

; C 언어에서 호출할 수 있도록 이름을 노출함(Export)
global kReadCPUID

SECTION .text       ; text 섹션(세그먼트)을 정의

; CPUID를 반환
;   PARAM: DWORD dwEAX, DWORD *pdwEAX, DWORD *pdwEBX, DWORD *pdwECX, DWORD *pdwEDX
kReadCPUID:
    push ebp        ; 베이스 포인터 레지스터(EBP)를 스택에 삽입
    mov ebp, esp    ; 베이스 포인터 레지스터(EBP)에 스택 포인터 레지스터(ESP)의 값을 설정
    push eax        ; 함수에서 임시로 사용하는 레지스터로 함수의 마지막 부분에서 스택에 삽입된 값을 꺼내 원래 값으로 복원
    push ebx
    push ecx
    push edx
    push esi

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;   EAX 레지스터의 값으로 CPUID 명령어 실행
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov eax, dword [ebp+8]      ; 파라미터 1(dwEAX)를 EAX 레지스터에 저장
    cpuid                       ; CPUID 명령어 실행

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;   반환된 값을 파라미터에 저장
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;   *pdwEAX
    mov esi, dword [ebp+12]     ; 파라미터 2(pdwEAX)를 ESI 레지스터에 저장
    mov dword [esi], eax        ; pdwEAX가 포인터이므로 포인터가 가리키는 어드레스에 EAX 레지스터의 값을 저장

    ;   *pdwEBX
    mov esi, dword [ebp+16]     ; 파라미터 3(pdwEBX)를 ESI 레지스터에 저장
    mov dword [esi], ebx        ; pdwEBX가 포인터이므로 포인터가 가리키는 어드레스에 EBX 레지스터의 값을 저장

    ;   *pdwECX
    mov esi, dword [ebp+20]     ; 파라미터 4(pdwECX)를 ESI 레지스터에 저장
    mov dword [esi], ecx        ; pdwECX가 포인터이므로 포인터가 가리키는 어드레스에 ECX 레지스터의 값을 저장

    ;   *pdwEDX
    mov esi, dword [ebp+24]     ; 파라미터 5(pdwEDX)를 ESI 레지스터에 저장
    mov dword [esi], edx        ; pdwEDX가 포인터이므로 포인터가 가리키는 어드레스에 EDX 레지스터의 값을 저장

    pop esi     ; 함수에서 사용이 끝난 ESI 레지스터부터 EBP 레지스터까지를 스택에 삽입된 값을 이용해서 복원
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret         ; 함수를 호출한 다음 코드의 위치로 복귀

; IA-32e 모드로 전환하고 64비트 커널을 수행
;   PARAM: 없음
kSwitchAndExecute64bitKernel:
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;   CR4 컨트롤 레지스터의 PAE 비트를 1로 설정
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov eax, cr4        ; CR4 컨트롤 레지스터의 값을 EAX 레지스터에 저장
    or eax, 0x20        ; PAE 비트(비트 5)를 1로 설정
    mov cr4, eax        ; PAE 비트가 1로 설정된 값을 CR4 컨트롤 레지스터에 저장
    