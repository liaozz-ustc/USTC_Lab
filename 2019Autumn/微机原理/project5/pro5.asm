;ml /c /coff pro4.asm
;Link /subsystem:console pro4.obj
.386
.model flat,stdcall
option casemap:none
include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\masm32.inc
includelib \masm32\lib\masm32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
include \masm32\include\msvcrt.inc
includelib \masm32\lib\msvcrt.lib
.data
Message db "Input:",0 
MessageSize = ($-Message)
Message2 db "result=%d$",0
Message2Size = ($-Message2)
ConsoleHandle DWORD 0
StdinHandle DWORD 0
BytesWritten DWORD ?
BytesRead DWORD ? 
szBuffer db 1024 dup(0)


num dw 0 ;保存输入的表达式的长度
i dw 0
j db 0
tmp dw 0
ten db 10
.code
start:
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov ConsoleHandle,eax ;输出句柄

invoke GetStdHandle,STD_INPUT_HANDLE
mov StdinHandle,eax ;输入句柄

invoke WriteConsole,\ ;输出提示,我们的数字输入要小于256
	    ConsoleHandle,\
	    offset Message,\
		MessageSize,\
		offset BytesWritten,\
		NULL
invoke ReadConsole,\ ;读取字符
	    StdinHandle,\
		addr szBuffer,\
		sizeof szBuffer,\
		addr BytesRead,\
		NULL
mov eax,BytesRead
sub eax,2
mov num,ax
;invoke crt_printf,addr Message2,num;打印读入表达式的长度
;invoke crt_printf,addr Message2,szBuffer[0]
;invoke crt_printf,addr Message2,szBuffer[1]
;invoke crt_printf,addr Message2,szBuffer[2]
;invoke crt_printf,addr Message2,szBuffer[3]
;invoke crt_printf,addr Message2,szBuffer[4]
;invoke crt_printf,addr Message2,szBuffer[5]
mov eax,0;用于压栈，ah为符号位，al为数字位
mov ebx,0
mov ecx,0;保存当前栈中还有多少元素
mov edx,0
DO:
.WHILE bx<num;对输入的表达式字符一个个处理
	mov dl,szBuffer[bx]
	inc bx
	;先对符号进行处理
	.IF dl=='+';符号+
		mov ah,1
		mov al,'+'
		jmp STACK
	.ELSEIF dl=='-'
		mov ah,2
		mov al,'-'
		jmp STACK
	.ELSEIF dl=='('
		mov ah,3
		mov al,'('
		jmp STACK
	.ELSEIF dl==')'
		mov ah,4
		mov al,')'
		jmp STACK
	.ENDIF;能进入到下面说明一定读取了个数字
	mul ten;ax=al*10
	sub dl,'0'
	add al,dl;讲数字转化为十进制
	.IF bx<num;下一个字符是运算符，数字读取完毕,若bx=num，说明的h是最后一个数字，直接进入STACK
		mov dl,szBuffer[bx]
		.IF dl=='+' ||dl=='-'||dl=='('||dl==')'
			jmp STACK
		.ENDIF
			jmp DO
	.ENDIF

STACK:	
	;mov j,al
	;invoke crt_printf,addr Message2,j
	.IF cx==0;开始时，栈中没有元素
		push ax
		inc cx
	.ELSEIF cx>0
		pop dx;得到栈顶元素
		.IF dh==0;栈顶是数,则输入只可能为+,-,)
			.IF ah==1;+
				push dx
				push ax
				inc cx
			.ELSEIF ah==2;-
				push dx
				push ax
				inc cx
			.ELSEIF ah==4;),这里要一直到出(
				pop ax
				.WHILE ah!=3;则数前只可能为加减,一直出栈，直到发现（
					.IF ah==1;+
						pop ax
						add al,dl
						push ax
					.ELSEIF ah==2;-
						pop ax
						sub al,dl
						push ax
					.ENDIF
						sub cx,2
						pop dx
						pop ax
				.ENDW
				push dx
				dec cx
			 .ENDIF
		.ELSEIF dh==1;栈顶是+
			.IF ah==0;num
				pop dx
				add al,dl
				push ax
				dec cx
			.ELSEIF ah==3;(
				push dx
				push ax
				inc cx
			.ENDIF
		.ELSEIF dh==2;栈顶是-
			.IF ah==0;num
				pop dx
				sub dl,al
				push dx
				dec cx
			.ELSEIF ah==3;(
				push dx
				push ax
				inc cx
			.ENDIF
		.ELSEIF dh==3;栈顶是(
			.IF ah==0;num
				push dx
				push ax
				inc cx
			.ELSEIF ah==3;(
				push dx
				push ax
				inc cx
			.ELSEIF dh==4;)
				dec cx
			.ENDIF
		.ENDIF
	.ENDIF 
	mov eax,0
	mov edx,0	
.ENDW
;现在栈内没有括号了，但是还有部分未处理的表达式
.WHILE cx>1
	pop ax
	dec cx
	jmp STACK
.ENDW
pop ax
mov j,al
;mov i,cx
;invoke crt_printf,addr Message2,i
invoke crt_printf,addr Message2,j
end start