//int symnum=0;//变量数目
/*在符号表中搜寻变量，找到则返回其地址，未找到则返回NULL*/
struct symtab *symlook(char *s) {
    struct symtab *sp;
    for (sp = symtab; sp < &symtab[NSYMS]; sp++) {//在符号表中查找
        if (sp->name && !strcmp(sp->name, s))//找到则返回
            return sp;
        if (!sp->name||(sp->name&&!sp->value)) {//查询到一个空的符号表位置或者该符号表位置虽然有名字，但并没有被赋值
            //也就是说，在lex中虽然一个没被赋值的变量可能被加入到符号表，但是之后很快会被覆盖
            sp->name = strdup(s);//该函数返回新分配的一段空间的指针，然后将s中的内容复制到该空间
            return sp;
        }
    }
    yyerror("Too many symbols!");
    exit(1);
    //eturn NULL;//符号表满，但是没找到
    /* if(symnum==0)//符号表空，返回NULL
         return NULL;
     for(int i=0;i<symnum;i++){//在符号表中查找
         if(strcmp(symtab[i].name,s)==0){//找到
             return  &symtab[i];
         }
     }
     return NULL;*/
}
/*struct symtab *installsym(char *s,struct sym *v){//向符号表中加入一个变量,返回在符号表中的指针
    if(symnum==NSYMS){//符号表满
        yyerror("Too many symbols!");
        exit(1);
    }
    symtab[symnum].name=strdup(s);//该函数返回新分配的一段空间的指针，然后将s中的内容复制到该空间
    symtab[symnum].id=v;
    symnum++;
    return &symtab[symnum-1];
}*/
void list_print(struct l_node *l) {//对列表进行输出
    printf("[");
    if (l == NULL) {
        printf("]");
        return;
    }
    struct l_node *p = l;
    do {
        sym_print(*(p->value));
        if (p->next != l)//最后一个元素
            printf(",");
        p = p->next;
    } while (p != l);
    printf("]");

}

void id_print(struct symtab *i) {
    printf("%s=", i->name);
    sym_print(*(i->value));

}

struct sym sym_print(struct sym a) {//打印输出
    struct sym result;
    result.type=NONTYPE;
    if (a.type == I)
        printf("%d", a.i);
    else if (a.type == D)
        printf("%g", a.d);
    else if (a.type == S) {
        if (a.s)
            printf("'%s'", a.s);
        else
            printf("''");
    } else if (a.type == L)
        list_print(a.list);
    else if (a.type == V) {
           if(a.id->value==NULL)
               printf("NameError: name '%s' is not defined!",a.id->name);
            else
                id_print(a.id);
    }
    else if (a.type == IND)
        sym_print(*(a.indexing));
    else if(a.type==BOOL){
        if(a.i)
            printf("True");
        else 
            printf("Flase");
    }
    return result;
}
void init_symtab() {
    struct symtab *sp;
    for (sp = symtab; sp < &symtab[NSYMS]; sp++) {//将符号表中的指针初始化为NULL，防止出现野指针
        sp->value = NULL;
        sp->name = NULL;
    }
}

struct l_node *addnode_list(struct l_node *l, struct sym e) {//向链表末尾添加一个节点
    if(e.type==V&&e.id->value){
        e=*(e.id->value);
    }else if(e.type==V&&!e.id->value){
        char *s = get_idname(e);
        printf("NameError: name '%s' is not defined\n", s);
        return l;
    }
    if (l == NULL) {//若l是空链表
        l = (struct l_node *) malloc(sizeof(struct l_node));
        l->value = (struct sym *) malloc(sizeof(struct sym));
        *(l->value) = e;//对节点赋值
        l->next = l;
        l->prior = l;
        return l;
    } else {//链表不空
        struct l_node *tail = l->prior;
        tail->next = (struct l_node *) malloc(sizeof(struct l_node));
        /*之前竟然忘记了对该节点赋值，导致报错,然后竟然忘记了对该节点的值申请空间，又报错*/
        tail->next->value = (struct sym *) malloc(sizeof(struct sym));
        *(tail->next->value) = e;//对节点赋值
        tail->next->prior = tail;
        tail->next->next = l;
        l->prior = tail->next;
        return l;
    }
}


struct l_node *append_list(struct l_node *l, struct l_node *s) {//将双向链表s加到l的末尾
    if (s == NULL)
        return l;
    struct l_node *p = s;
    do {
        l = addnode_list(l, *(p->value));
        p = p->next;
    } while (p != s);
    return l;
}

struct sym cal_add(struct sym op1, struct sym op2) {//对两个sym变量相加
    struct sym result;
    if (op1.type == I) {
        if (op2.type == I) {
            result.type = I;
            result.i = op1.i + op2.i;
            return result;
        } else if (op2.type == D) {
            result.type = D;
            result.d = (double) op1.i + op2.d;
            return result;
        }
    } else if (op1.type == D) {
        result.type = D;
        if (op2.type == I) {
            result.d = op1.d + (double) op2.i;
            return result;
        } else {
            result.d = op1.d + op2.d;
            return result;
        }
    } else if (op1.type == S && op2.type == S) {//字符串的相加
        result.s = (char *) malloc((strlen(op1.s) + strlen(op2.s) + 1) * sizeof(char));
        strcpy(result.s, op1.s);
        strcat(result.s, op2.s);
        result.type = S;
        return result;
    } else if ((op1.type == L ||op1.type==SLI)&& (op2.type == L||op2.type==SLI)) {//列表的相加
        result.type = L;
        result.list = NULL;//构造一个空的列表
        // if(op1.list==NULL&&op2.list==NULL)//两个空列表相加
        //    return result;
        result.list = append_list(result.list, op1.list);
        result.list = append_list(result.list, op2.list);
        return result;
    }

}


struct sym cal_sub(struct sym op1, struct sym op2) {//减计算,只有两个数字才能进行相减
    struct sym result;
    if (op1.type == I) {
        if (op2.type == I) {
            result.type = I;
            result.i = op1.i - op2.i;
            return result;
        } else if (op2.type == D) {
            result.type = D;
            result.d = (double) op1.i - op2.d;
            return result;
        }
    } else if (op1.type == D) {
        result.type = D;
        if (op2.type == I) {
            result.d = op1.d - (double) op2.i;
            return result;
        } else {
            result.d = op1.d - op2.d;
            return result;
        }
    }
}

struct sym str_mul(struct sym op1, struct sym op2) {//字符串和数字的相乘，op1为字符串类型，op2为整型类型
    struct sym result;
    result.type = S;
    if (op2.i <= 0 || op1.s == NULL) {//乘小于0的数或字符串为空
        result.s = NULL;
        return result;
    } else {
        result.s = (char *) malloc((op2.i * strlen(op1.s) + 1) * sizeof(char));//加1是为了存放'\0'
        int i;
        for (i = 0; i < op2.i; i++) {
            strcat(result.s, op1.s);
        }
        return result;
    }
}

struct sym list_mul(struct sym op1, struct sym op2) {//列表和数字的相乘，op1为列表类型，op2为整形类型
    struct sym result;
    result.type = L;
    if (op2.i <= 0 || op1.list == NULL) {//乘小于0的数或列表为空
        result.list = NULL;
        return result;
    } else {
        int i;
        result.list = NULL;
        for (i = 0; i < op2.i; i++) {
            result.list = append_list(result.list, op1.list);
        }
        return result;
    }
}

struct sym cal_mul(struct sym op1, struct sym op2) {
    struct sym result;
    if (op1.type == I) {
        if (op2.type == I) {
            result.type = I;
            result.i = op1.i * op2.i;
            return result;
        } else if (op2.type == D) {
            result.type = D;
            result.d = (double) op1.i * op2.d;
            return result;
        } else if (op2.type == S) {
            return str_mul(op2, op1);
        } else if (op2.type == L || op2.type==SLI) {
            return list_mul(op2, op1);
        }
    } else if (op1.type == D) {
        result.type = D;
        if (op2.type == I) {
            result.d = op1.d * (double) op2.i;
            return result;
        } else if (op2.type == D) {
            result.d = op1.d * op2.d;
            return result;
        }
    } else if (op1.type == S && op2.type == I) {//字符串的相乘
        return str_mul(op1, op2);
    } else if ((op1.type == L || op1.type==SLI) && op2.type == I) {//列表的相乘
        return list_mul(op1, op2);
    }
}

