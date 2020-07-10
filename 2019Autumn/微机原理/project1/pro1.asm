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
.data
endL EQU <0dh,0ah>
Message db endL,"Please input:",0 
MessageSize = ($-Message)
szBuffer db 80 dup(0)

ConsoleHandle DWORD 0
StdinHandle DWORD 0

BytesWritten DWORD ?
BytesRead DWORD ? 

filename db 'Input1.txt',0
fileHandle DWORD ?
fileBuffer db 80 dup(0)
fileBufferSize = ($-fileBuffer)
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

invoke ReadConsole,\ ;读取字符串
		StdinHandle,\
		addr szBuffer,\
		sizeof szBuffer,\
		addr BytesRead,\
		NULL
invoke CreateFile,\ ;创建文件
		addr filename,\
		GENERIC_WRITE,\
		FILE_SHARE_WRITE,\
		NULL,\
		CREATE_ALWAYS,\
		FILE_ATTRIBUTE_NORMAL,\
		0
mov fileHandle,eax
invoke WriteFile,\ ;写文件
		fileHandle,\
		addr szBuffer,\
		BytesRead,\
		addr BytesWritten,\
		0
invoke CloseHandle,fileHandle
invoke CreateFile,\ ;打开文件
		addr filename,\
		GENERIC_READ,\
		FILE_SHARE_READ,\
		NULL,\
		OPEN_EXISTING,\
		FILE_ATTRIBUTE_NORMAL,\
		0
mov fileHandle,eax

invoke ReadFile,\ ;读文件
		fileHandle,\
		addr fileBuffer,\
		fileBufferSize,\
		addr BytesRead,\
		0
invoke CloseHandle,fileHandle
		
mov ebx,BytesRead
mov ecx,0 ;i
mov edi,0
.while(ecx<ebx) ;小写转大写
	mov dl,fileBuffer[edi]
	.if(dl>'Z') 
		sub dl,32
		mov fileBuffer[edi],dl
	.endif
	inc ecx
	inc edi
.endw

invoke WriteConsole,\ ;输出结果
		ConsoleHandle,\
		offset fileBuffer,\
		BytesRead,\
		offset BytesWritten,\
		NULL
		
invoke CreateFile,\ ;打开文件
		addr filename,\
		GENERIC_WRITE,\
		FILE_SHARE_WRITE,\
		NULL,\
		OPEN_EXISTING,\
		FILE_ATTRIBUTE_NORMAL,\
		0
mov fileHandle,eax
invoke SetFilePointer,\ ;移动文件指针到文件尾
		fileHandle,\
		0,\
		0,\
		FILE_END
invoke WriteFile,\ ;将结果写入文件
		fileHandle,\
		addr fileBuffer,\
		BytesRead,\
		addr BytesWritten,\
		0
invoke CloseHandle,fileHandle		
;invoke CloseConsoleHandle,OutputHandle
;invoke ExitProcess,0

END start