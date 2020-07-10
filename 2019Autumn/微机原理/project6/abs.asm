.386
.model flat,stdcall
option casemap:none
include \masm32\include\windows.inc
;include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
;include \masm32\include\masm32.inc
;includelib \masm32\lib\masm32.lib
;includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
include \masm32\include\msvcrt.inc
includelib \masm32\lib\msvcrt.lib
printf PROTO C:DWORD,:vararg
.data
endL EQU <0dh,0ah>
Message db "Input:",0 
MessageSize = ($-Message)
Message2 db "x=%d",endL,0
Message2Size = ($-Message2)
Error db "Error: x<=0!",0
ErrorSize = ($-Error)
ConsoleHandle DWORD 0
StdinHandle DWORD 0
BytesWritten DWORD ?
BytesRead DWORD ? 
szBuffer db 1024 dup(0)
Message3 db "a1=%d",endL,0
Message4 db "a2=%d",endL,0
Message5 db "a3=%d",endL,0
OutFmt db "result=%g",endL,0
OutFmt2 db "%d",0
;DD和DWORD都是32位，DB8位，DW16�?
;num DD 0 ;保存输入的数
x DD 0
a1 DD 0
a2 DD 0
a3 DD 0
i DD 0
j DD 0
TEN DD 10
;result DD 0
test_result qword 0
data db 40 dup(0);保存字符�?
whole dd ?
fract dd ?
temp dw ? ;放cw
ten dw 10
test1 db 'csbc$'
num dd 10
result db 0
array db '123211'
number dd 6
b dd 0
.code
start:
mov ecx,0
mov b,ecx
fild b
mov result,1
mov esi,0
dec number
mov edi,number
cmpl1:
cmp esi,edi
jg over
mov al,array[esi]
mov ah,array[edi]
inc esi
dec edi
cmp ah,al
jz cmpl1
mov result,0
over: 
invoke crt_printf,addr OutFmt2,result
end start
