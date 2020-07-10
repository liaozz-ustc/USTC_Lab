#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NSYMS 100 /*符号表大小*/
enum cal_type {
    ADD = 0, SUB, MUL, DIV, MOD
};
/*struct slicing_address {
    struct l_node *head;
    struct l_node *tail;
};*/
struct sym {
    int type;//类型
    union {
        int i;
        double d;
        char *s;
        struct l_node *list;  //指向一个列表的头节点
        struct symtab *id;//变量，则指向其在符号表中的位置
        struct sym *indexing;//索引赋值返回的sym地址
        //struct slicing_address *slicing;
        struct sym (*func0)();//无参函数
        struct sym (*func1)(struct sym object);//一个参数的函数指针
        struct sym (*func2)(struct sym object,struct sym arg1);//两个参数的函数指针
        struct sym (*func3)(struct sym object,struct sym arg1,struct sym arg2);//三个参数的函数指针
        struct arg_node *arglist;//参数表，为yacc中识别函数调用保留变量，调用函数，参数
    };
};//(symbol)
struct arg_node{//参数链表，第一个参数总是变量或常量，第二个参数是函数，之后的就是调用函数的参数
    struct sym *arg;
    struct arg_node *next;
};
struct symtab {
    char *name;//变量名
    struct sym *value;    //域
} symtab[NSYMS];//符号表
struct l_node {//列表的每个元素，用双向链表实现
    struct sym *value;
    struct l_node *next;
    struct l_node *prior;
};
#define NONTYPE -1//空类型，用于空产生式和函数调用的产生式
#define I 0 //int型
#define D 1 //real型
#define S 2 //字符串类型
#define L 3 //列表类型
#define V 4 //变量
#define IND 5//索引类型
#define SLI 6//列表切片类型，字符串切片类型为字符串
#define FUNC0 7//无参函数
#define FUNC1 8//函数类型,函数中只含一个参数
#define FUNC2 9//函数中两个参数
#define FUNC3 10 //函数中含三个参数
#define ARGLIST 11//参数表
#define BOOL 12//布尔值

void yyerror(const char *s);

struct symtab *symlook(char *s);

struct symtab *installsym(char *s, struct sym *v);

struct sym sym_print(struct sym a);

void id_print(struct symtab *i);

void list_print(struct l_node *l);

struct l_node *addnode_list(struct l_node *l, struct sym e);

struct l_node *append_list(struct l_node *l, struct l_node *s);

struct sym cal_add(struct sym op1, struct sym op2);

struct sym cal_sub(struct sym op1, struct sym op2);

struct sym cal_mul(struct sym op1, struct sym op2);

struct sym list_mul(struct sym op1, struct sym op2);

struct sym str_mul(struct sym op1, struct sym op2);

struct sym cal_mod(struct sym op1, struct sym op2);

struct sym cal_div(struct sym op1, struct sym op2);

struct sym cal(struct sym op1, struct sym op2, enum cal_type op);

struct sym indexing(struct sym op1, struct sym op2);

void init_symtab();
//int type_check(struct sym op1,struct sym op2,enum cal_type op);
//struct sym list_sort(struct sym op);
struct sym indexing(struct sym op1, struct sym op2);

struct sym list_indexing(struct sym op1, struct sym op2);

int list_len(struct l_node *l);

struct sym str_indexing(struct sym op1, struct sym op2);

struct sym slicing(struct sym data, struct sym op1, struct sym op2, struct sym op3);

int slicing_type_check(struct sym data, struct sym op1, struct sym op2, struct sym op3);

int slicing_prepa(int len, struct sym op1, struct sym op2, int op3, int *index1, int *index2);
struct sym list_slicing(struct sym data, struct sym op1, struct sym op2, int op3);

struct sym str_slicing(struct sym data, struct sym op1, struct sym op2, int op3);
struct l_node *addnode_list_getaddress(struct l_node *l, struct sym *e);

struct sym assign(struct sym dest,struct sym source);//内包类型检查
struct l_node *list_assign(struct l_node *dest,struct l_node *source);
struct sym func_look(char *s);
struct arg_node *addnode_arglist(struct arg_node *l,struct sym arg);
struct sym functions(struct sym func,struct sym arg);

int arglist_len(struct arg_node *l);
struct sym append(struct sym object, struct sym arg);
int check_list(struct sym object);
struct l_node *get_listhead(struct sym object);
struct sym len(struct sym object);
char *get_str(struct sym object);
int check_str(struct sym object);
struct sym quit();

struct sym extend(struct sym object,struct sym arg);
struct sym reverse(struct sym object);

struct sym list_sort(struct sym source);
struct sym list_sort1(struct sym op1);
struct sym string_to_list(struct sym object);
struct sym string_to_list1(struct sym source);

char *get_idname(struct sym objest);
int check_int(struct sym object);
int check_id(struct sym object);
struct sym str_find(struct sym object,struct sym arg);
struct sym str_lower(struct sym object);

struct sym *get_sym_pointer(struct sym *object);
int type_check1(int type1,int type2,enum cal_type op);
int type_check(struct sym op1,struct sym op2,enum cal_type op);
struct sym range(struct sym object, struct sym arg1, struct sym arg2);
struct sym max(struct sym object);
struct sym min(struct sym object);
struct sym sym_pow(struct sym object, struct sym arg);
struct sym sym_abs(struct sym object);
struct sym sym_int(struct sym object);
struct sym sym_sqrt(struct sym object);

struct sym replace(struct sym object, struct sym arg1,struct sym arg2);
struct sym split(struct sym object,struct sym arg1);

struct sym membership_check(struct sym member,struct sym list1);
int membership_check1(struct sym *member,struct l_node *list1);
int value_equal(struct sym *op1,struct sym *op2);
int list_equal(struct l_node *list1,struct l_node *list2);
struct sym del(struct sym object,struct sym arg1);
int get_int(struct sym object);
int judgesame(struct sym arg1, struct sym arg2);
struct sym sym_remove(struct sym object, struct sym arg);
struct sym *remove_node(struct l_node *target_node);
struct sym list_pop(struct sym target_list,struct sym offset);
void list_insert_operator(struct l_node *list2,int offset2,struct sym *object2);
struct sym list_insert(struct sym list1,struct sym offset1,struct sym object1);
struct sym list_clear(struct sym object);
struct sym list_index(struct sym object, struct sym arg);