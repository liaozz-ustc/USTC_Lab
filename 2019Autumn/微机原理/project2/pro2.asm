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
array dd 100 dup(0) ;10*10二维数组

endL EQU <0dh,0ah>

arraydata db '%d ',0

Message db endL,'Please input:',0 
MessageSize = ($-Message)
szBuffer db 10 dup(0)
ConsoleHandle DWORD 0
StdinHandle DWORD 0
BytesWritten DWORD ?
BytesRead DWORD ? 
i dw 0
j dw 0
N db 0
arraydataend db '%d ',endL
.code
start:
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov ConsoleHandle,eax ;输出句柄

invoke GetStdHandle,STD_INPUT_HANDLE
mov StdinHandle,eax ;输入句柄

invoke WriteConsole,\ ;输出提示
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

mov ecx,0
mov cl,szBuffer[0]
sub cl,'0' ;cl=N
mov N,cl


mov bx,0 ;基址
mov si,0 ;变址
mov eax,0 ;i=0
mov edx,1
.while(i<cx);
	mov j,0
	mov si,0
	.while(j<cx)
		mov eax,0
		mov ax,bx
        add ax,si
        mov DWORD ptr array[eax],edx
		inc edx
		inc j
		add si,4
	.endw
	inc i
	add bx,40
.endw


mov bx,0 ;基址
mov si,0 ;变址
mov eax,0 ;i=0
mov i,0
mov j,0	

mov ecx,0
mov cl,szBuffer[0]
sub cl,'0' ;cl=N
mov bx,0

.while(i<cx)
	mov j,0
	mov si,0
	.while(j<cx)
		mov eax,0
		mov ax,bx
        add ax,si
		mov dx,i
		.if j<dx
        invoke crt_printf,addr arraydata,array[eax]
		.elseif j==dx
		invoke crt_printf,addr arraydataend,array[eax]
		.endif
		inc j
		add si,4
		mov ecx,0
		mov cl,N
	.endw
    
	inc i
	add bx,40
	
.endw



END start