struct sym cal_div(struct sym op1, struct sym op2) {//只有两个数才进行相除，结果总是real型，进行了错误检查才进入除函数，保证除数不为0
    struct sym result;
    result.type = D;
    if (op1.type == I) {
        if (op2.type == I) {
            result.d = (double) op1.i / (double) op2.i;
            return result;
        } else if (op2.type == D) {
            result.d = (double) op1.i / op2.d;
            return result;
        }
    } else if (op1.type == D) {
        if (op2.type == I) {
            result.d = op1.d / (double) op2.i;
            return result;
        } else if (op2.type == D) {
            result.d = op1.d / op2.d;
            return result;
        }
    }

}

struct sym cal_mod(struct sym op1, struct sym op2) {//只有两个数字之间才可以求模
    struct sym result;
    if (op1.type == I && op2.type == I) {
        result.type = I;
        result.i = op1.i % op2.i;
        return result;
    } else {
        result.type = D;
        if (op1.type == I && op2.type == D) {
            int k = (int) ((double) op1.i / op2.d);
            result.d = (double) op1.i - k * op2.d;
            return result;
        } else if (op1.type == D && op2.type == I) {
            int k = (int) (op1.d / (double) op2.i);
            result.d = op1.d - (double) k * op2.i;
            return result;
        } else if (op1.type == D && op2.type == D) {
            int k = (int) (op1.d / op2.d);
            result.d = op1.d - k * op2.d;
            return result;
        }
    }
}
struct sym cal(struct sym op1, struct sym op2, enum cal_type op) {
    if(type_check(op1,op2,op)==0){
       
        struct sym error;error.type=-1;
        return error;
    }
  
    if (op1.type == V)//如果操作数中有变量，将变量转换为它们的值域
        op1 = *(op1.id->value);
    if (op2.type == V)
        op2 = *(op2.id->value);
    if(op1.type==IND)
        op1=*(op1.indexing);
    if(op2.type==IND)
        op2=*(op2.indexing);
    if (op == ADD)
        return cal_add(op1, op2);
    else if (op == SUB)
        return cal_sub(op1, op2);
    else if (op == MUL)
        return cal_mul(op1, op2);
    else if (op == DIV)
        return cal_div(op1, op2);
    else if (op == MOD)
        return cal_mod(op1, op2);
}

struct sym str_indexing(struct sym op1, struct sym op2) {//字符串索引不能复制，故不返回地址
    struct sym result;
    result.type = S;
    result.s = NULL;
    if (op1.s == NULL) {
        yyerror("IndexError: string index out of range");
        result.type=NONTYPE;
    } else {
        int len = (int) strlen(op1.s);
        if (op2.i > (len - 1) || op2.i < (-len)) {//正索引偏移编号从0开始，负索引偏移编号
            yyerror("IndexError: string index out of range");
            result.type=NONTYPE;
        } else {
            result.s = (char *) malloc(2 * sizeof(char));
            result.s[1] = '\0';
            if (op2.i >= 0) {
                result.s[0] = op1.s[op2.i];
            } else {
                result.s[0] = op1.s[len + op2.i];
            }
        }
    }
    return result;
}

int list_len(struct l_node *l) {
    if(l==NULL)
        return 0;
    else{
        struct l_node *p=l;
        int len=0;
        do{
            len++;
            p=p->next;
        }while(p!=l);
        return len;
    }

}

struct sym list_indexing(struct sym op1, struct sym op2) {//对列表索引，返回该列表元素的域地址，因为这样方便进行列表索引的赋值
    struct sym result;
    result.type = IND;
    result.indexing = NULL;
    struct l_node *p = op1.list;
    if (op1.list == NULL) {
        yyerror("IndexError: list index out of range");
        result.type=NONTYPE;
    } else {
        int len = list_len(op1.list);//对列表求长度
        if (op2.i > (len - 1) || op2.i < (-len)) {
            yyerror("IndexError: list index out of range");
            result.type=NONTYPE;
        } else {
            int i;
            if (op2.i < 0)//如果是负索引，将它转化为正索引，负索引对应的正索引=len+负索引
                // 这里op2是传进来的参变量，不会改变函数外部的值
                op2.i = op2.i + len;
            for (i = 0; i < op2.i; i++)//求相应索引在列表中的位置
                p = p->next;
            result.indexing = p->value;
        }
    }
    return result;
}



struct sym indexing(struct sym op1, struct sym op2) {//索引只能对列表和字符串进行，索引里的编号类型只能是整型,进入该函数前进行类型检查
    struct sym result;
    result.type = NONTYPE;
    if (op1.type == V &&op1.id->value)//如果操作数中有变量，将变量转换为它们的值域,只能是列表或字符串变量
        op1 = *(op1.id->value);
    else if(op1.type==V&&!op1.id->value){
        char *s = get_idname(op1);
        printf("NameError: name '%s' is not defined\n", s);
        return result;
    }
    if(check_int(op2)==0){
        return result;
    }
        if (op2.type == V && op2.id->value)//如果操作数中有变量，将变量转换为它们的值域,只能是列表或字符串变量
        op2 = *(op2.id->value);
    else if (op2.type == V && !op2.id->value) {
        char *s = get_idname(op2);
        printf("NameError: name '%s' is not defined\n", s);
        return result;
    }
    if (op1.type == IND)//如果索引的变量是一个列表或字符串，那么也能再进行索引
        op1 = *(op1.indexing);
    if (op1.type == S)
        return str_indexing(op1, op2);
    else if (op1.type == L || op1.type == SLI)
        return list_indexing(op1, op2);
    else {
        yyerror("TypeError: Need list or string");
        return result;
    }
}

struct sym positive(struct sym op1){//正数处理
    if(op1.type==I ||op1.type==D)
        return op1;
    else if(op1.type==V)
        return positive(*(op1.id->value));
}
struct sym minus(struct sym op1){//负数处理
    struct sym result;
    if(op1.type==I){
        result.type=I;
        result.i=-op1.i;
        return result;
    }else if(op1.type==D){
        result.type=D;
        result.d=-op1.d;
        return result;
    }else if(op1.type==V){
        return minus(*(op1.id->value));
    }
}

//(列表或字符串，整型或空，整型或空，整型或空（不为0））
int slicing_type_check(struct sym data, struct sym op1, struct sym op2, struct sym op3) {//对切片类型检查，错误返回0，正确返回1
    if ((data.type == L || (data.type == V && data.id->value && data.id->value->type == L)
         || data.type == S || (data.type == V && data.id->value && data.id->value->type == S)
         || (data.type == IND && (data.indexing->type == L || data.indexing->type == S)))
        && (op1.type == I || op1.type == NONTYPE || (op1.type == V && op1.id->value && (op1.id->value->type == I)))
        && (op2.type == I || op2.type == NONTYPE || (op2.type == V && op2.id->value && (op2.id->value->type == I)))
        && ((op3.type == I && op3.i != 0) || op3.type == NONTYPE
            || (op3.type == V && op3.id->value && (op3.id->value->type == I) && (op3.id->value->i != 0)))
            )
        return 1;
    else {//步长不能为0
        if (data.type == V && !data.id->value) {
            char *s = get_idname(data);
            printf("NameError: name '%s' is not defined\n", s);
        } else if (op1.type == V && !op1.id->value) {
            char *s = get_idname(op1);
            printf("NameError: name '%s' is not defined\n", s);
        } else if (op2.type == V && !op2.id->value) {
            char *s = get_idname(op2);
            printf("NameError: name '%s' is not defined\n", s);
        }else if (op3.type == V && !op3.id->value) {
            char *s = get_idname(op3);
            printf("NameError: name '%s' is not defined\n", s);
        } else if (((op3.type == I && op3.i == 0)
                    || (op3.type == V && data.id->value && (op3.id->value->type == I) && (op3.id->value->i == 0))))
            yyerror("ValueError: slice step cannot be zero!");
        else
            yyerror("slicing Typeerror!");
        return 0;
    }
}
int slicing_prepa(int len, struct sym op1, struct sym op2, int op3, int *index1, int *index2) {
    //切片前的准备工作,对索引进行转换
    if (op3 > 0) {
        if (op1.type == NONTYPE)
            *index1 = 0;
        else if (op1.i > len - 1)
            *index1 = len;
        else if (op1.i >= 0)
            *index1 = op1.i;
        else if (op1.i >= -len)
            *index1 = op1.i + len;
        else
            *index1 = 0;
        if (op2.type == NONTYPE)
            *index2 = len;
        else if (op2.i > len)
            *index2 = len;
        else if (op2.i >= 0)
            *index2 = op2.i;
        else if (op2.i >= -len)
            *index2 = op2.i + len;
        else
            *index2 = -1;
    } else if (op3 < 0) {
        if (op1.type == NONTYPE)
            *index1 = len-1;
        else if (op1.i > len - 1)
            *index1 = len-1;
        else if (op1.i >= 0)
            *index1 = op1.i;
        else if (op1.i >= -len)
            *index1 = op1.i + len;
        else
            *index1 = -1;
        if (op2.type == NONTYPE)
            *index2 = -1;
        else if (op2.i > len)
            *index2 = len;
        else if (op2.i >= 0)
            *index2 = op2.i;
        else if (op2.i >= -len-1)
            *index2 = op2.i + len;
        else
            *index2 = -1;
    }
    return 1;
}
struct sym str_slicing(struct sym data, struct sym op1, struct sym op2, int op3) {//第1、2索引偏移量（可能为空），步长(不为0）
    //切片包含第一索引指向的元素，不包含第而索引指向的元素
    int index1, index2,i,counter=0;
    struct sym result;
    result.type=S;result.s=NULL;
    if(data.s==NULL)
        return result;
    int len=(int)strlen(data.s);
    slicing_prepa(len,op1,op2,op3,&index1,&index2);
    if ((op3 > 0 && index1 >= index2) || (op3 < 0 && index1 <= index2))//返回空的情况
        return result;//虽然说index1索引的包含在切片，但是因为-1和len分别是最小最大编号，故不会被取中
    result.s = (char *) malloc((abs(index2 - index1) + 1) * sizeof(char));
   // printf("(index1=%d,index2=%d)\n", index1, index2);
    if (op3 > 0) {//则index1<index2
        for (i = index1; i <= index2 - 1 && i < len; i = i + op3) {
            result.s[counter++] = data.s[i];
        }
        result.s[counter] = '\0';
    } else if (op3 < 0) {//则index1>index2
        for (i = index1; i >= index2 + 1 && i >= 0; i = i + op3) {
            result.s[counter++] = data.s[i];
        }
        result.s[counter] = '\0';
    }
    return result;
    
}

