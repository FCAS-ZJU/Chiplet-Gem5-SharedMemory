	.file	"lastone.cpp"
	.text
	.section	.rodata
	.type	_ZStL19piecewise_construct, @object
	.size	_ZStL19piecewise_construct, 1
_ZStL19piecewise_construct:
	.zero	1
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.globl	mutex
	.bss
	.align 32
	.type	mutex, @object
	.size	mutex, 40
mutex:
	.zero	40
	.globl	lock
	.align 32
	.type	lock, @object
	.size	lock, 56
lock:
	.zero	56
	.globl	A
	.align 32
	.type	A, @object
	.size	A, 400
A:
	.zero	400
	.globl	B
	.align 4
	.type	B, @object
	.size	B, 4
B:
	.zero	4
	.globl	_Z3msgB5cxx11
	.align 32
	.type	_Z3msgB5cxx11, @object
	.size	_Z3msgB5cxx11, 32
_Z3msgB5cxx11:
	.zero	32
	.text
	.globl	_Z6write0v
	.type	_Z6write0v, @function
_Z6write0v:
.LFB5530:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -4(%rbp)
.L3:
	cmpl	$1, -4(%rbp)
	jg	.L4
	movl	$1, A(%rip)
	movl	$1, 64+A(%rip)
	movl	$1, 128+A(%rip)
	movl	$1, 192+A(%rip)
	mfence
	addl	$1, -4(%rbp)
	jmp	.L3
.L4:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5530:
	.size	_Z6write0v, .-_Z6write0v
	.globl	_Z9makenoisePv
	.type	_Z9makenoisePv, @function
_Z9makenoisePv:
.LFB5531:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movl	$0, -20(%rbp)
.L7:
	movl	-20(%rbp), %eax
	movslq	%eax, %rbx
	leaq	_Z3msgB5cxx11(%rip), %rdi
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6lengthEv@PLT
	cmpq	%rax, %rbx
	setb	%al
	testb	%al, %al
	je	.L6
	movl	$1, B(%rip)
	addl	$1, -20(%rbp)
	jmp	.L7
.L6:
	nop
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5531:
	.size	_Z9makenoisePv, .-_Z9makenoisePv
	.section	.rodata
.LC0:
	.string	"sched_setaffinity"
	.text
	.globl	_Z5writePv
	.type	_Z5writePv, @function
