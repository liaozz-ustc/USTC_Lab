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
scanf PROTO C:DWORD,:vararg
printf PROTO C:DWORD,:vararg
.data
InFmt DB '%lf',0
OutFmt db '%g is %d ',0
x qword ?
y dd ?
x2 db '1'
OutFmt2 db '%c is ',0
.code
start:
;invoke scanf,addr InFmt,addr x
;FLD x
;FISTP y
;invoke printf,addr OutFmt,x,y
invoke printf,addr OutFmt2,x2
end start