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
Message db "Input a number:",0 
MessageSize = ($-Message)
Message2 db "%d!=",0
Message2Size = ($-Message2)
ConsoleHandle DWORD 0
StdinHandle DWORD 0
BytesWritten DWORD ?
BytesRead DWORD ? 
szBuffer db 10 dup(0)

result db 100 dup(0) ;用来保存计算的结果，每个数组元素代表位数,第0位表示当前数的位数，1位表示个位，类推
tmp db 100 dup(0)
num DWORD 0 ;保存输入的数
i DWORD 0
j DWORD 0
ten DWORD 10
arraydata db '%d',0
arraydata2 db '-%d-',0
.code
factorial proc, n:DWORD
;输入n，在result中得到n的阶乘
pushad;保存所有32位通用寄存器
.IF n==0   
    mov result[0],1
    mov result[1],1
.ELSE
dec n;n减1
invoke factorial,n
;在result中已得到(n-1)!,计算n!=n*(n-1)!即为n个result相加
;使用tmp数组暂存结果
;invoke crt_printf,addr arraydata2,n
mov ebx,0
mov al,result[0];得到位数
mov ah,0
.WHILE ah<=al
    mov bl,ah
    mov dl,result[bx]
    mov tmp[bx],dl
    inc ah
.ENDW
;计算阶乘,只要将n-1个result加到result上即可
mov ecx,1
mov ah,1
.WHILE ecx<=n;此时n已经为n-1
    mov ah,1
    mov al,result[0];在两个数相加中，位数最多加1，更新cl
    .WHILE ah<=al
        mov bl,ah
        mov dl,tmp[bx]
        add dl,result[bx]
        .IF dl>=10;大于10,进位
            sub dl,10
            .IF ah==result[0];如果在最后一位进位了，位数加1
                inc result[0]
            .ENDIF
            inc bl
            inc result[bx];进一位，高位加1
            dec bl
        .ENDIF
        mov result[bx],dl
        inc ah 
    .ENDW
    inc ecx
.ENDW

.ENDIF
popad;恢复所有32位通用寄存器
ret
factorial endp

main proc 
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
;下面将读入的字符串转换为十进制数
mov eax,BytesRead
sub eax,2;从控制台输入的字符串比看得到的多两个
mov BytesRead,eax
;invoke crt_printf,addr Message2,szBuffer[0]
;invoke crt_printf,addr Message2,szBuffer[1]

mov eax,0;eax用于保存转换的十进制数
mov ebx,0;数组偏移地址
mov ecx,0
mov edx,0
.WHILE ecx<BytesRead
    mul ten;eax=eax*10***********特别注意乘法指令对edx的修改
    mov dl,szBuffer[ecx];******edx会被乘法指令修改******
    sub dl,'0';ASCII '0'=48
    add eax,edx;eax=eax*10+fileBuffer[i]-'0'
    inc ecx
.ENDW
mov num,eax
invoke crt_printf,addr Message2,num
invoke factorial,num;调用函数，得到num的阶乘，存储在result数组中

mov ecx,0
mov ebx,0
mov edx,0
mov bl,result[0]
mov cl,result[0]
.WHILE ecx>=1    
    push ecx
    push ebx
    mov edx,0
    mov dl,result[ebx]
    invoke crt_printf,addr arraydata,edx
    ;***注意：在使用基址加变址寻址时不能使用8位寄存器
    ;为什么调用这个函数经常出错，这是因为调用函数会有返回值，返回值会存储在eax中，故使用eax经常会出错
    ;这是因为经过这个函数调用后改变了eax的值，以后要注意
    pop ebx
    pop ecx
    dec ebx
    dec ecx
.ENDW

main endp
END main