%{
   /* definition */
   #include <stdio.h>
   #include <ctype.h>
   #include <stdlib.h>
   #include <string.h>
   #include "type.h"
   #define YYSTYPE struct sym
   #include "minipy-lab.h"
   
   
%}
%token ID INT REAL STRING_LITERAL IN DEL
%left '-' '+'
%left '*' '/' '%'

%%
Start: prompt Lines
      ;
Lines: error '\n' prompt  {/*yyerror("");*//* yyerrork();*/}
      |Lines  stat '\n' prompt
      | Lines  '\n' prompt
      | 
     // | error '\n' prompt  {/*yyerror("");*//* yyerrork();*/}
      ;
prompt: {  printf("miniPy>");   /*cout << "miniPy> ";*/}
       ;
stat:
        assignExpr{ if($1.type!=NONTYPE){
                        sym_print($1);
                        printf("\n");
                        }
                        //printf("\n");
                }       
      ;
assignExpr:
        atom_expr '=' assignExpr{   $$=assign($1,$3);
                                    //   if($1.type==V){对变量赋值，先分配空间，再赋值
                                      //          $1.id->value=(struct sym *)malloc(sizeof(struct sym));
                                     //           *($1.id->value)=$3;
                                        //        $$=$1;
                                      //  }else if($1.type==IND){//对列表的索引赋值
                                          //      *($1.indexing)=$3;
                                            //    $$=$1;
                                        //} 
                          }
      | add_expr 
      | del        
      ;
del: DEL atom_expr '[' add_expr ']'{$$=del($2,$4);}         
number: INT
       | REAL
       ;
factor: '+' factor{ $$=positive($2);    }//正数
       | '-' factor { $$=minus($2);//负数
                }
       | atom_expr
       
       ; 
atom: ID
      | STRING_LITERAL 
      | List 
      | number 
      
      ;
slice_op:/*  empty production */{/*yyerror("test yyerror");printf("#NULL-slicingop#");*/$$.type=NONTYPE;}//空产生式
        | ':' /*  empty production(新增，能识别形如a[::]或a[1:2:]型切片操作 */{/*yyerror("test yyerror");*/$$.type=NONTYPE;}
        | ':' add_expr {$$=$2;}
        ;
sub_expr:  /*  empty production */{/*yyerror("test yyerror");printf("#NULL-subexpr#");*/$$.type=NONTYPE;}
        | add_expr
        ;        
atom_expr: atom {
                if($1.type==FUNC0||$1.type==FUNC1||$1.type==FUNC2||$1.type==FUNC3){//若是函数，则会生成函数的链表
                        $$.type=ARGLIST;$$.arglist=NULL;
                        $$.arglist=addnode_arglist($$.arglist,$1);
                      //  printf("test*atom->func");
                }
                }
        | atom_expr  '[' sub_expr  ':' sub_expr  slice_op ']'{//切片操作，可以对字符串或列表进行切片操作 
                                              //  printf("#ok#");
                                               // if(slicing_type_check($1,$3,$5,$6)==1){
                                                      // printf("#ok#");
                                                       $$=slicing($1,$3,$5,$6);
                                                      //  }
                                                  }
        | atom_expr  '[' add_expr ']'{ //索引,$1可能是一个字符串或列表常量，也有可能是一个变量
                                $$=indexing($1,$3);//字符串返回的是偏移量对应的字符，列表返回的是偏移量对应的列表元素地址
                        }
        | atom_expr  '.' ID     { //$1为变量或常量，$2为函数名
                                      //  if($3.type==FUNC1||$3.type==FUNC2||$3.type==FUNC3){
                                                $$.type=ARGLIST;$$.arglist=NULL;
                                                $$.arglist=addnode_arglist($$.arglist,$3);
                                                $$.arglist=addnode_arglist($$.arglist,$1);
                                                //这样我们的函数总是在链表头了
                                       // }
                                              }
        | atom_expr  '(' arglist opt_comma ')'{$$=functions($1,$3);
                                        //若单为函数无变量，$1链表中只有一个函数，有变量则有两个节点
                                        //调用函数，$1为中有函数名和变量名，$3中为参数链
                                        }
        | atom_expr  '('  ')'{//参数为空的情况
                                struct sym arg;
                                arg.type=ARGLIST;arg.arglist=NULL;//参数链为空
                                $$=functions($1,arg);
                        }
        
        ;
arglist: add_expr {//识别第一个参数，初始化参数链表
                $$.type=ARGLIST;$$.arglist=NULL;
                $$.arglist=addnode_arglist($$.arglist,$1);
                     }
        | arglist ',' add_expr{//添加后续参数
                $1.arglist=addnode_arglist($1.arglist,$3);
                $$=$1;
        } 
        ;
        ;      
List: '[' ']'{  $$.type=L;$$.list=NULL;//空列表    
            }
      | '[' List_items opt_comma ']' {$$=$2;/*将列表传递过来*/}
      ;
opt_comma: /*  empty production */
          | ','
          ;
List_items: add_expr{//列表中的第一个元素会匹配该文法，给链表生成第一个节点的空间
                     $$.type=L;$$.list=NULL;//printf("*OK*");
                     $$.list=addnode_list($$.list,$1);//初始化链表，将链表元素加到链表中
                    // printf("--%d--",$$.list);        
                   }
            | List_items ',' add_expr {//列表的其他元素依次匹配该文法，向链表依次增加节点
                                       // printf("#ok#");
                                        $1.list=addnode_list($1.list,$3);//向列表中加入元素
                                        $$=$1;
                                       // printf("#ok#");
                                       // printf("--%d--",$$.list); 
                                 }
      ;
add_expr: 
           add_expr '+' mul_expr{$$=cal($1,$3,ADD);}
	|  add_expr '-' mul_expr{$$=cal($1,$3,SUB);}
        |  mul_expr
        |  add_expr IN mul_expr {$$=membership_check($1,$3);}  
        ;
mul_expr: 
           mul_expr '*' factor {$$=cal($1,$3,MUL);}
        |  mul_expr '/' factor{$$=cal($1,$3,DIV);}
	|  mul_expr '%' factor{$$=cal($1,$3,MOD);}
        |  factor
        ;

%%

int main()
{      
   init_symtab();
   return yyparse();
}

void yyerror(const char *s)
{
  // cout << s << endl<<"miniPy> ";
  //if(strlen(s)>0)
        printf("%s\n",s);
 // printf("miniPy>"); 
}

int yywrap()
{ return 1; }        		    
