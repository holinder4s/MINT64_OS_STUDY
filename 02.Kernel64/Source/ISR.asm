[BITS 64]           ; 이하의 코드는 64비트 코드로 설정

SECTION .text       ; text 섹션(세그먼트)을 정의

; 콘텍스트를 저장하고 셀렉터를 교체하는 매크로
%macro KSAVECONTEXT 0       ; 파라미터를 전달받지 않는 KSAVECONTEXT 매크로 정의
    ; RBP 레지스터부터 GS 세그먼트 셀렉터까지 모두 스택에 삽입
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov ax, ds      ; DS 세그먼트 셀렉터와 ES 세그먼트 셀렉터는 스택에 직접 삽입할 수 없으므로, RAX 레지스터에 저장한 후 스택에 삽입
    push rax
    mov ax, es
    push rax
    push fs
    push gs

    ; 세그먼트 셀렉터 교체
    mov ax, 0x10    ; AX 레지스터에 커널 데이터 세그먼트 디스크립터 저장
    mov ds, ax      ; DS 세그먼트 셀렉터부터 FS 세그먼트 셀렉터까지 모두 커널 데이터 세그먼트로 교체
    mov es, ax
    mov gs, ax
    mov fs, ax
%endmacro           ; 매크로 끝

; 콘텍스트를 복원하는 매크로
%macro KLOADCONTEXT 0       ; 파라미터를 전달받지 않는 KLOADCONTEXT 매크로 정의
    ; GS 세그먼트 셀렉터부터 RBP 레지스터까지 모두 스택에서 꺼내 복원
    pop gs
    pop fs
    pop rax
    mov es, ax      ; ES 세그먼트 셀렉터와 DS 세그먼트 셀렉터는 스택에서 직접 꺼내 복원할 수 없으므로, RAX 레지스터에 저장한 뒤에 복원
    pop rax
    mov ds, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    pop rbp
%endmacro           ; 매크로 끝