struct sym slicing(struct sym data, struct sym op1, struct sym op2, struct sym op3) {//切片操作
    if(slicing_type_check(data,op1,op2,op3)==0){
        struct sym result;result.type=NONTYPE;
        return result;
    }
    if (data.type == V)//将变量都转换为值
        data = *(data.id->value);
    else if(data.type==IND)//将索引转化为值
        data =*(data.indexing);
    if (op1.type == V)
        op1 = *(op1.id->value);
    if (op2.type == V)
        op2 = *(op2.id->value);
    if (op3.type == V)
        op3 = *(op3.id->value);
    if (data.type == S) {
        if (op3.type == NONTYPE)//如果步长为空，则设步长为1
            return str_slicing(data, op1, op2, 1);//op1,op2为空再进入函数里判断
        else
            return str_slicing(data, op1, op2, op3.i);
    } else if (data.type == L) {
        if (op3.type == NONTYPE)
            return list_slicing(data, op1, op2, 1);
        else
            return list_slicing(data, op1, op2, op3.i);
    }


}
struct sym list_slicing(struct sym data, struct sym op1, struct sym op2, int op3) {//第1、2索引偏移量（可能为空），步长(不为0）
    //对列表的切片，返回一个列表，列表中的元素域地址和切取的元素域地址相同
    int index1, index2,i,counter=0;
    struct sym result;
    result.type=L;result.list=NULL;
    if(data.list==NULL)
        return result;
    int len=list_len(data.list);
    slicing_prepa(len,op1,op2,op3,&index1,&index2);
    if ((op3 > 0 && index1 >= index2) || (op3 < 0 && index1 <= index2))//返回空的情况
        return result;//虽然说index1索引的包含在切片，但是因为-1和len分别是最小最大编号，故不会被取中
    //result.list=(struct l_node *)malloc(sizeof(struct l_node));
    //printf("(index1=%d,index2=%d)\n", index1, index2);
    struct sym tmp;
    if (op3 > 0) {//则index1<index2,且index1!=-1
        for (i = index1; i <= index2 - 1 && i < len; i = i + op3) {
            op1.i=i;
            tmp=list_indexing(data,op1);//从链表中索引，得到指向的元素的域地址
            result.list=addnode_list_getaddress(result.list,tmp.indexing);//将该域地址加到输出列表
        }
    } else if (op3 < 0) {//则index1>index2
        for (i = index1; i >= index2 + 1 && i >= 0; i = i + op3) {
            op1.i=i;
            tmp=list_indexing(data,op1);//从链表中索引，得到指向的元素的域地址
            result.list=addnode_list_getaddress(result.list,tmp.indexing);//将该域地址加到输出列表
        }
    }
    return result;
}
struct l_node *addnode_list_getaddress(struct l_node *l, struct sym *e) {//向链表末尾添加一个节点,输入的是地址
    if (l == NULL) {//若l是空链表
        l = (struct l_node *) malloc(sizeof(struct l_node));
        l->value = e;
        l->next = l;
        l->prior = l;
        return l;
    } else {//链表不空
        struct l_node *tail = l->prior;
        tail->next = (struct l_node *) malloc(sizeof(struct l_node));
        tail->next->value = e;
        tail->next->prior = tail;
        tail->next->next = l;
        l->prior = tail->next;
        return l;
    }
}

struct l_node *list_assign(struct l_node *dest,struct l_node *source){//对列表的赋值操作，会改变原列表的值
    //两个列表长度相等
    if(dest==NULL)
        return dest;
    struct l_node *p=dest;
    do{
        *(p->value)=*(source->value);
        p=p->next;
        source=source->next;
    }while(p!=dest);
    return dest;

}
struct sym assign(struct sym dest, struct sym source) {
    if (dest.type == V) {//对变量赋值，先分配空间，再赋值
        dest.id->value = (struct sym *) malloc(sizeof(struct sym));
        if(source.type==V){
            if(source.id->value)
                *(dest.id->value)=*(source.id->value);
            else if(!source.id->value){
                char *s = get_idname(source);
                printf("NameError: name '%s' is not defined\n", s);
                struct sym error;
                error.type=NONTYPE;
                return error;
            }
        }
        else
            *(dest.id->value) = source;
    } else if (dest.type == IND) {//对列表的索引赋值
        *(dest.indexing) = source;
    } else if (dest.type == L && (source.type == L || (source.type == V && (source.id->value->type == L)) ||
                                  source.type == SLI)) {//对列表切片的赋值的
        if (source.type == V)//将变量转化为值
            source = *(source.id->value);
        if (list_len(dest.list) != list_len(source.list))
            yyerror("ValueError: attempt to assign sequence with different slice lengths!");
        else {
            list_assign(dest.list, source.list);
        }
    } else {
        yyerror("SyntaxError: can't assign to literal");
    }
    dest.type = NONTYPE;
    return dest;

}