_Z5writePv:
.LFB5532:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$184, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -184(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-160(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	movq	$0, -168(%rbp)
	movq	-168(%rbp), %rax
	cmpq	$1023, %rax
	ja	.L10
	movq	-168(%rbp), %rax
	shrq	$6, %rax
	leaq	0(,%rax,8), %rdx
	leaq	-160(%rbp), %rcx
	addq	%rcx, %rdx
	movq	(%rdx), %rdx
	movq	-168(%rbp), %rcx
	andl	$63, %ecx
	movl	$1, %esi
	salq	%cl, %rsi
	movq	%rsi, %rcx
	leaq	0(,%rax,8), %rsi
	leaq	-160(%rbp), %rax
	addq	%rsi, %rax
	orq	%rcx, %rdx
	movq	%rdx, (%rax)
.L10:
	call	pthread_self@PLT
	movq	%rax, %rcx
	leaq	-160(%rbp), %rax
	movq	%rax, %rdx
	movl	$128, %esi
	movq	%rcx, %rdi
	call	pthread_setaffinity_np@PLT
	shrl	$31, %eax
	testb	%al, %al
	je	.L11
	leaq	.LC0(%rip), %rdi
	call	perror@PLT
.L11:
	movl	$0, -172(%rbp)
.L14:
	movl	-172(%rbp), %eax
	movslq	%eax, %rbx
	leaq	_Z3msgB5cxx11(%rip), %rdi
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6lengthEv@PLT
	cmpq	%rax, %rbx
	setb	%al
	testb	%al, %al
	je	.L12
	movl	-172(%rbp), %eax
	cltq
	movq	%rax, %rsi
	leaq	_Z3msgB5cxx11(%rip), %rdi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEixEm@PLT
	movzbl	(%rax), %eax
	cmpb	$48, %al
	sete	%al
	testb	%al, %al
	je	.L13
	call	_Z6write0v
.L13:
	movl	$1000000, %edi
	call	usleep@PLT
	addl	$1, -172(%rbp)
	jmp	.L14
.L12:
	nop
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L15
	call	__stack_chk_fail@PLT
.L15:
	addq	$184, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5532:
	.size	_Z5writePv, .-_Z5writePv
	.section	.rodata
.LC1:
	.string	"%lu,"
	.text
	.globl	_Z4readPv
	.type	_Z4readPv, @function
_Z4readPv:
.LFB5533:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$248, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -248(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-160(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	movq	$1, -232(%rbp)
	movq	-232(%rbp), %rax
	cmpq	$1023, %rax
	ja	.L18
	movq	-232(%rbp), %rax
	shrq	$6, %rax
	leaq	0(,%rax,8), %rdx
	leaq	-160(%rbp), %rcx
	addq	%rcx, %rdx
	movq	(%rdx), %rdx
	movq	-232(%rbp), %rcx
	andl	$63, %ecx
	movl	$1, %esi
	salq	%cl, %rsi
	movq	%rsi, %rcx
	leaq	0(,%rax,8), %rsi
	leaq	-160(%rbp), %rax
	addq	%rsi, %rax
	orq	%rcx, %rdx
	movq	%rdx, (%rax)
.L18:
	call	pthread_self@PLT
	movq	%rax, %rcx
	leaq	-160(%rbp), %rax
	movq	%rax, %rdx
	movl	$128, %esi
	movq	%rcx, %rdi
	call	pthread_setaffinity_np@PLT
	shrl	$31, %eax
	testb	%al, %al
	je	.L19
	leaq	.LC0(%rip), %rdi
	call	perror@PLT
.L19:
	movl	$0, -240(%rbp)
.L25:
	movl	-240(%rbp), %eax
	movslq	%eax, %rbx
	leaq	_Z3msgB5cxx11(%rip), %rdi
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6lengthEv@PLT
	cmpq	%rax, %rbx
	setb	%al
	testb	%al, %al
	je	.L20
	movl	$400, %edi
	call	usleep@PLT
	mfence
	lfence
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -224(%rbp)
	lfence
	movl	A(%rip), %eax
	movl	%eax, -236(%rbp)
	movl	64+A(%rip), %eax
	movl	%eax, -236(%rbp)
	movl	128+A(%rip), %eax
	movl	%eax, -236(%rbp)
	movl	192+A(%rip), %eax
	movl	%eax, -236(%rbp)
	lfence
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -216(%rbp)
	lfence
	movq	-216(%rbp), %rax
	subq	-224(%rbp), %rax
	movq	%rax, -208(%rbp)
	mfence
	lfence
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -224(%rbp)
	lfence
	lfence
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -216(%rbp)
	lfence
	movq	-216(%rbp), %rax
	subq	-224(%rbp), %rax
	movq	%rax, -200(%rbp)
	movq	-208(%rbp), %rax
	subq	-200(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	A(%rip), %rax
	movq	%rax, -192(%rbp)
	movq	-192(%rbp), %rax
	clflush	(%rax)
	leaq	64+A(%rip), %rax
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %rax
	clflush	(%rax)
	leaq	128+A(%rip), %rax
	movq	%rax, -176(%rbp)
	movq	-176(%rbp), %rax
	clflush	(%rax)
	leaq	192+A(%rip), %rax
	movq	%rax, -168(%rbp)
	movq	-168(%rbp), %rax
	clflush	(%rax)
	movl	$999600, %edi
	call	usleep@PLT
	addl	$1, -240(%rbp)
	jmp	.L25
.L20:
	nop
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L26
	call	__stack_chk_fail@PLT
.L26:
	addq	$248, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5533:
	.size	_Z4readPv, .-_Z4readPv
	.globl	main
	.type	main, @function
main:
.LFB5534:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-16(%rbp), %rax
	movl	$0, %ecx
	leaq	_Z4readPv(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L29
	call	__stack_chk_fail@PLT
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5534:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC2:
	.string	"01010101010101010101010101010101010101010101010101"
	.text
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB6037:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA6037
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	cmpl	$1, -36(%rbp)
	jne	.L30
	cmpl	$65535, -40(%rbp)
	jne	.L30
	leaq	_ZStL8__ioinit(%rip), %rdi
.LEHB0:
	call	_ZNSt8ios_base4InitC1Ev@PLT
.LEHE0:
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rax
	movq	%rax, %rdi
	call	__cxa_atexit@PLT
	leaq	-25(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcEC1Ev@PLT
	leaq	-25(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rsi
	leaq	_Z3msgB5cxx11(%rip), %rdi
.LEHB1:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_@PLT
.LEHE1:
	leaq	-25(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev@PLT
	leaq	__dso_handle(%rip), %rdx
	leaq	_Z3msgB5cxx11(%rip), %rsi
	movq	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@GOTPCREL(%rip), %rax
	movq	%rax, %rdi
	call	__cxa_atexit@PLT
	jmp	.L30
.L34:
	movq	%rax, %rbx
	leaq	-25(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev@PLT
	movq	%rbx, %rax
	movq	%rax, %rdi
.LEHB2:
	call	_Unwind_Resume@PLT
.LEHE2:
.L30:
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L33
	call	__stack_chk_fail@PLT
.L33:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6037:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA6037:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6037-.LLSDACSB6037
.LLSDACSB6037:
	.uleb128 .LEHB0-.LFB6037
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB6037
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L34-.LFB6037
	.uleb128 0
	.uleb128 .LEHB2-.LFB6037
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSE6037:
	.text
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I_mutex, @function
_GLOBAL__sub_I_mutex:
.LFB6038:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6038:
	.size	_GLOBAL__sub_I_mutex, .-_GLOBAL__sub_I_mutex
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_mutex
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
