	.arch i8086,jumps
	.code16
	.att_syntax prefix
#NO_APP
	.global	i
	.data
	.p2align	1
	.type	i, @object
	.size	i, 2
i:
	.hword	9
	.comm	bytearr,256,1
	.global	str1
	.type	str1, @object
	.size	str1, 4
str1:
	.string	"Lol"
	.section	.rodata
.LC0:
	.string	"Idk %d"
	.text
	.global	main
	.type	main, @function
main:
	pushw	%bp
	movw	%sp,	%bp
	movw	%ss:i,	%ax
	pushw	%ax
	movw	$.LC0,	%ax
	pushw	%ax
	movw	$bytearr,	%ax
	pushw	%ax
	pushw	%ss
	popw	%ds
	call	sprintf
	addw	$6,	%sp
	movw	$4,	%ax
	pushw	%ax
	movw	$str1,	%ax
	pushw	%ax
	movw	$bytearr,	%ax
	pushw	%ax
	pushw	%ss
	popw	%ds
	call	memcpy
	addw	$6,	%sp
	movw	$5,	%ax
	movw	%ax,	%ax
	movw	%ax,	%ax
	movw	%bp,	%sp
	popw	%bp
	pushw	%ss
	popw	%ds
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 6.3.0"
