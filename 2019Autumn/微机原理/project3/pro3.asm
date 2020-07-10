;ml /c /coff pro3.asm
;Link /subsystem:console pro3.obj
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
array dd 100 dup(0)
endL EQU <0dh,0ah>
Message db endL,"After sort:",0 
MessageSize = ($-Message)
Message2 db "Before sort:",0
Message2Size = ($-Message2)
ConsoleHandle DWORD 0
StdinHandle DWORD 0

BytesWritten DWORD ?
BytesRead DWORD ? 

filename db 'Input3.txt',0
fileHandle DWORD ?
fileBuffer db 500 dup(0)
fileBufferSize = ($-fileBuffer)

arraydata db '%d ',0
arraydata2 db '-%d- ',0
arraydata3 db '-%x- ',0
i db 0
j db 0
num db 0 ;用于保存输入了多少数�?
ten DWORD 10
.code
sort:
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov ConsoleHandle,eax ;输出句柄

invoke GetStdHandle,STD_INPUT_HANDLE
mov StdinHandle,eax ;输入句柄

invoke CreateFile,\ ;打开文件
		addr filename,\
		GENERIC_READ,\
		FILE_SHARE_READ,\
		NULL,\
		OPEN_EXISTING,\
		FILE_ATTRIBUTE_NORMAL,\
		0
mov fileHandle,eax

invoke ReadFile,\ ;读文�?
		fileHandle,\
		addr fileBuffer,\
		fileBufferSize,\
		addr BytesRead,\
		0

invoke WriteConsole,\ ;输出提示
		ConsoleHandle,\
		offset Message2,\
		Message2Size,\
		offset BytesWritten,\
		NULL           
;下面将读入的字符串转换为十进制数
;invoke crt_printf,addr arraydata,BytesRead
mov eax,0;eax用于保存转换的十进制�?
mov ebx,0;数组偏移地址
mov edx,0
mov ecx,0
mov edi,0;edi用于判断读入的字符串是否为负�?
DO:
.WHILE ecx<BytesRead
    mov dl,fileBuffer[ecx]
    .IF dl=='-';负数,edi=1
        mov edi,1
        inc ecx
        jmp DO
    .ENDIF
    .if dl==32;space
        inc num;数字
        .IF edi==1;处理负数
            mov edi,eax
            mov eax,0
            sub eax,edi;得到-eax            
        .ENDIF
        mov array[ebx],eax;保存
        ;invoke crt_printf,addr arraydata2,eax
        mov eax,0;eax重新初始�?
        add ebx,4;偏移�?*****偏移是加4�?
        mov edi,0;edi重新赋为0
        inc ecx
        jmp DO;是空格，保存转换的十进制数，结束本次循环
    .endif
    mul ten;eax=eax*10***********特别注意乘法指令对edx的修�?
    mov dl,fileBuffer[ecx];******edx会被乘法指令修改******
    sub dl,'0';ASCII '0'=48
    add eax,edx;eax=eax*10+fileBuffer[i]-'0'
    inc ecx
.ENDW

;invoke crt_printf,addr arraydata2,ecx
;输出未排序的数字
;mov eax,array[0]
;invoke crt_printf,addr arraydata2,array[0]
;invoke crt_printf,addr arraydata2,array[4]
;invoke crt_printf,addr arraydata2,array[8]
;invoke crt_printf,addr arraydata2,array[12]
;invoke crt_printf,addr arraydata2,array[16]
;invoke crt_printf,addr arraydata2,array[20]
;mov eax,array[4]
;invoke crt_printf,addr arraydata,eax
mov cl,0
mov ebx,0
.WHILE cl<num
    push ebx
    push ecx
    invoke crt_printf,addr arraydata,array[ebx]
    pop ecx
    pop ebx
    add ebx,4
    inc cl
.ENDW
;下面对数组进行排�?
mov eax,0
mov ebx,0
mov ecx,0
mov edx,0
.WHILE cl<num
    mov edx,array[ebx]
    mov eax,ebx;edx保存当前最小值，eax保存当前最小值的位置
    mov ch,cl;ch相当j
    inc ch;j=i+1
    mov esi,ebx
    add esi,4;esi为array[j]对应的偏移地址
    .WHILE ch<num
       ;.IF array[esi]<edx
       ;     mov edx,array[esi]
       ;     mov eax,esi
       ; .ENDIF
        cmp array[esi],edx
        jge GOGE 
            mov edx,array[esi]
            mov eax,esi
        GOGE:;array[esi]>=edx
        add esi,4
        inc ch
    .ENDW
    mov edi,array[ebx];array[i]<->array[eax]
    mov array[ebx],edx
    mov array[eax],edi
    add ebx,4
    inc cl
.ENDW

invoke WriteConsole,\ ;输出提示
		ConsoleHandle,\
		offset Message,\
		MessageSize,\
		offset BytesWritten,\
		NULL 

mov cl,0
mov ebx,0
.WHILE cl<num
    push ebx
    push ecx
    invoke crt_printf,addr arraydata,array[ebx]
    pop ecx
    pop ebx
    add ebx,4
    inc cl
.ENDW
imul eax,0
invoke crt_printf,addr arraydata2,eax

mov ax,5678H
mov bx,0ADBCH
sub al,bl
;invoke crt_printf,addr arraydata3,ax
;mov ax,FLAGS
pushf
pop ax
invoke crt_printf,addr arraydata3,ax


end sort


