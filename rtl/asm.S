.text

/*
 * uint64_t roundup_power_of_two(uint64_t);
 */
.globl roundup_power_of_two
roundup_power_of_two:
    sub     $1, %rdi
    bsr     %rdi, %rcx
    mov     $2, %rax
    shl     %cl, %rax
    ret

/*
 * uint8_t msb_index(uint64_t);
 */
.globl msb_index
msb_index:
    bsr     %rdi, %rax
    ret
