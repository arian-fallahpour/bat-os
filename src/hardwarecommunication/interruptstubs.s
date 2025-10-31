.set IRQ_BASE, 0x20

.section .text

.extern _ZN5batos21hardwarecommunication16InterruptManager15handleInterruptEhj 

.global _ZN16InterruptManager22IgnoreInterruptRequestEv

.macro HandleException num
.global _ZN5batos21hardwarecommunication16InterruptManager16HandleException\num\()Ev
_ZN5batos21hardwarecommunication16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN5batos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN5batos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C

int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    push %esp
    push (interruptnumber)
    call _ZN5batos21hardwarecommunication16InterruptManager15handleInterruptEhj
    # addl $5, %esp
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

.global _ZN5batos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv
_ZN5batos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv:

    iret

.data
    interruptnumber: .byte 0