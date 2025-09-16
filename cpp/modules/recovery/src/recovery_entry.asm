; x64 MASM entry for Recovery HLE stub
option casemap:none

EXTERN GSCX_RecoveryMain:PROC

.code

; Exported symbol as initial entry (like BIOS vector)
PUBLIC GSCX_RecoveryEntry
GSCX_RecoveryEntry PROC
    ; Preserve non-volatile registers
    push rbx
    push rdi
    push rsi
    push rbp

    ; Call into C++ HLE main
    sub rsp, 32                ; shadow space
    call GSCX_RecoveryMain
    add rsp, 32

    ; Restore
    pop rbp
    pop rsi
    pop rdi
    pop rbx
    ret
GSCX_RecoveryEntry ENDP

END