	.file	"prod-cons.c"
	.text
	.comm	funcArray,160,32
	.section	.rodata
	.align 8
.LC0:
	.string	"***ERROR***: An error occured while creating the list."
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$160, %rsp
	movl	%edi, -148(%rbp)
	movq	%rsi, -160(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	testFunction0(%rip), %rax
	movq	%rax, funcArray(%rip)
	leaq	testFunction1(%rip), %rax
	movq	%rax, 16+funcArray(%rip)
	leaq	testFunction2(%rip), %rax
	movq	%rax, 32+funcArray(%rip)
	leaq	testFunction3(%rip), %rax
	movq	%rax, 48+funcArray(%rip)
	leaq	testFunction4(%rip), %rax
	movq	%rax, 64+funcArray(%rip)
	leaq	testFunction5(%rip), %rax
	movq	%rax, 80+funcArray(%rip)
	leaq	testFunction6(%rip), %rax
	movq	%rax, 96+funcArray(%rip)
	leaq	testFunction7(%rip), %rax
	movq	%rax, 112+funcArray(%rip)
	leaq	testFunction8(%rip), %rax
	movq	%rax, 128+funcArray(%rip)
	leaq	testFunction9(%rip), %rax
	movq	%rax, 144+funcArray(%rip)
	movl	$0, %eax
	call	queueInit
	movq	%rax, -136(%rbp)
	cmpq	$0, -136(%rbp)
	jne	.L2
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$1, %edi
	call	exit@PLT
.L2:
	movl	$0, -140(%rbp)
	jmp	.L3
.L4:
	leaq	-112(%rbp), %rax
	movl	-140(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	leaq	(%rax,%rdx), %rdi
	movq	-136(%rbp), %rax
	movq	%rax, %rcx
	leaq	producer(%rip), %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	addl	$1, -140(%rbp)
.L3:
	cmpl	$3, -140(%rbp)
	jle	.L4
	movl	$0, -140(%rbp)
	jmp	.L5
.L6:
	leaq	-80(%rbp), %rax
	movl	-140(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	leaq	(%rax,%rdx), %rdi
	movq	-136(%rbp), %rax
	movq	%rax, %rcx
	leaq	consumer(%rip), %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	addl	$1, -140(%rbp)
.L5:
	cmpl	$7, -140(%rbp)
	jle	.L6
	movl	$0, -140(%rbp)
	jmp	.L7
.L8:
	movl	-140(%rbp), %eax
	cltq
	movq	-112(%rbp,%rax,8), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	addl	$1, -140(%rbp)
.L7:
	cmpl	$3, -140(%rbp)
	jle	.L8
	movl	$0, -140(%rbp)
	jmp	.L9
.L10:
	movl	-140(%rbp), %eax
	cltq
	movq	-80(%rbp,%rax,8), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	addl	$1, -140(%rbp)
.L9:
	cmpl	$7, -140(%rbp)
	jle	.L10
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L12
	call	__stack_chk_fail@PLT
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
.LC1:
	.string	"%d\n"
	.text
	.globl	producer
	.type	producer, @function
producer:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-40(%rbp), %rax
	movq	%rax, -24(%rbp)
	movl	$0, -32(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	%eax, %edi
	call	srand@PLT
	movl	$0, -32(%rbp)
	jmp	.L14
.L17:
	movq	-24(%rbp), %rax
	movq	184(%rax), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock@PLT
	jmp	.L15
.L16:
	movq	-24(%rbp), %rax
	movq	184(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	192(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pthread_cond_wait@PLT
.L15:
	movq	-24(%rbp), %rax
	movl	176(%rax), %eax
	testl	%eax, %eax
	jne	.L16
	call	rand@PLT
	movl	%eax, %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-28(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rdx
	leaq	funcArray(%rip), %rax
	movq	(%rdx,%rax), %rcx
	movq	8(%rdx,%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	queueAdd
	movq	-24(%rbp), %rax
	movq	184(%rax), %rax
	movq	%rax, %rdi
	call	pthread_mutex_unlock@PLT
	movq	-24(%rbp), %rax
	movq	200(%rax), %rax
	movq	%rax, %rdi
	call	pthread_cond_signal@PLT
	movl	-32(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -32(%rbp)
.L14:
	movl	-32(%rbp), %eax
	cmpl	$9, %eax
	jle	.L17
	movl	$0, %edi
	call	pthread_exit@PLT
	.cfi_endproc
.LFE7:
	.size	producer, .-producer
	.globl	consumer
	.type	consumer, @function
consumer:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-56(%rbp), %rax
	movq	%rax, -48(%rbp)
.L22:
	movq	-48(%rbp), %rax
	movq	184(%rax), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock@PLT
	jmp	.L20
.L21:
	movq	-48(%rbp), %rax
	movq	184(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	200(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pthread_cond_wait@PLT
.L20:
	movq	-48(%rbp), %rax
	movl	180(%rax), %eax
	testl	%eax, %eax
	jne	.L21
	leaq	-32(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	queueDel
	movq	-48(%rbp), %rax
	movq	184(%rax), %rax
	movq	%rax, %rdi
	call	pthread_mutex_unlock@PLT
	movq	-48(%rbp), %rax
	movq	192(%rax), %rax
	movq	%rax, %rdi
	call	pthread_cond_signal@PLT
	movq	-32(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rdx, %rdi
	call	*%rax
	jmp	.L22
	.cfi_endproc
.LFE8:
	.size	consumer, .-consumer
	.globl	queueInit
	.type	queueInit, @function
queueInit:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$208, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L25
	movl	$0, %eax
	jmp	.L26
.L25:
	movq	-8(%rbp), %rax
	movq	$0, 160(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 168(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 176(%rax)
	movq	-8(%rbp), %rax
	movl	$1, 180(%rax)
	movl	$40, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 184(%rax)
	movq	-8(%rbp), %rax
	movq	184(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_mutex_init@PLT
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 192(%rax)
	movq	-8(%rbp), %rax
	movq	192(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_cond_init@PLT
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 200(%rax)
	movq	-8(%rbp), %rax
	movq	200(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_cond_init@PLT
	movq	-8(%rbp), %rax
.L26:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	queueInit, .-queueInit
	.globl	queueAdd
	.type	queueAdd, @function
queueAdd:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, %rax
	movq	%rdx, %rcx
	movq	%rcx, %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	168(%rax), %rdx
	movq	-8(%rbp), %rax
	salq	$4, %rdx
	leaq	(%rax,%rdx), %rcx
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-8(%rbp), %rax
	movq	168(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 168(%rax)
	movq	-8(%rbp), %rax
	movq	168(%rax), %rax
	cmpq	$10, %rax
	jne	.L28
	movq	-8(%rbp), %rax
	movq	$0, 168(%rax)
.L28:
	movq	-8(%rbp), %rax
	movq	168(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	160(%rax), %rax
	cmpq	%rax, %rdx
	jne	.L29
	movq	-8(%rbp), %rax
	movl	$1, 176(%rax)
.L29:
	movq	-8(%rbp), %rax
	movl	$0, 180(%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	queueAdd, .-queueAdd
	.globl	queueDel
	.type	queueDel, @function
queueDel:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	160(%rax), %rdx
	movq	-16(%rbp), %rcx
	movq	-8(%rbp), %rax
	salq	$4, %rdx
	addq	%rdx, %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-8(%rbp), %rax
	movq	160(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 160(%rax)
	movq	-8(%rbp), %rax
	movq	160(%rax), %rax
	cmpq	$10, %rax
	jne	.L32
	movq	-8(%rbp), %rax
	movq	$0, 160(%rax)
.L32:
	movq	-8(%rbp), %rax
	movq	160(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	168(%rax), %rax
	cmpq	%rax, %rdx
	jne	.L33
	movq	-8(%rbp), %rax
	movl	$1, 180(%rax)
.L33:
	movq	-8(%rbp), %rax
	movl	$0, 176(%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	queueDel, .-queueDel
	.globl	queueDelete
	.type	queueDelete, @function
queueDelete:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	184(%rax), %rax
	movq	%rax, %rdi
	call	pthread_mutex_destroy@PLT
	movq	-8(%rbp), %rax
	movq	184(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-8(%rbp), %rax
	movq	192(%rax), %rax
	movq	%rax, %rdi
	call	pthread_cond_destroy@PLT
	movq	-8(%rbp), %rax
	movq	192(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-8(%rbp), %rax
	movq	200(%rax), %rax
	movq	%rax, %rdi
	call	pthread_cond_destroy@PLT
	movq	-8(%rbp), %rax
	movq	200(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	queueDelete, .-queueDelete
	.section	.rodata
.LC5:
	.string	"testFunction0"
	.text
	.globl	testFunction0
	.type	testFunction0, @function
testFunction0:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L37
.L38:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	sin@PLT
	movapd	%xmm0, %xmm1
	movsd	-24(%rbp), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L37:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L38
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC5(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L40
	call	__stack_chk_fail@PLT
.L40:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	testFunction0, .-testFunction0
	.section	.rodata
.LC6:
	.string	"testFunction1"
	.text
	.globl	testFunction1
	.type	testFunction1, @function
testFunction1:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L42
.L43:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	cos@PLT
	movapd	%xmm0, %xmm1
	movsd	-24(%rbp), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L42:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L43
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L45
	call	__stack_chk_fail@PLT
.L45:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	testFunction1, .-testFunction1
	.section	.rodata
.LC7:
	.string	"testFunction2"
	.text
	.globl	testFunction2
	.type	testFunction2, @function
testFunction2:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L47
.L48:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	tan@PLT
	movapd	%xmm0, %xmm1
	movsd	-24(%rbp), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L47:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L48
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L50
	call	__stack_chk_fail@PLT
.L50:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	testFunction2, .-testFunction2
	.section	.rodata
.LC8:
	.string	"testFunction3"
	.text
	.globl	testFunction3
	.type	testFunction3, @function
testFunction3:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L52
.L53:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	sin@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	cos@PLT
	mulsd	-48(%rbp), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L52:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L53
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC8(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L55
	call	__stack_chk_fail@PLT
.L55:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	testFunction3, .-testFunction3
	.section	.rodata
.LC9:
	.string	"testFunction4"
	.text
	.globl	testFunction4
	.type	testFunction4, @function
testFunction4:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L57
.L58:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	sin@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	tan@PLT
	mulsd	-48(%rbp), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L57:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L58
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC9(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L60
	call	__stack_chk_fail@PLT
.L60:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	testFunction4, .-testFunction4
	.globl	testFunction5
	.type	testFunction5, @function
testFunction5:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L62
.L63:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	cos@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	tan@PLT
	mulsd	-48(%rbp), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L62:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L63
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L65
	call	__stack_chk_fail@PLT
.L65:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	testFunction5, .-testFunction5
	.section	.rodata
.LC10:
	.string	"testFunction6"
	.text
	.globl	testFunction6
	.type	testFunction6, @function
testFunction6:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L67
.L68:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	sin@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	sin@PLT
	mulsd	-48(%rbp), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L67:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L68
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC10(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L70
	call	__stack_chk_fail@PLT
.L70:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	testFunction6, .-testFunction6
	.section	.rodata
.LC11:
	.string	"testFunction7"
	.text
	.globl	testFunction7
	.type	testFunction7, @function
testFunction7:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L72
.L73:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	cos@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	cos@PLT
	mulsd	-48(%rbp), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L72:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L73
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC11(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L75
	call	__stack_chk_fail@PLT
.L75:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	testFunction7, .-testFunction7
	.section	.rodata
.LC12:
	.string	"testFunction8"
	.text
	.globl	testFunction8
	.type	testFunction8, @function
testFunction8:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L77
.L78:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	tan@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	tan@PLT
	mulsd	-48(%rbp), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L77:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L78
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC12(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L80
	call	__stack_chk_fail@PLT
.L80:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	testFunction8, .-testFunction8
	.section	.rodata
.LC13:
	.string	"testFunction9"
	.text
	.globl	testFunction9
	.type	testFunction9, @function
testFunction9:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	jmp	.L82
.L83:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movsd	-48(%rbp), %xmm0
	call	sin@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	cos@PLT
	movapd	%xmm0, %xmm2
	mulsd	-48(%rbp), %xmm2
	movsd	%xmm2, -48(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	call	tan@PLT
	mulsd	-48(%rbp), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-16(%rbp), %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
.L82:
	movsd	.LC4(%rip), %xmm0
	ucomisd	-16(%rbp), %xmm0
	ja	.L83
	leaq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC13(%rip), %rdi
	call	puts@PLT
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L85
	call	__stack_chk_fail@PLT
.L85:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	testFunction9, .-testFunction9
	.section	.rodata
	.align 8
.LC3:
	.long	0
	.long	1072693248
	.align 8
.LC4:
	.long	0
	.long	1081507840
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~19.10) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
