	movq	$12, %r8
	movq	$3, %r9
	imulq	%r8, %r9
	movq	%r9, %rdi
	call	printint
	movq	$18, %r8
	movq	$2, %r9
	subq	%r9, %r8
	movq	$4, %r8
	imulq	%r9, %r8
	movq	%r8, %rdi
	call	printint
