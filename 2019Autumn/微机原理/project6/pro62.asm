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
printf PROTO C:DWORD,:vararg
.data
endL EQU <0dh,0ah>
Message db "Input:",0 
MessageSize = ($-Message)
Message2 db "x=%d",endL,0
Message2Size = ($-Message2)
Error db "Error: x<0!",0
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
OutFmt2 db "%c",0
;DD和DWORD都是32位，DB8位，DW16位
num DD 0 ;保存输入的数
x DD 0
a1 DD 0
a2 DD 0
a3 DD 0
i DD 0
j DD 0
TEN DD 10
result DD 0
test_result qword 0
data db 40 dup('@');保存字符串
whole dd ?
fract dd ?
temp dw ? ;放cw
ten dw 10
.code
start:
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov ConsoleHandle,eax ;输出句柄

invoke GetStdHandle,STD_INPUT_HANDLE
mov StdinHandle,eax ;输入句柄

.WHILE i<4
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

    mov eax,0;eax用于保存转换的十进制数
    mov ebx,0;数组偏移地址
    mov ecx,0
    mov edx,0
    .WHILE ecx<BytesRead
        mul TEN;eax=eax*10***********特别注意乘法指令对edx的修改
        mov dl,szBuffer[ecx];******edx会被乘法指令修改******
        .IF i==0 && dl=='-'
            invoke WriteConsole,\ ;输出提示
	        ConsoleHandle,\
	        offset Error,\
		    ErrorSize,\
		    offset BytesWritten,\
		    NULL
            jmp errorprint
        .ENDIF
        .IF dl=='-'
            mov ebx,1
        .ELSE
            sub dl,'0';ASCII '0'=48
            add eax,edx;eax=eax*10+fileBuffer[i]-'0'
        .ENDIF
        inc ecx
    .ENDW
    ;下面对转换的十进制数进行保存
    .IF ebx==1
        mov ebx,eax
        mov eax,0
        sub eax,ebx
    .ENDIF
    .IF i==0
        mov x,eax
        invoke crt_printf,addr Message2,x
    .ELSEIF i==1
        mov a1,eax
        invoke crt_printf,addr Message3,a1
    .ELSEIF i==2
        mov a2,eax
        invoke crt_printf,addr Message4,a2
    .ELSEIF i==3
        mov a3,eax
        invoke crt_printf,addr Message5,a3
    .ENDIF
    inc i
.ENDW

FILD x;将整数x加载到栈顶st(0)
FSQRT ;st(0)=sqrt(st(0))
FIMUL a1;st(0)=st(0)*a1(实数乘整数)
FSTP result;暂存结果a1*(x)^1/2

FILD a2;将整数a2加载到栈，变为st(1)
FILD x;将整数x加载到栈顶st(0)
FYL2X ;st(1)=st(1)*log2(st(0)),即a2*log2(x),然后st(0)出栈
FADD result ;即a1*(x)^1/2+a2*log2(x)，结果存st(0)
FSTP result ;暂存result

FILD x;st(0)=x
FSIN ;st(0)=sin(st(0))=sin(x)
FIMUL a3 ;st(0)=st(0)*a3(整数乘)
FADD result ;st(0)=st(0)+result
FSTP result;结果最后在result

FLD result
FSTP test_result

invoke printf,addr OutFmt,test_result;调用函数测试

;下面将单精度浮点数转换成字符串
mov ecx,0
FSTCW temp
mov ax,temp ;将四舍五入改为截断
push ax
or ax,0C00H;将RC舍入类型修改为截断,都是保留整数
mov temp,ax
FLDCW temp
FLD result;实数入栈,书上少了这句话
FTST;和0比较
FSTSW ax
and ax,4500H  ;have c3,c2,c0
.IF ax==100H ;如果为负
    mov data[ecx],'-'
    inc ecx
    FABS
.ENDIF

FRNDINT ;舍入为整数,栈顶为整数
FIST whole ;保存整数
FLD result ;实数减整数部分即为小数
FABS
FSUBR ;st(0)=st(0)-st(1)
FSTP fract ;保存小数部分
;下面转化整数部分
mov eax,whole
mov ebx,10
push ebx;作为整数是否出栈完毕的标志
.REPEAT
    mov edx,0
    div ebx;edx保存除的余数，整数部分一直除10，余数入栈，直到为0
    add dl,'0' ;转化为ASCII码
    push edx
.UNTIL eax==0
;栈中保存了整数部分从高位到低位的ASCII码

pop edx
;mov ah,3;-计数
.WHILE edx!=10 ;将整数部分的ASCII依次保存在数组中
   ; invoke printf,addr OutFmt2,data[0]
    pop ebx
  ;  dec ah
 ;   .IF ah==0 && ebx !=10;每3个数字加一个-
  ;      mov data[ecx],'-'
  ;      inc ecx
  ;      mov ah,3
  ;  .ENDIF
    mov data[ecx],dl
    inc ecx
    mov edx,ebx
.ENDW
;invoke printf,addr OutFmt2,data[0]
;invoke printf,addr OutFmt2,data[1]
;invoke printf,addr OutFmt2,data[2]

;整数部分处理完毕

mov data[ecx],'.'
inc ecx
pop temp ;重新存储原始的cw
FLDCW temp
FLD fract ;转换小数部分
.REPEAT
    FIMUL ten;//小数乘10
    FIST temp;保存乘10后的整数，即小数高位
    mov ax,temp
    add al,'0'
    mov data[ecx],al;存入数组
    inc ecx
    FISUB temp;小数乘10后减去整数部分，处理剩余小数
    FXAM;检测栈顶是否为0
    SAHF;传给标志寄存器
.UNTIL ZERO?
FCOMP
mov data[ecx],0
mov i,ecx
mov ebx,0
.WHILE i>0
    invoke crt_printf,addr OutFmt2,data[ebx]
    inc ebx
    dec i
.ENDW
errorprint:

end start