struct arg_node *addnode_arglist(struct arg_node *l,struct sym arg){
    if(l==NULL){
        l=(struct arg_node *)malloc(sizeof(struct arg_node));
        l->arg=(struct sym *)malloc(sizeof(struct sym));
        *(l->arg)=arg;
        l->next=NULL;
        return l;
    }else{//链表不空
        struct arg_node *p=l;
        struct arg_node *tail=(struct arg_node *)malloc(sizeof(struct arg_node));
        tail->arg=(struct sym *)malloc(sizeof(struct sym));
        *(tail->arg)=arg;
        while(p->next!=NULL){
            p=p->next;
        }
        p->next=tail;
        tail->next=NULL;
        return  l;
    }
}
int arglist_len(struct arg_node *l) {
    int len = 0;
    if (l == NULL);
    else {
        while (l != NULL) {
            l = l->next;
            len++;
        }
    }
    return len;
}
struct sym functions(struct sym func, struct sym arg) {
    //func为函数链，第一元素为函数指针，第二元素（若存在为对象），
    //arg为参数链，从前往后为参数1、2、3
    struct sym error;int i;
    error.type = NONTYPE;
    struct arg_node *p=arg.arglist;
    for( i=0;i<arglist_len(arg.arglist);i++){
        if(p->arg->type==V&&!p->arg->id->value){
            char *s = get_idname(*p->arg);
            printf("NameError: name '%s' is not defined\n", s);
            return error;
        }
        p = arg.arglist->next;
    }
    if (func.arglist->next == NULL) {//无对象的情况
        if (func.arglist->arg->type == FUNC0) {
            return (func.arglist->arg->func0)();
        } else if (func.arglist->arg->type == FUNC1) {//函数参数为1
            if (arglist_len(arg.arglist) != 1) {
                yyerror("TypeError: only takes one argument");
                return error;
            }
            //struct sym op1=*(arg.arglist->arg);//得到参数
            if (func.arglist->arg->func1 != sym_print)
                return (func.arglist->arg->func1)(*(arg.arglist->arg));
            else {
                (func.arglist->arg->func1)(*(arg.arglist->arg));
                printf("\n");
                return error;
            }
        } else if (func.arglist->arg->type == FUNC2) {//函数参数为2
            if (arglist_len(arg.arglist) != 2) {
                yyerror("TypeError: only takes two arguments");
                return error;
            }
            return (func.arglist->arg->func2)(*(arg.arglist->arg), *(arg.arglist->next->arg));
        } else if (func.arglist->arg->type == FUNC3) {//函数参数为3
                 if(func.arglist->arg->func3==range){
                if(arglist_len(arg.arglist)==1){
                    struct sym arg2;arg2.type=NONTYPE;
                    struct sym arg3;arg3.type=NONTYPE;
                    return (func.arglist->arg->func3)(*(arg.arglist->arg), arg2,
                                                      arg3);
                }else if(arglist_len(arg.arglist)==2){
                    struct sym arg3;arg3.type=NONTYPE;
                    return (func.arglist->arg->func3)(*(arg.arglist->arg), *(arg.arglist->next->arg),
                                                      arg3);
                }
            }
            
            if (arglist_len(arg.arglist) != 3) {
                yyerror("TypeError: only takes three arguments");
                return error;
            }
            return (func.arglist->arg->func3)(*(arg.arglist->arg), *(arg.arglist->next->arg),
                                              *(arg.arglist->next->next->arg));
        } else {
            yyerror("NameError: not a function");
            return error;
        }
    } else {//有对象的情况
        if(func.arglist->next->arg->type==V&&!func.arglist->next->arg->id->value){
            char *s = get_idname(*func.arglist->next->arg);
            printf("NameError: name '%s' is not defined\n", s);
            return error;
        }
        struct sym data = *(func.arglist->next->arg);//获得对象
        if (func.arglist->arg->type == FUNC1) {//函数参数为1
            if (arglist_len(arg.arglist) != 0) {
                yyerror("TypeError: only takes no argument");
                return error;
            }
            //struct sym op1=*(arg.arglist->arg);//得到参数
            return (func.arglist->arg->func1)(data);
        } else if (func.arglist->arg->type == FUNC2) {//函数参数为2
            if (arglist_len(arg.arglist) != 1) {
                yyerror("TypeError: only takes one arguments");
                return error;
            }
            return (func.arglist->arg->func2)(data, *(arg.arglist->arg));
        } else if (func.arglist->arg->type == FUNC3) {//函数参数为3
            if (arglist_len(arg.arglist) != 2) {
                yyerror("TypeError: only takes two arguments");
                return error;
            }
            return (func.arglist->arg->func3)(data, *(arg.arglist->arg), *(arg.arglist->next->arg));
        } else {
            yyerror("NameError: not a function");
            return error;
        }

    }
}

int check_list(struct sym object){
    if(object.type==L
    ||(object.type==V&&object.id->value->type==L)
    ||(object.type==IND&&object.indexing->type==L)
    ||(object.type==SLI))
        return 1;
    else
        return 0;
}
struct l_node *get_listhead(struct sym object){
    if(object.type==L)
        return object.list;
    else if(object.type==V&&object.id->value->type==L)
        return object.id->value->list;
    else if(object.type==IND&&object.indexing->type==L)
        return object.indexing->list;
    else if(object.type==SLI)
        return object.list;
    else
        return NULL;
}
struct sym append(struct sym object, struct sym arg){
    if(check_list(object)==1){//切片的列表是一个新建的，只是域指针与原理相同，故对它加一个元素对原列表是不影响的
        addnode_list(get_listhead(object),arg);
        object.type=NONTYPE;
        return object;
    }
    yyerror("AttributeError: Need list!");
    struct sym error;error.type=NONTYPE;
    return error;
}
int check_str(struct sym object){
    if(object.type==S
       ||(object.type==V&&object.id->value->type==S)
       ||(object.type==IND&&object.indexing->type==S)
       )
        return 1;
    else
        return 0;
}
char *get_str(struct sym object){
    if(object.type==S)
        return object.s;
    else if(object.type==V&&object.id->value->type==S)
        return object.id->value->s;
    else if(object.type==IND&&object.indexing->type==S)
        return object.indexing->s;
    else
        return NULL;
}
struct sym len(struct sym object){
    struct sym result;
    result.type=NONTYPE;
    if(check_list(object)==1){
        result.i=list_len(get_listhead(object));
        result.type=I;
        return result;
    } else if(check_str(object)==1){
        result.i=(int)strlen(get_str(object));
        result.type=I;
        return result;
    } else{
        yyerror("TypeError: type of object should be 'list' or 'str'! ");
        result.type=NONTYPE;
        return result;
    }
}
struct sym quit(){
    exit(1);
}

struct sym extend(struct sym object,struct sym arg){
    struct sym result;
    result.type=NONTYPE;
    if(check_list(object)==1&&check_list(arg)==1){
        struct l_node *list1=get_listhead(object);
        struct l_node *list2=get_listhead(arg);
        append_list(list1,list2);
        return result;
    }else{
        yyerror("TypeError: type of object should be 'list'");
        return result;
    }
}
/*struct l_node *list_reverse(struct l_node *l){
    if(l==NULL)
        return l;
    struct l_node *p=l;
    do{
        struct l_node *tmp;
        tmp=p->prior;
        p->prior=p->next;
        p->next=tmp;
        p=p->prior;
    }while(p!=l);
    return  p->next;
}*/
struct sym reverse(struct sym object){
    struct sym result;
    result.type=NONTYPE;
    if(check_list(object)==1){
        struct l_node *list1=get_listhead(object);
        struct sym data;data.type=L;data.list=list1;
        struct sym op1;op1.type=NONTYPE;
        struct sym tmp=list_slicing(data,op1,op1,-1);

 //     list1=list_reverse(list1);
        if(object.type==L)
            object.list=tmp.list;
        else if(object.type==V&&object.id->value->type==L)
            object.id->value->list=tmp.list;
        else if(object.type==IND&&object.indexing->type==L)
            object.indexing->list=tmp.list;
        else
            ;
        return result;
    }else{
        yyerror("TypeError: type of object should be 'list'");
        return result;
    }
}

struct sym list_sort1(struct sym op1)
{
    struct sym op2=op1;
    struct l_node *list1;
    list1=get_listhead(op1);
    if(list1==NULL)
        return op2;
    int len=list_len(list1);

    int i,j;
    struct l_node *ptr1,*ptr2;
    struct sym *tmp1;

    ptr1=list1;int flag=ptr1->value->type;
    for(i=2;i<=len;i++)
    {
        ptr1=ptr1->next;
        if(ptr1->value->type!=flag){
            yyerror("TypeError:Need the same type!");
        }

        if(list1->value->type!=ptr1->value->type)
            return op2;
    }
    if(list1->value->type==3)
        return op2;
    if(list1->value->type==0)
        for(i=1;i<=len-1;i++)
        {
            ptr1=ptr1->next;
            ptr2=ptr1;
            for(j=i+1;j<=len;j++)
            {
                ptr2=ptr2->next;
                if(ptr1->value->i>ptr2->value->i)
                {
                    tmp1=ptr1->value;
                    ptr1->value=ptr2->value;
                    ptr2->value=tmp1;
                }
            }
        }
    else if(list1->value->type==1)
        for(i=1;i<=len-1;i++)
        {
            ptr1=ptr1->next;
            ptr2=ptr1;
            for(j=i+1;j<=len;j++)
            {
                ptr2=ptr2->next;
                if(ptr1->value->d>ptr2->value->d)
                {
                    tmp1=ptr1->value;
                    ptr1->value=ptr2->value;
                    ptr2->value=tmp1;
                }
            }
        }
    else
        for(i=1;i<=len-1;i++)
        {
            ptr1=ptr1->next;
            ptr2=ptr1;
            for(j=i+1;j<=len;j++)
            {
                ptr2=ptr2->next;
                if(strcmp(ptr1->value->s,ptr2->value->s)>0)
                {
                    tmp1=ptr1->value;
                    ptr1->value=ptr2->value;
                    ptr2->value=tmp1;
                }
            }
        }
    return op2;
}

struct sym list_sort(struct sym source)
{
    struct sym result;
    if(check_list(source)==0)
    {
        yyerror("TypeError: type of object should be 'list'! ");
        result.type=-1;
        return result;
    }
    list_sort1(source);
    result.type=-1;
    return result;
}


