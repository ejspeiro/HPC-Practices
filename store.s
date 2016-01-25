	.file	"store.c"
	.text
	.p2align 4,,15
	.globl	store
	.type	store, @function
store:
.LFB0:
	.cfi_startproc
	movsd	(%rdi), %xmm0
	addsd	(%rsi), %xmm0
	movsd	%xmm0, (%rdx)
	ret
	.cfi_endproc
.LFE0:
	.size	store, .-store
	.ident	"GCC: (SUSE Linux) 4.8.3 20140627 [gcc-4_8-branch revision 212064]"
	.section	.note.GNU-stack,"",@progbits
