.set IRQ_BASE, 0x20

.section .text

.extern _ZN4myos12hardwarecoms16InterruptManager15handleInterruptEhj

.macro handleException num
.global _ZN4myos12hardwarecoms16InterruptManager19handleException\num\()Ev
_ZN4myos12hardwarecoms16InterruptManager19handleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro handleInterruptRequest num
.global _ZN4myos12hardwarecoms16InterruptManager26handleInterruptRequest\num\()Ev
_ZN4myos12hardwarecoms16InterruptManager26handleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    push $0
    jmp int_bottom
.endm

handleInterruptRequest 0x00
handleInterruptRequest 0x01
handleInterruptRequest 0x0C

int_bottom:
    
    # pusha
    # pushl %ds
    # pushl %es
    # pushl %fs
    # pushl %gs
    
    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    pushl %esp
    push (interruptnumber)
    call _ZN4myos12hardwarecoms16InterruptManager15handleInterruptEhj
    # addl $5, %esp
    movl %eax, %esp


    # restore registers
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    
    popl %esi
    popl %edi
    popl %ebp

    # popl %gs
    # popl %fs
    # popl %es
    # popl %ds
    # popa

    add $4, %esp

.global _ZN4myos12hardwarecoms16InterruptManager22ignoreInterruptRequestEv
_ZN4myos12hardwarecoms16InterruptManager22ignoreInterruptRequestEv:

    iret

.data
    interruptnumber: .byte 0
