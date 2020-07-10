#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct node{
    int key;
    int color;//0为黑，1为红
    struct node *left;
    struct node *right;
    struct node *p;
};
struct RB_tree{
    struct node *root;
    struct node *nil;
};
struct RB_tree T;
int left_rotate(struct node *x){//对x左旋,x的右孩子不为nil
    struct node *y=x->right;//step1
    x->right=y->left;y->left->p=x;//step2
    y->p=x->p;//step3
    if(x->p==T.nil)
        T.root=y;
    else if(x==x->p->left)
        x->p->left=y;
    else
        x->p->right=y;
    y->left=x;x->p=y;//step4
}
int right_rotate(struct node *y){//对y右旋,y的左孩子不为nil
    struct node *x=y->left;//step1
    y->left=x->right;x->right->p=y;//step2
    x->p=y->p;//step3
    if(y->p==T.nil)
        T.root=x;
    else if(y==y->p->left)
        y->p->left=x;
    else
        y->p->right=x;
    x->right=y;y->p=x;
}
int rb_insert_fixup(struct node *z){//调整
    struct node *y;
    while(z->p->color==1){
        //若z为根，z.p.color=black,不进入循环
        //若z父节点为黑，无需调整，不进入循环
        if(z->p==z->p->p->left){//case1,2,3，双亲是祖父的左孩子
            y=z->p->p->right;//y是z的叔叔
            if(y->color==1) {//case1,z的叔叔是红色
                y->color=0;z->p->color=0;
                z->p->p->color=1;
                z=z->p->p;
            }
            else{ //case 2 or 3
                if(z==z->p->right){//case2
                    z=z->p;//上溯到双亲
                    left_rotate(z);
                }//case 3
                z->p->color=0;z->p->p->color=1;
                right_rotate(z->p->p);
            }
        }
        else{//case 4,5,6,双亲是祖父的右孩子
            y=z->p->p->left;//y是z的叔叔
            if(y->color==1) {//case4,z的叔叔是红色
                y->color=0;z->p->color=0;
                z->p->p->color=1;
                z=z->p->p;
            }
            else{ //case 5 or 6,z的叔叔是黑色
                if(z==z->p->left){//case5,z是双亲的左孩子
                    z=z->p;//上溯到双亲
                    right_rotate(z);
                }//case 6
                z->p->color=0;z->p->p->color=1;
                left_rotate(z->p->p);
            }
        }//对称的case4，5，6
    }
    T.root->color=0;
}
int rb_insert(struct node *z){//插入z
    struct node *y=T.nil;//y用于记录当前扫描节点的双亲节点
    struct node *x=T.root;//从根开始扫描
    while(x!=T.nil){
        y=x;
        if(z->key < x->key)
            x=x->left;
        else
            x=x->right;
    }
    z->p=y;//y是z的双亲
    if(y==T.nil)//z插入空树,z为根
        T.root=z;
    else if(z->key<y->key)//z是y的左子插入
        y->left=z;
    else//z是y的右子插入
        y->right=z;
    z->right=T.nil;z->left=T.nil;
    z->color=1;//将z涂红
    rb_insert_fixup(z);
}
int rb_inorder(struct node *x){ //中序遍历
    if(x!=T.nil){
        rb_inorder(x->left);
        printf("%d ",x->key);
        rb_inorder(x->right);
    }
}
struct node *rb_search_key(struct node *x,int k){//按关键字进行查询
    if(x==T.nil || k==x->key)
        return x;
    if(k < x->key)
        return rb_search_key(x->left,k);
    else
        return rb_search_key(x->right,k);
}
struct node* rb_search_order(struct node *x,int *n,struct node **p){//查找第n小的节点,返回节点p
    //注意：在这里进行节点的保存时不能使用*p，而是要使用**p，因为传进函数的是一个临时指针变量，仅能对指针指向的内容进行修改，
    //对该指针进行修改没有用，在函数体外的指针不会变，故需要传进该指针变量的地址，才能保存下查找到的节点地址
    if(x!=T.nil && *n>=0){
        if(*p!=T.nil)
            return *p;
        rb_search_order(x->left,n,p);
        *n=(*n)-1;
        //printf("%d(%d) ",x->key,*n);
        if(*n==0) {
            *p = x;
        }
        rb_search_order(x->right,n,p);
    }
    return T.nil;
}