struct sym string_to_list1(struct sym source)
{
    int i;
    int l;
    char *str,*tmp;
    struct sym new_list;
    struct l_node *node1,*node2;
    str=get_str(source);
    l=strlen(str);
    new_list.type=3;
    if(l==0)
    {
        new_list.list=NULL;
        return new_list;
    }

    new_list.list=(struct l_node *)malloc(sizeof(struct l_node));
    node2=new_list.list;
    for(i=2;i<=l;i++)
    {
        node2->next=(struct l_node *)malloc(sizeof(struct l_node));
        node1=node2;
        node2=node2->next;
        node2->prior=node1;
    }
    node2->next=new_list.list;
    new_list.list->prior=node2;

    node1=new_list.list;
    for(i=1;i<=l;i++)
    {
        node1->value=(struct sym *)malloc(sizeof(struct sym));
        node1->value->type=2;
        node1->value->s=(char*)malloc(2*sizeof(char));
        tmp=node1->value->s;
        tmp[0]=str[i-1];
        tmp[1]='\0';
        node1=node1->next;
    }

    return new_list;
}

struct sym string_to_list(struct sym object)
{
    struct sym result;
    if(check_str(object)==0)
    {
        result.type=-1;
        yyerror("TypeError: type of object should be 'str'! ");
        return result;
    }
    result=string_to_list1(object);
    return result;
}

char *get_idname(struct sym object){//输入的object是变量类型的
    struct symtab *sp;
    for (sp = symtab; sp < &symtab[NSYMS]; sp++) {//在符号表中查找
        if(sp==object.id){
            return sp->name;
        }
    }
    return NULL;
}
int check_int(struct sym object){
    if(object.type==I
    ||(object.type==V&&object.id->value&&object.id->value->type==I)
    ||(object.type==IND&&object.indexing->type==I))
        return 1;
    else{
        if(object.type==V&&!object.id->value){
            char *s = get_idname(object);
            printf("NameError: name '%s' is not defined\n", s);
        }else {
            yyerror("TypeError: must be integers");
        }
        return 0;
    }
}
int check_id(struct sym object){
   if(object.type==V&&object.id->value)
       return 1;
   else if(object.type==V&&!object.id->value){
       char *s = get_idname(object);
       printf("NameError: name '%s' is not defined\n", s);
   }
    return 0;
}

struct sym str_find(struct sym object, struct sym arg) {
    struct sym result;
    result.type = NONTYPE;
    if (check_str(object) == 0) {
        yyerror("Typeerror:Need string!");
        return result;
    }
    if (check_str(arg) == 0) {
        yyerror("Typeerror:Need string!");
        return result;
    }
    result.type=I;
    char *s1=get_str(object);
    char *s2=get_str(arg);
    char *s;
    s=strstr(s1,s2);
    if(s==NULL)
        result.i=-1;
    else
        result.i=(int)(s-s1);
    return result;
}

struct sym str_lower(struct sym object){
    struct sym result;
    result.type = NONTYPE;
    if (check_str(object) == 0) {
        yyerror("Typeerror:Need string!");
        return result;
    }
    result.type=S;
    result.s=strdup(get_str(object));
    int i;
    for(i=0;i<strlen(result.s);i++){
        if('A'<=*(result.s+i)&&*(result.s+i)<='Z')
            *(result.s+i)=*(result.s+i)+32;
    }
    return result;
}

struct sym *get_sym_pointer(struct sym *object)
{
    struct sym *ptr;
    if(object->type==0||object->type==1||object->type==2||object->type==3||object->type==6)
        ptr=object;
    else if(object->type==4)
        ptr=object->id->value;
    else if(object->type==5)
        ptr=object->indexing;
    else
        ptr=NULL;
    return ptr;
}

