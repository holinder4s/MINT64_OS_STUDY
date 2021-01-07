[ORG 0x00]              ; 코드의 시작 어드레스를 0x00으로 설정
[BITS 16]               ; 이하의 코드는 16비트 코드로 설정

SECTION .text           ; text 섹션(세그먼트)을 정의


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   데이터 영역
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   아래의 데이터들을 8바이트에 맞춰 정렬하기 위해 추가
align 8, db 0


;   GDT 테이블 정의
GDT:
    ;   널(NULL) 디스크립터, 반드시 0으로 초기화해야 함
    NULLDescriptor:
        dw 0x0000
        dw 0x0000
        db 0x00
        db 0x00
        db 0x00
        db 0x00
    
    ;   보호 모드 커널용 코드 세그먼트 디스크립터
    CODEDESCRIPTOR:
        dw 0xFFFF           ; Limit [15:0]
        dw 0x0000           ; Base [15:0]
        db 0x00             ; Base [23:16]
        db 0x9A             ; P=1, DPL=0, COde Segment, Execute/Read
        db 0xCF             ; G=1, D=1, L=0, Limit [19:16]
        db 0x00             ; Base [31:24]
    
    ;   보호 모드 커널용 데이터 세그먼트 디스크립터
    DATADESCRIPTOR:
        dw 0xFFFF           ; Limit [15:0]
        dw 0x0000           ; Base [15:0]
        db 0x00             ; Base [23:16]
        db 0x92             ; P=1, DPL=0, Data Segment, Read/Write
        db 0xCF             ; G=1, D=1, L=0, Limit [19:16]
        db 0x00             ; Base [31:24]
GDTEND:

times 512 - ( $ - $$ )  db  0x00    ; 512바이트를 맞추기 위해 남은 부분을 0으로 채움