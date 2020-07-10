#include<stdio.h>
#include<stdlib.h>
#include<math.h>
struct Polyn {
	float coef; //系数
	int expn;   //指数
	struct Polyn *next;
};
void CreatPolyn(struct Polyn *p,int m);//创建
void AddPolyn(struct Polyn *pa,struct Polyn *pb);//相加
void SubtractPolyn(struct Polyn *pa,struct Polyn *pb);//相减
struct Polyn *MutilplyPolyn(struct Polyn *pa,struct Polyn *pb);//相乘
void Print(struct Polyn *p);//打印
struct Polyn *Mutilply(struct Polyn *pa,struct Polyn *pb);//pa是一个多项式的头指针，pb中只是一个指针，其含多项式的一项
void DestroyPolyn(struct Polyn *pa);//销毁
void ClearPolyn(struct Polyn *pa);//清空
void DPolyn(struct Polyn *pa);//微分
int Df(int m,int n);//用来求m*(m-1)*(m-2)...*(m-n+1)
void DefPolyn(struct Polyn *pa,float m,float n);//定积分
float EvaluationPolyn(struct Polyn *pa,float m);//求值
struct Polyn * IndefPolyn(struct Polyn *pa);//不定积分
main() {
	int m,flag;float x;float a,b,f;
	struct Polyn *pa,*pb,*la;
	printf("创建第一个多项式，请输入其非零项个数\n");
	scanf("%d",&m);
	pa=(struct Polyn *)malloc(sizeof(struct Polyn));
	CreatPolyn(pa,m);
	printf("创建第二个多项式，请输入其非零项个数\n");
	scanf("%d",&m);
	pb=(struct Polyn *)malloc(sizeof(struct Polyn));
	CreatPolyn(pb,m);
	printf("\t功能表\t\n多项式相加--1\n多项式相减--2\n多项式相乘--3\n销毁多项式--4\n清空多项式--5\n求导多项式--6\n");
	printf("不定积分--7\n求值--8\n定积分--9\n");
	printf("请输入你的选择：\n");
	scanf("%d",&flag);
	
	if(flag==1){
		AddPolyn(pa,pb);		printf("加法完成\n");
		Print(pa);  }
 	else if(flag==2){
		SubtractPolyn(pa,pb);	printf("减法完成\n");
		Print(pa);  }
	else if(flag==3) {
		la=MutilplyPolyn(pa,pb);	printf("乘法完成\n");
		Print(la);
		}
	else if(flag==4){
		printf("Destroy First--1,Destroy Second--2\n");
		scanf("%d",&flag);
		if(flag==1) DestroyPolyn(pa);
		else if (flag==2) DestroyPolyn(pb);
		else printf("ERROR!");
	}
	else if(flag==5){
		printf("Clear First--1,Clear Second--2\n");
		scanf("%d",&flag);
		if(flag==1) ClearPolyn(pa);
		else if (flag==2) ClearPolyn(pb);
		else printf("ERROR!");
	}
	else if(flag==6){
		printf("求导 First--1,求导 Second--2\n");
		scanf("%d",&flag);
		if(flag==1)
			DPolyn(pa);
		else if (flag==2)
			DPolyn(pb);
		else printf("ERROR!");
		
	}
	else if(flag==7){
		printf("不定积分 First--1,不定积分 Second--2\n");
		scanf("%d",&flag);
		if(flag==1) {
			la=IndefPolyn(pa);	printf("不定积分积分完成：\n");
			Print(la);}
		else if (flag==2) {
			la=IndefPolyn(pb);	printf("不定积分积分完成：\n");
			Print(la);}
		else printf("ERROR!");
	}
	else if(flag==8){
		printf(" 计算First--1, 计算 Second--2\n");
		scanf("%d",&flag);
		printf("请输入要代入的X：\n");
		scanf("%f",&x);
		if(flag==1) {
			f=EvaluationPolyn(pa,x);
			printf("计算完成：\n");
			printf("P(x)=%f\n",f);}
		else if (flag==2) {
			f=EvaluationPolyn(pb,x);
			printf("计算完成：\n");
			printf("P(x)=%f\n",f);}
		else printf("ERROR!");
	}
	else if(flag==9){
		printf("定积分：First --1,Second --2\n");
		scanf("%d",&flag);
		printf("请输入积分上限和下限：\n");
		scanf("%f%f",&a,&b);
		if(flag==1)
			DefPolyn(pa,a,b);
		else if(flag==2)
			DefPolyn(pb,a,b);
		else
			printf("ERROR\n");
	}
		else
		printf("ERROR\n");

}
void Print(struct Polyn *p) {
	printf("存储的多项式为：\nP(x)=");
	struct Polyn *h=p->next;
	if(h==NULL) printf("0");
	while(h){
		printf("%fx^%d",h->coef,h->expn);
		h=h->next;
		if(h){
			if(h->coef>0)
				printf("+");}
	}
	printf("\n");
}
void CreatPolyn(struct Polyn *p,int m){
	p->coef=0;p->expn=-1;p->next=NULL;
	printf("请依次输入每一项的系数和指数，最后一个数据以回车键结束\n");
	int i;
	struct Polyn *q;
	struct Polyn *h=p;
	for(i=1;i<=m;i++) {
		q=(struct Polyn *)malloc(sizeof(struct Polyn));
		scanf("%f%d",&q->coef,&q->expn);
		h->next=q;
		h=q;
	}
	h->next=NULL;
	printf("创建成功\n");
	Print(p);
}
void AddPolyn(struct Polyn *pa,struct Polyn *pb){
	struct Polyn *p=pa->next;
	struct Polyn *q=pb->next;
	struct Polyn *h=pa,*x;
	while(p&&q) {
		if(p->expn < q->expn) {
			h->next=p;
			h=p;
			p=p->next;
			}
		else if(p->expn > q->expn) {
			h->next=q;
			h=q;
			q=q->next;
		}
		else {
			float sum=p->coef+q->coef;
			if(sum!=0){
			h->next=p;
			h=p;
			p=p->next;
			x=q;
			q=q->next;
			h->coef=sum;
			free(x);
			}
			else{
			x=p;
			p=p->next;
			free(x);
			x=q;
			q=q->next;
			free(x);
			h->next=p;
			}
		}
	}
	h->next=p?p:q;

}

