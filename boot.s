/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */
 
/* 
Declare a multiboot header that marks the program as a kernel. These are magic
values that are documented in the multiboot standard. The bootloader will
search for this signature in the first 8 KiB of the kernel file, aligned at a
32-bit boundary. The signature is in its own section so the header can be
forced to be within the first 8 KiB of the kernel file.
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 
/*
The multiboot standard does not define the value of the stack pointer register
(esp) and it is up to the kernel to provide a stack. This allocates room for a
small stack by creating a symbol at the bottom of it, then allocating 16384
bytes for it, and finally creating a symbol at the top. The stack grows
downwards on x86. The stack is in its own section so it can be marked nobits,
which means the kernel file is smaller because it does not contain an
uninitialized stack. The stack on x86 must be 16-byte aligned according to the
System V ABI standard and de-facto extensions. The compiler will assume the
stack is properly aligned and failure to align the stack will result in
undefined behavior.
*/
.section .bss
 
.align 16
stack_bottom:
.skip 16384
stack_top:


 
// multiboot supporting bootstrap for a 32 bit OS kernel in assembly that enters 32 bit mode and calls an external C function named "kernel entry"
.section .text
 
.align 16
.global _entry
 
.type _entry, @function
.size _entry, 0x10
 
.global _start
 
.type _start, @function
.size _start, 0x10
 
.global _entry
 
.text
.globl _start
 
_start:
 
    //set up the stack pointer
    movl $stack_top, %esp
    //save the return address
    pushl %ebp

    //save the frame pointer
    pushl %ebx
    
    // call kernel_entry
    call kernel_entry

_loop:
    jmp _loop