int type_check1(int type1,int type2,enum cal_type op)
{
    int judge_table[4][4];
    if(op==ADD)
    {
        int make_judge_table[16]={1,1,0,0,1,1,0,0,0,0,1,0,0,0,0,1};
        memcpy(judge_table,make_judge_table,16*sizeof(int));
    }
    else if(op==SUB)
    {
        int make_judge_table[16]={1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
        memcpy(judge_table,make_judge_table,16*sizeof(int));
    }
    else if(op==MUL)
    {
        int make_judge_table[16]={1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,0};
        memcpy(judge_table,make_judge_table,16*sizeof(int));
    }
    else if(op==DIV)
    {
        int make_judge_table[16]={1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
        memcpy(judge_table,make_judge_table,16*sizeof(int));
    }
    else
    {
        int make_judge_table[16]={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//real可以参与求模吗？
        memcpy(judge_table,make_judge_table,16*sizeof(int));
    }
    return judge_table[type1][type2];
}

int type_check(struct sym op1,struct sym op2,enum cal_type op)
{
    struct sym *ptr1,*ptr2;
    int type1,type2;
    int flag;
    if(op1.type==4)
        if(op1.id->value==NULL)
        {
            printf("NameError: name '%s' is not defined\n",op1.id->name);
            return 0;
        }
    if(op2.type==4)
        if(op2.id->value==NULL)
        {
            printf("NameError: name '%s' is not defined\n",op2.id->name);
            return 0;
        }
    ptr1=get_sym_pointer(&op1);
    ptr2=get_sym_pointer(&op2);
    type1=ptr1->type;
    type2=ptr2->type;
    flag=type_check1(type1,type2,op);
    if(flag==1)
        return 1;

    char type_name[4][7]={"int","real","string","list"};
    char option_name[6]={"+-*/%"};
    printf("TypeError: unsupported operand type(s) for %c: '%s' and '%s'\n",option_name[op],type_name[type1],type_name[type2]);
    return 0;
}
struct sym range(struct sym object, struct sym arg1, struct sym arg2)
{   
    int j;
    struct sym result;
    result.type=L;
    result.list=NULL;
        struct sym x;
        if(object.type == V)
            x = *object.id->value;
        else
            x = object;
    if(x.type!=I)
    {
        yyerror("TypeError: the object cannot be interpreted as an integer");
        result.type = NONTYPE;
        return result;
    }
    if(arg1.type==NONTYPE)
    {
        for(j=0;j<x.i;j++)
        {
            struct sym e;
            e.type=I;
            e.i=j;
            result.list=addnode_list(result.list,e);
        }
    }
    else
    {
        struct sym y;
        if(arg1.type == V)
            y = *arg1.id->value;
        else
            y = arg1;
        if(y.type!=I)
        {
            yyerror("TypeError: the object cannot be interpreted as an integer");
            result.type = NONTYPE;
            return result;
        }
        struct sym z;
        if(arg2.type == V)
            z = *arg2.id->value;
        else
            z = arg2;
        if((z.type!=I)&&(z.type!=NONTYPE))
        {
            yyerror("TypeError: the object cannot be interpreted as an integer");
            result.type = NONTYPE;
            return result;
        }
        int k;
        if(arg2.type == NONTYPE)
            k=1;
        else
            k=z.i;
        for(j=x.i;j<y.i;j+=k)
        {
            struct sym e;
            e.type=I;
            e.i=j;
            result.list=addnode_list(result.list,e);
        }
    }
    return result;
}

struct sym max(struct sym object)
{
    struct sym result;
    result.type = NONTYPE;
        struct sym v1;
        if(object.type == V)
            v1 = *object.id->value;
        else
            v1 = object;
        if(v1.type == I)
            yyerror("TypeError: 'int' object is not iterable");
        else if(v1.type == D)
            yyerror("TypeError: 'float' object is not iterable");
        else if(v1.type == S)
        {
            char c=v1.s[0];
            int j;
            for(j=1;j<strlen(v1.s);j++)
                if(v1.s[j]>c)
                    c = v1.s[j];
            result.type = S;
            result.s=(char*)malloc(2*sizeof(char));
            result.s[0] = c;
            result.s[1] = 0;
        }
        else if(v1.type == L)
        {   int j;
            struct l_node *l = v1.list;
            result = *l->value;
            for( j=1;j<list_len(v1.list);j++)
            {
                l=l->next;
                if(l->value->type == I)
                {
                    if(result.type == I)
                    {
                        if(l->value->i>result.i)
                            result = *l->value;
                    }
                    else if(result.type == D)
                    {
                        if(l->value->i>result.d)
                            result = *l->value;
                    }
                    else if(result.type == S)
                    {
                        yyerror("TypeError: '>' not supported between instances of 'int' and 'str'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == L)
                    {
                        yyerror("TypeError: '>' not supported between instances of 'int' and 'list'");
                        result.type = NONTYPE;
                        return result;
                    }
                }
                else if(l->value->type == D)
                {
                    if(result.type == I)
                    {
                        if(l->value->d>result.i)
                            result = *l->value;
                    }
                    else if(result.type == D)
                    {
                        if(l->value->d>result.d)
                            result = *l->value;
                    }
                    else if(result.type == S)
                    {
                        yyerror("TypeError: '>' not supported between instances of 'float' and 'str'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == L)
                    {
                        yyerror("TypeError: '>' not supported between instances of 'float' and 'list'");
                        result.type = NONTYPE;
                        return result;
                    }
                }
                else if(l->value->type == S)
                {
                    if(result.type == I)
                    {
                        yyerror("TypeError: '>' not supported between instances of 'str' and 'int'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == D)
                    {
                        yyerror("TypeError: '>' not supported between instances of 'str' and 'float'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == S)
                    {
                        if(strcmp(l->value->s,result.s)>0)
                            result = *l->value;
                    }
                    else if(result.type == L)
                    {
                        yyerror("TypeError: '>' not supported between instances of 'str' and 'list'");
                        result.type = NONTYPE;
                        return result;
                    }
                }
            }
        }
        return result;
}

struct sym min(struct sym object)
{
    struct sym result;
    result.type = NONTYPE;
        struct sym v1;
        if(object.type == V)
            v1 = *object.id->value;
        else
            v1 = object;
        if(v1.type == I)
            yyerror("TypeError: 'int' object is not iterable");
        else if(v1.type == D)
            yyerror("TypeError: 'float' object is not iterable");
        else if(v1.type == S)
        {
            char c=v1.s[0];
            int j;
            for(j=1;j<strlen(v1.s);j++)
                if(v1.s[j]<c)
                    c = v1.s[j];
            result.type = S;
            result.s=(char*)malloc(2*sizeof(char));
            result.s[0] = c;
            result.s[1] = 0;
        }
        else if(v1.type == L)
        {   int j;
            struct l_node *l = v1.list;
            result = *l->value;
            for( j=1;j<list_len(v1.list);j++)
            {
                l=l->next;
                if(l->value->type == I)
                {
                    if(result.type == I)
                    {
                        if(l->value->i<result.i)
                            result = *l->value;
                    }
                    else if(result.type == D)
                    {
                        if(l->value->i<result.d)
                            result = *l->value;
                    }
                    else if(result.type == S)
                    {
                        yyerror("TypeError: '<' not supported between instances of 'int' and 'str'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == L)
                    {
                        yyerror("TypeError: '<' not supported between instances of 'int' and 'list'");
                        result.type = NONTYPE;
                        return result;
                    }
                }
                else if(l->value->type == D)
                {
                    if(result.type == I)
                    {
                        if(l->value->d<result.i)
                            result = *l->value;
                    }
                    else if(result.type == D)
                    {
                        if(l->value->d<result.d)
                            result = *l->value;
                    }
                    else if(result.type == S)
                    {
                        yyerror("TypeError: '<' not supported between instances of 'float' and 'str'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == L)
                    {
                        yyerror("TypeError: '<' not supported between instances of 'float' and 'list'");
                        result.type = NONTYPE;
                        return result;
                    }
                }
                else if(l->value->type == S)
                {
                    if(result.type == I)
                    {
                        yyerror("TypeError: '<' not supported between instances of 'str' and 'int'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == D)
                    {
                        yyerror("TypeError: '<' not supported between instances of 'str' and 'float'");
                        result.type = NONTYPE;
                        return result;
                    }
                    else if(result.type == S)
                    {
                        if(strcmp(l->value->s,result.s)<0)
                            result = *l->value;
                    }
                    else if(result.type == L)
                    {
                        yyerror("TypeError: '<' not supported between instances of 'str' and 'list'");
                        result.type = NONTYPE;
                        return result;
                    }
                }
            }
        }
        return result;
}

struct sym sym_pow(struct sym object, struct sym arg)
{
    double y1,y2;
    struct sym result,x1,x2;
    result.type = NONTYPE;
    int iferr=0;
    if(object.type == V)
        x1 = *object.id->value;
    else
        x1 = object;
    if(arg.type == V)
        x2 = *arg.id->value;
    else
        x2 = arg;
    if(x1.type == I)
        y1 = x1.i;
    else if(x1.type == D)
        y1 = x1.d;
    else
        iferr = 1;
    if(x2.type == I)
    {
        y2 = x2.i;
        if(iferr)
        {
            if(x1.type == S)
                printf("TypeError: unsupported operand type(s) for ** or pow(): 'str' and 'int'\n");
            else
                printf("TypeError: unsupported operand type(s) for ** or pow(): 'list' and 'int'\n");
            return result;            
        }
    }
    else if(x2.type == D)
    {
        y2 = x2.d;
        if(iferr)
        {
            if(x1.type == S)
                printf("TypeError: unsupported operand type(s) for ** or pow(): 'str' and 'float'\n");
            else
                printf("TypeError: unsupported operand type(s) for ** or pow(): 'list' and 'float'\n");
            return result;
        }
    }
    else if(x2.type == S)
    {
        if(x1.type == I)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'int' and 'str'\n");
        else if(x1.type == D)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'float' and 'str'\n");
        else if(x1.type == S)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'str' and 'str'\n");
        else if(x1.type == L)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'list' and 'str'\n");
        return result;  
    }
    else
    {
        if(x1.type == I)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'int' and 'list'\n");
        else if(x1.type == D)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'float' and 'list'\n");
        else if(x1.type == S)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'str' and 'list'\n");
        else if(x1.type == L)
            printf("TypeError: unsupported operand type(s) for ** or pow(): 'list' and 'list'\n");
        return result;  
    }
    if((x1.type == I)&&(x2.type == I))
    {
        result.type = I;
        result.i = (int)pow(x1.i,x2.i);
    }
    else
    {
        result.type = D;
        result.d = pow(y1,y2);
    }
    return result;    
}

struct sym sym_abs(struct sym object)
{
    struct sym result,x;
    result.type = NONTYPE;
    if(object.type == V)
        x = *object.id->value;
    else
        x=object;
    if(x.type == S)
    {
        yyerror("TypeError: bad operand type for abs(): 'str'");
        return result;
    }
    if(x.type == L)
    {
        yyerror("TypeError: bad operand type for abs(): 'list'");
        return result;
    }
    if(x.type == I)
    {
        result.type = I;
        result.i = abs(x.i);
        return result;
    }    
    if(x.type == D)
    {
        result.type = D;
        result.d = (x.d>-x.d)?x.d:-x.d;
        return result;
    }    
}    

struct sym sym_int(struct sym object)
{
    struct sym result,x;
    result.type = NONTYPE;
    if(object.type == V)
        x = *object.id->value;
    else
        x=object;
    if(x.type == L)
    {
        yyerror("TypeError: int() argument must be a bytes-like object or a number, not 'list'");
        return result;
    }
    if(x.type == S)
    {
        yyerror("TypeError: int() argument must be a bytes-like object or a number, not 'str'");
        return result;
    }
    result.type = I;
    if(x.type == I)
    {
        result.i = x.i;
        return result;
    }
    if(x.type == D)
    {
        result.i = (int)x.d;
        return result;
    }
}

struct sym sym_sqrt(struct sym object)
{
    struct sym result,x;
    result.type = NONTYPE;
    if(object.type == V)
        x = *object.id->value;
    else
        x=object;
    if(x.type == L)
    {
        yyerror("TypeError: int() argument must be a bytes-like object or a number, not 'list'");
        return result;
    }
    if(x.type == S)
    {
        yyerror("TypeError: int() argument must be a bytes-like object or a number, not 'str'");
        return result;
    }
    result.type = D;
    if(x.type == I)
    {
        result.d = sqrt(x.i);
        return result;
    }
    if(x.type == D)
    {
        result.d = sqrt(x.d);
        return result;
    }
}

struct sym replace(struct sym object, struct sym arg1,struct sym arg2){
    struct sym result;
    result.type = NONTYPE;
    if (check_str(object) == 0||check_str(arg1) == 0||check_str(arg2) == 0) {
        yyerror("Typeerror:Need string!");
        return result;
    }
    result.type=S;
    result.s=strdup(get_str(object));
    char *s1=get_str(arg1);char *s2=get_str(arg2);
    int len1=(int)strlen(s1);int len2=(int)strlen(s2);
    char *p;int n=0;
    //printf("%s,%s,%s",result.s,s1,s2);
    while((p=strstr(result.s+n,s1))){
        int len=(int)strlen(result.s);
        char *tmp=result.s;
        result.s=(char *)malloc((len-len1+len2+1)* sizeof(char));
        result.s[len-len1+len2]='\0';
        n=(int)(p-tmp);
        strncpy(result.s,tmp,n);
        strcat(result.s,s2);
        strcat(result.s,p+len1);
       // printf("%s",result.s);
        n=n+len2;
    }
    return result;
}
struct sym split(struct sym object,struct sym arg1){
    struct sym result;
    result.type = NONTYPE;
    if (check_str(object) == 0||check_str(arg1) == 0) {
        yyerror("Typeerror:Need string!");
        return result;
    }
    result.type=L;result.list=NULL;
    char *s=get_str(object);char *s1=get_str(arg1);
    char *p;struct sym tmp;
    p = strtok(s, s1);
    while(p)
    {
        tmp.type=S;tmp.s=strdup(p);
       result.list=addnode_list(result.list,tmp);
        p = strtok(NULL, s1);
    }
    return result;
}

int list_equal(struct l_node *list1,struct l_node *list2)
{
	int i;
	struct l_node *node1,*node2;
	int len1=list_len(list1);
	int len2=list_len(list2);
	if(len1!=len2)
		return 0;
	node1=list1;
	node2=list2;
	for(i=1;i<=len1;i++)
	{
		if(value_equal(node1->value,node2->value)==0)
			return 0;
		node1=node1->next;
		node2=node2->next;
	}
	return 1;
}

int value_equal(struct sym *op1,struct sym *op2)
{
	if(op1->type!=op2->type)
		return 0;
	if(op1->type==0)
		if(op1->i==op2->i)
			return 1;
	if(op1->type==1)
		if(op1->d==op2->d)
			return 1;
	if(op1->type==2)
		if(strcmp(op1->s,op2->s)==0)
			return 1;
	if(op1->type==3)
		if(list_equal(op1->list,op2->list))
			return 1;
	return 0;
}

int membership_check1(struct sym *member,struct l_node *list1)
{
	int i;
	int len;
	struct sym *ptr;
	struct l_node *node1;
	ptr=member;
	node1=list1;

	len=list_len(node1);
	for(i=1;i<=len;i++)
	{
		if(value_equal(ptr,node1->value))
			return 1;
		node1=node1->next;
	}
	return 0;
}

struct sym membership_check(struct sym member,struct sym list1)
{
	struct sym result;
	result.type=-1;
    if(list1.type==V){
        if(list1.id->value==NULL){
            char *s = get_idname(list1);
            printf("NameError: name '%s' is not defined\n", s);
            return result;
        }
    }
	if(check_list(list1)==0)
	{
		yyerror("TypeError");
		return result;
	}
    if(member.type==V){
        if(member.id->value==NULL){
            char *s = get_idname(member);
            printf("NameError: name '%s' is not defined\n", s);
            return result;
        }
    }
	struct sym *ptr;
	struct l_node *node1;
	ptr=get_sym_pointer(&member);
	node1=get_listhead(list1);
	int flag=membership_check1(ptr,node1);
	if(flag==1){
		result.type=BOOL;
        result.i=1;
    }
	else{
		result.type=BOOL;
        result.i=0;
    }
	return result;
}
int get_int(struct sym object){
    if(object.type==I)
        return  object.i;
    if(object.type==V)
        return object.id->value->i;
}
struct sym del(struct sym object, struct sym arg1) {
    struct sym result;
    result.type = NONTYPE;
    if (object.type == V) {
        if (object.id->value == NULL) {
            char *s = get_idname(object);
            printf("NameError: name '%s' is not defined\n", s);
            return result;
        }
    }
    if (arg1.type == V) {
        if (arg1.id->value == NULL) {
            char *s = get_idname(arg1);
            printf("NameError: name '%s' is not defined\n", s);
            return result;
        }
    }
    if (check_list(object) == 0) {
        yyerror("Typeerror:Need list!");
        return result;
    }
    if (check_int(arg1) == 0) {
        return result;
    }
    struct l_node *l = get_listhead(object);
    int m = get_int(arg1);
    int len = list_len(l);
    if (m > len - 1 || m < -len) {//这样以后len不为0
        yyerror("IndexError: list assignment index out of range");
        return result;
    }

    if (m < 0)
        m = m + len;
    if (object.type == V) {
        if (len == 1) {
            object.id->value->list = NULL;
            return result;
        } else  {
            int i;
            struct l_node *p;
            for (i = 0; i < m; i++)
                l = l->next;
            if (m == 0) {
                object.id->value->list = l->next;
                l->next->prior = l->prior;
                l->prior->next = l->next;
            } else {
                l->next->prior = l->prior;
                l->prior->next = l->next;
            }
        }
    } else if(object.type==IND){
        if (len == 1) {
            object.indexing->list= NULL;
            return result;
        } else {
            int i;
            struct l_node *p;
            for (i = 0; i < m; i++)
                l = l->next;
            if (m == 0) {
                object.indexing->list = l->next;
                l->next->prior = l->prior;
                l->prior->next = l->next;
            } else {
                l->next->prior = l->prior;
                l->prior->next = l->next;
            }
        }
    }
    return result;
}

int judgesame(struct sym arg1, struct sym arg2)
{
    if(arg1.type != arg2.type)
        return 0;
    if(arg1.type == I)
    {
        if(arg1.i == arg2.i)
            return 1;
        return 0;
    }
    if(arg1.type == D)
    {
        if(arg1.d == arg2.d)
            return 1;
        return 0;
    }
    if(arg1.type == S)
    {
        if(strcmp(arg1.s,arg2.s) == 0)
            return 1;
        return 0;
    }
    if(arg1.type == L)
    {
        struct l_node *l1 = arg1.list;
        struct l_node *l2 = arg2.list;
        struct l_node *p1 = l1;
        struct l_node *p2 = l2;
        do
        {
            if(!judgesame(*p1->value,*p2->value))
                return 0;
            p1 = p1->next;
            p2 = p2->next;
            if(((p1 == l1)&&(p2 != l2))||((p1 != l1)&&(p2 == l2)))
                return 0;
        }while((p1 == l1)&&(p2 == l2));
        return 1;
    }
    return 1;
}

struct sym sym_remove(struct sym object, struct sym arg)
{
    struct sym result;
    result.type = NONTYPE;
    if(object.type == L)
    {
        return result;
    }
    struct sym x;
    if(object.type == V)
        x = *object.id->value;
    else
        x=object;
    if(x.type == I)
    {
        yyerror("AttributeError: 'int' object has no attribute 'remove'");
        return result;
    }
    if(x.type == D)
    {
        yyerror("AttributeError: 'float' object has no attribute 'remove'");
        return result;
    }
    if(x.type == S)
    {
        yyerror("AttributeError: 'str' object has no attribute 'remove'");
        return result;
    }
    if(arg.type == V)
        arg = *arg.id->value;
    struct sym *y = object.id->value;
    struct l_node *z1,*z2;
    z1 = y->list;
    z2 = z1;
    if(judgesame(*z1->value,arg))
    {
        y->list = z1->next;
        y->list->prior = z1->prior;
        z1->prior->next = y->list;
        return result;
    }
    z1 = z1->next;
    while(z1 != z2)
    {
        if(judgesame(*z1->value,arg))
        {
            z1->next->prior = z1->prior;
            z1->prior->next = z1->next;
            return result;
        }
        z1 = z1->next;
    }
    yyerror("ValueError: list.remove(x): x not in list");
    return result;    
}

struct sym *remove_node(struct l_node *target_node)
{
	struct l_node *a,*b;
	a=target_node->prior;
	b=target_node->next;
	a->next=b;
	b->prior=a;
	return target_node->value;
}

struct sym list_pop(struct sym target_list,struct sym offset)
{
	struct sym result;
	if(target_list.type==4)
		if(target_list.id->value==NULL)
		{
			printf("NameError: name '%s' is not defined\n",target_list.id->name);
			result.type=-1;
			return result;
		}
	if(offset.type==4)
		if(offset.id->value==NULL)
		{
			printf("NameError: name '%s' is not defined\n",offset.id->name);
			result.type=-1;
			return result;
		}
	
	struct sym *ptr_list,*ptr_offset;
	ptr_list=get_sym_pointer(&target_list);
	ptr_offset=get_sym_pointer(&offset);
	if(ptr_list->type!=3)
	{
		printf("TypeError:the target is not a list\n");
		result.type=-1;
		return result;
	}
	if(ptr_offset->type!=0)
	{
		printf("TypeError:the index is not an integer\n");
		result.type=-1;
		return result;
	}
	
	int num_offset,length;
	num_offset=ptr_offset->i;
	length=list_len(ptr_list->list);
	if(length==0)
	{
		printf("IndexError: pop from empty list\n");
		result.type=-1;
		return result;
	}
	if(num_offset>=length)
	{
		printf("IndexError: pop index out of range\n");
		result.type=-1;
		return result;
	}
	if(length==1)
	{
		struct l_node *ptr_node=ptr_list->list;
		ptr_list->list=NULL;
		memcpy(&result,ptr_node->value,sizeof(struct sym));
		return result;
	}
	
	int i;
	struct l_node *target_node;
	struct sym *ptr_l;
	target_node=ptr_list->list;
	for(i=1;i<=num_offset;i++)
		target_node=target_node->next;
	if(num_offset==0)
		ptr_list->list=ptr_list->list->next;
	ptr_l=remove_node(target_node);
	memcpy(&result,ptr_l,sizeof(struct sym));
	return result;
}

void list_insert_operator(struct l_node *list2,int offset2,struct sym *object2)
{
	int i;
	struct l_node *x,*a,*b;
	x=(struct l_node *)malloc(sizeof(struct l_node));
	x->value=(struct sym *)malloc(sizeof(struct sym));
	memcpy(x->value,object2,sizeof(struct sym));
	b=list2;
	for(i=1;i<=offset2;i++)
		b=b->next;
	a=b->prior;
	x->next=b;
	b->prior=x;
	x->prior=a;
	a->next=x;
	return;
}

struct sym list_insert(struct sym list1,struct sym offset1,struct sym object1)
{
	struct sym result;
	result.type=-1;
	if(list1.type==4)
		if(list1.id->value==NULL)
		{
			printf("NameError: name '%s' is not defined\n",list1.id->name);
			return result;
		}
	if(offset1.type==4)
		if(offset1.id->value==NULL)
		{
			printf("NameError: name '%s' is not defined\n",offset1.id->name);
			return result;
		}
	if(object1.type==4)
		if(object1.id->value==NULL)
		{
			printf("NameError: name '%s' is not defined\n",object1.id->name);
			return result;
		}
	
	struct sym *ptr1,*ptr2,*ptr3;
	ptr1=get_sym_pointer(&list1);
	ptr2=get_sym_pointer(&offset1);
	ptr3=get_sym_pointer(&object1);
	if(ptr1->type!=3)
	{
		printf("TypeError:the target is not a list\n");
		return result;
	}
	if(ptr2->type!=0)
	{
		printf("TypeError:the index is not an integer\n");
		return result;
	}
	
	if(ptr1->list==NULL)
	{
		ptr1->list=(struct l_node *)malloc(sizeof(struct l_node));
		ptr1->list->next=ptr1->list;
		ptr1->list->prior=ptr1->list;
		ptr1->list->value=(struct sym *)malloc(sizeof(struct sym));
		memcpy(ptr1->list->value,ptr3,sizeof(struct sym));
		return result;
	}
	
	struct l_node *listptr=ptr1->list;
	int offseta=ptr2->i;
	struct sym *objectptr=ptr3;
	int length=list_len(listptr);
	if(offseta>length)
		offseta=length;
		
	if(offseta==0)
	{
		struct l_node *x,*a,*b;
		x=(struct l_node *)malloc(sizeof(struct l_node));
		x->value=(struct sym *)malloc(sizeof(struct sym));
		memcpy(x->value,objectptr,sizeof(struct sym));
		a=ptr1->list;
		b=a->prior;
		x->next=a;
		a->prior=x;
		x->prior=b;
		b->next=x;
		ptr1->list=x;
		return result;
	}
	
	list_insert_operator(listptr,offseta,objectptr);
	return result;
}

struct sym list_clear(struct sym object)
{
    struct sym result;
    result.type = NONTYPE;
    if(object.type == S)
    {
        yyerror("AttributeError: 'str' object has no attribute 'clear'");
        return result;
    }
    if(object.type == L)
        return result;
    struct sym x;
    x = *object.id->value;
    if(x.type == I)
    {
        yyerror("AttributeError: 'int' object has no attribute 'clear'");
        return result;
    }
    if(x.type == D)
    {
        yyerror("AttributeError: 'float' object has no attribute 'clear'");
        return result;
    }
    if(x.type == S)
    {
        yyerror("AttributeError: 'str' object has no attribute 'clear'");
        return result;
    }
    object.id->value->list = NULL;
    return result;
}

struct sym list_index(struct sym object, struct sym arg)
{
    struct sym result;
    result.type = NONTYPE;
    if(object.type == L)
    {
        return result;
    }
    struct sym x;
    if(object.type == V)
        x = *object.id->value;
    else
        x=object;
    if(x.type == I)
    {
        yyerror("AttributeError: 'int' object has no attribute 'remove'");
        return result;
    }
    if(x.type == D)
    {
        yyerror("AttributeError: 'float' object has no attribute 'remove'");
        return result;
    }
    if(x.type == S)
    {
        yyerror("AttributeError: 'str' object has no attribute 'remove'");
        return result;
    }
    if(arg.type == V)
        arg = *arg.id->value;
    struct sym *y = object.id->value;
    struct l_node *z1,*z2;
    z1 = y->list;
    z2 = z1;
    int cnt=0;
    if(judgesame(*z1->value,arg))
    {
        printf("0\n");
        return result;
    }
    z1 = z1->next;
    while(z1 != z2)
    {
        cnt++;
        if(judgesame(*z1->value,arg))
        {
            printf("%d\n",cnt);
            return result;
        }
        z1 = z1->next;
    }
    yyerror("ValueError: list.index(x): x not in list");
    return result;    
}

struct sym func_look(char *s) {
    struct sym result;
    result.type = V;
    if (strcmp(s, "print") == 0) {
        result.type = FUNC1;
        result.func1 = sym_print;
    } else if (strcmp(s, "append") == 0) {
        result.type = FUNC2;
        result.func2 = append;
    } else if (strcmp(s, "len") == 0) {
        result.type = FUNC1;
        result.func1 = len;
    } else if (strcmp(s, "quit") == 0) {
        result.type = FUNC0;
        result.func0 = quit;
    } else if (strcmp(s, "extend") == 0) {
        result.type = FUNC2;
        result.func2 = extend;
    } else if (strcmp(s, "reverse") == 0) {
        result.type = FUNC1;
        result.func1 = reverse;
    } else if (strcmp(s, "sort") == 0) {
        result.type = FUNC1;
        result.func1 = list_sort;
    } else if (strcmp(s, "list") == 0) {
        result.type = FUNC1;
        result.func1 = string_to_list;
    } else if(strcmp(s,"find")==0){
        result.type=FUNC2;
        result.func2=str_find;
    }else if(strcmp(s,"lower")==0){
        result.type=FUNC1;
        result.func1=str_lower;
    }else if(strcmp(s,"range")==0){
        result.type=FUNC3;
        result.func3=range;
    }else if(strcmp(s,"max")==0){
        result.type=FUNC1;
        result.func1=max;
    }else if(strcmp(s,"min")==0){
        result.type=FUNC1;
        result.func1=min;
    }else if(strcmp(s,"pow")==0){
        result.type=FUNC2;
        result.func2=sym_pow;
    }else if(strcmp(s,"abs")==0){
        result.type=FUNC1;
        result.func1=sym_abs;
    }else if(strcmp(s,"int")==0){
        result.type=FUNC1;
        result.func1=sym_int;
    }else if(strcmp(s,"sqrt")==0){
        result.type=FUNC1;
        result.func1=sym_sqrt;
    }else if(strcmp(s,"replace")==0){
        result.type=FUNC3;
        result.func3=replace;
    }else if(strcmp(s,"split")==0){
        result.type=FUNC2;
        result.func2=split;
    }else if(strcmp(s,"remove")==0){
        result.type=FUNC2;
        result.func2=sym_remove;
    }else if(strcmp(s,"pop")==0){
        result.type=FUNC2;
        result.func2=list_pop;
    }else if(strcmp(s,"insert")==0){
        result.type=FUNC3;
        result.func3=list_insert;
    }else if(strcmp(s,"clear")==0){
        result.type=FUNC1;
        result.func1=list_clear;
    }else if(strcmp(s,"index")==0){
        result.type=FUNC2;
        result.func2=list_index;
    }
    return result;
}