void SubtractPolyn(struct Polyn *pa,struct Polyn *pb) {
	struct Polyn *p=pa->next;
	struct Polyn *q=pb->next;
	struct Polyn *h=pa,*x;
	while(p&&q) {
		if(p->expn < q->expn) {
			h->next=p;
			h=p;
			p=p->next;
			}
		else if(p->expn > q->expn) {
			h->next=q;
			q->coef=-q->coef;
			h=q;q=q->next;
		}
		else {
			float sum=p->coef-q->coef;
			if(sum!=0){
			h->next=p;
			h=p;
			p=p->next;
			x=q;
			q=q->next;
			h->coef=sum;free(x);
			}
			else{
			x=p;
			p=p->next;
			free(x);
			x=q;
			q=q->next;
			free(x);
			h->next=p;
			}
		}
	}
	if(p)
		h->next=p;
	else{
		x=q;
		while(q){
			q->coef=-q->coef;
			q=q->next;
		}
		h->next=x;
	}
}
struct Polyn *Mutilply(struct Polyn *pa,struct Polyn *pb){//对整个多项式pa乘以pb，pb只含一个多项式
	struct Polyn *h,*p=pa->next,*q,*la;
	la=h=(struct Polyn *)malloc(sizeof(struct Polyn));
	h->coef=0;h->expn=-1;h->next=NULL;
	while(p){
		q=(struct Polyn *)malloc(sizeof(struct Polyn));
		q->coef=p->coef*pb->coef;
		q->expn=p->expn+pb->expn;
		p=p->next;
		h->next=q;h=q;
	}
	h->next=NULL;
	return la;
}
struct Polyn *MutilplyPolyn(struct Polyn *pa,struct Polyn *pb) {
	struct Polyn *la,*p,*q=pb->next,*Cpa;//la存储结果
	int i;
	la=(struct Polyn *)malloc(sizeof(Polyn));la->coef=0;la->expn=-1;la->next=NULL;
	while(q){
		Cpa=Mutilply(pa,q);
		AddPolyn(la,Cpa);
		q=q->next;
		}
	return la;

}
void DestroyPolyn(struct Polyn *pa){
	struct Polyn *x;
	while(pa){
	x=pa;
	pa=pa->next;
	free(x);
		}
	printf("多项式已销毁！\n");
}
void ClearPolyn(struct Polyn *pa){
	struct Polyn *h=pa->next;
	while(h){
		h->coef=0;
		h->expn=0;
		h=h->next;
	}
	printf("清空完成，目前储存的多项式为：\n");
	Print(pa);
}
void DPolyn(struct Polyn *pa){
	struct Polyn *h=pa->next,*x,*p=NULL;
	printf("请输入求导阶数\n");
	int n;
	scanf("%d",&n);
	if(n<0)
		printf("ERROR!\n");
	while(h&&h->expn<0){
		p=h;h=h->next;
	}
	while(h&&h->expn<n){
		x=h;h=h->next;free(x);
	}
	if(p==NULL)
		pa->next=h;
	else
		p->next=h;
	h=pa->next;
	while(h){
		h->coef=h->coef*Df(h->expn,n);
		h->expn=h->expn-n;
		h=h->next;
	}
	printf("微分完成，微分后：\n");
	Print(pa);
}
int Df(int m,int n){
	int i;
	int j=m;
	if(!n)
		return 1;
	for(i=1;i<n;i++)
		j=j*(m-i);
 	return j;
}
struct Polyn * IndefPolyn(struct Polyn *pa){
	struct Polyn *p=pa->next;
	while(p){
		p->expn=p->expn+1;
		p->coef=p->coef/p->expn;
		p=p->next;
	}
	return pa;

}
float EvaluationPolyn(struct Polyn *pa,float m){
	float f=0;
	struct Polyn *p=pa->next;
	while(p){
		f+=p->coef*(pow(m,p->expn));
		p=p->next;
	}
	return f;

}
void DefPolyn(struct Polyn *pa,float m,float n){//m为上限，n为下限
	struct Polyn *p;
	float f;
	p=IndefPolyn(pa);
	f=EvaluationPolyn(p,m)-EvaluationPolyn(p,n);
	printf("定积分计算完成：\n");
	printf("结果为：%f\n",f);
}
