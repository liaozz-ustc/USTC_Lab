%{
#include <stdlib.h>
#include <stdio.h>
#include "type.h"
#define YYSTYPE struct sym
extern struct sym yylval;
#include "y.tab.h"
/*  any C declaration  */
%}

letter    [a-zA-Z]

digit     [0-9]

number   {digit}+

id       {letter}({letter}|{digit})*

real     {number}"."{number}?|{number}"."{number}

%%
[ \t]+      {/*do nothing , just skip */}

in          { 
              return IN;
                      }
del         { return DEL;}
{number}    { yylval.type=I;
              yylval.i=atoi(yytext);
           //   printf("\n***(%d,%d)",yylval.type,yylval.i);
              return INT;
            }
{real}      { yylval.type=D;
              yylval.d=atof(yytext);              
            //   printf("\n***(%d,%f)",yylval.type,yylval.d);
              return REAL;
            }
{id}        { yylval=func_look(yytext);//检查是不是函数名
              if(yylval.type==V){//不是函数名
                  yylval.id=symlook(yytext);//查找其在符号表中的地址，若没找到则向符号表中添加，返回添加的地址
                  // printf("\n***(%d,%s)",yylval.type,yylval.id->name);
                }/*else{
                     printf("lex-isfunctionname");
                 }*/
              return ID; 
            }
\"(\\.|[^\\"])*\"	{ yylval.type=S;
              yylval.s=strdup(yytext);//为字符串分配段空间，然后将指针保存下来
              yylval.s[yyleng-1]='\0';
              yylval.s++;
              //不能之间yylval.s=yytext,这是因为yytext是分配好的一段静态空间
              //而每次扫描字符的时候其内容会改变
              //printf("\n***(%d,%s)",yylval.type,yylval.s);
             return(STRING_LITERAL); 
                  }
.|\n        { //printf("\n***(X,%c)",yytext[0]);
               return yytext[0];}
%%