struct node *treesuccessor(struct node *x){//寻找x的中序后继
    if(x==T.nil) return x;
    if(x->right!=T.nil){
        x=x->right;
        while(x->left!=T.nil)
            x=x->left;
        return x;
    }
    struct node *y=x->p;
    while(y!=T.nil && x==y->right){
        x=y;
        y=y->p;
    }
    return y;
}
int rb_delete_fixup(struct node *x){
    struct node *w;
    while(x!=T.root && x->color==0){
        if(x==x->p->left){//x是双亲的左孩子
            w=x->p->right;//w是x的兄弟
            if(w->color==1){//x的兄弟是红色,case1
                w->color=0;
                x->p->color=1;
                left_rotate(x->p);
                w=x->p->right;
            }//case1转换为case2,3,4
            if(w->left->color==0&&w->right->color==0){//case2,w两个孩子为黑
                w->color=1;
                x=x->p;
            }//x上移
            else {//case 3,4
                if(w->right->color==0){//case3,w右子为黑，左子为红
                    w->left->color=0;
                    w->color=1;
                    right_rotate(w);
                    w=x->p->right;
                }//case3转为case4
                w->color=x->p->color;
                x->p->color=0;
                w->right->color=0;
                left_rotate(x->p);
                x=T.root;
            }
        }
        else{//x是双亲的右孩子
            w=x->p->left;//w是x的兄弟
            if(w->color==1){//x的兄弟是红色,case1
                w->color=0;
                x->p->color=1;
                right_rotate(x->p);
                w=x->p->left;
            }//case1转换为case2,3,4
            if(w->left->color==0&&w->right->color==0){//case2,w两个孩子为黑
                w->color=1;
                x=x->p;
            }//x上移
            else {//case 3,4
                if(w->left->color==0){//case3
                    w->right->color=0;
                    w->color=1;
                    left_rotate(w);
                    w=x->p->left;
                }//case3转为case4
                w->color=x->p->color;
                x->p->color=0;
                w->left->color=0;
                right_rotate(x->p);
                x=T.root;
            }
        }
    }//x是双亲的右孩子
    x->color=0;
}
struct node* rb_delete(struct node *z){//删除z结点
    struct node *y,*x;
    if(z->left==T.nil || z->right==T.nil)//case 1,2
        y=z;
    else
        y=treesuccessor(z);//y是z的中序后继
    //此时，y统一是x的双亲节点，且是要删除的节点
    //x是待连接到y.p的节点，下面要确定x
    if(y->left != T.nil)
        x=y->left;
    else
        x=y->right;
    //以下用x取代y
    x->p=y->p;
    if(y->p==T.nil)
        T.root=x;
    else if(y==y->p->left)
        y->p->left=x;
    else
        y->p->right=x;
    if(y!=z){
        z->key=y->key;
    }
    if(y->color==0)//y是红点，删除不影响，y是黑点，需要调整
        rb_delete_fixup(x);
    return y;//返回实际删除的节点
}


int main() {
    int i,num;
    struct node *p;
    struct node *x;
    srand(time(NULL));
    T.nil=(struct node *)malloc(sizeof(struct node));
    T.nil->color=0;//nil.color=black
    T.root=T.nil;
    printf("Please input the number of the nodes: ");
    scanf("%d",&num);
    printf("Random number:");
    for(i=0;i<num;i++){
        p=(struct node *)malloc(sizeof(struct node));
        p->key=rand()%100;
        printf("%d ",p->key);
        rb_insert(p);
    }
    printf("\ninorder walk:");
    rb_inorder(T.root);
    printf("\nSearch:");
    int tmp=num/4;
    p=T.nil;
    rb_search_order(T.root,&tmp,&p);
    printf("%dth lowest number:%d\n",num/4,p->key);
    tmp=num/2;
    x=T.nil;
    rb_search_order(T.root,&tmp,&x);
    printf("%dth lowest number:%d\n",num/2,x->key);
    //p=treesuccessor(p);
    //printf("%d ",p->key);
    rb_delete(p);
    rb_delete(x);
    printf("inorder walk:");
    rb_inorder(T.root);
    //p=rb_search_order(T.root,num/2);
    //rb_delete(p);
    return 0;
}