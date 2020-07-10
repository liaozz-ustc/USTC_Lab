#include <stdio.h>
#include <math.h>
#include <stdlib.h>
double f(int n,double *num,double *index,double x){
    int i;double result=0;
    for(i=0;i<n;i++){
        if(num[i]!=0)
            result+=num[i]*pow(x,index[i]);
    }
    return result;
}
double g(int n,double *num,double *index,double *num2,double *index2,double x){
    return (x-f(n,num,index,x)/f(n,num2,index2,x));
}
int Newton(int n,double *num,double *index){
    int k=0;double x0,x1;
    printf("请输入x0:");
    scanf("%lf",&x0);
    double *num2,*index2;
    num2 = (double *)malloc(sizeof(double)*n);
    index2 = (double *)malloc(sizeof(double)*n);
    int i;
    for(i=0;i<n;i++){
        num2[i]=num[i]*index[i];
        index2[i]=index[i]-1;
    }
    printf("g(x)=");
    for(i=0;i<n;i++){
        printf("%lf",num2[i]);
        printf("*x^%lf",index2[i]);
        if(i!=n-1)
            printf("+");
    }
    printf("\n");
    printf("迭代步数k\t\tx_k\t\t\t\tf(x_k)\n");
    double f_x;
    f_x=f(n,num,index,x0);
    printf("k=%d\t\t%.10e\t\t%.10e\n",k,x0,f_x);
    while(fabs(f_x)>=pow(10,-9)){
        k++;
        x0=g(n,num,index,num2,index2,x0);
        f_x=f(n,num,index,x0);
        printf("k=%d\t\t%.10e\t\t%.10e\n",k,x0,f_x);
    }
    return 1;
}
int Subtense(int n,double *num,double *index){
    int k=0;double x0,x1,f_x0,f_x1,x,f_x;
    printf("请输入x0,x1:");
    scanf("%lf %lf",&x0,&x1);
    f_x0=f(n,num,index,x0);
    printf("迭代步数k\t\tx_k\t\t\t\tf(x_k)\n");
    printf("k=%d\t\t%.10e\t\t%.10e\n",k,x0,f_x0);

    do {
        k++;
        f_x1=f(n,num,index,x1);
        printf("k=%d\t\t%.10e\t\t%.10e\n",k,x1,f_x1);
        x = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);
        x0=x1;f_x0=f_x1;
        x1=x;
    }while(fabs(f_x0)>=pow(10,-9));

    return 1;

}
int main() {
    int n;double *num,*index;
    printf("请输入方程的项数：\n");
    scanf("%d",&n);
    num = (double *)malloc(sizeof(double)*n);
    index = (double *)malloc(sizeof(double)*n);
    printf("请输入系数和指数：\n");
    int i;
    for(i=0;i<n;i++){
        scanf("%lf",&num[i]);
        scanf("%lf",&index[i]);
    }
    printf("f(x)=");
    for(i=0;i<n;i++){
        printf("%lf",num[i]);
        printf("*x^%lf",index[i]);
        if(i!=n-1)
            printf("+");
    }
    printf("\n");
    printf("1:Newton,2:弦截法\n");
    int flag;
    scanf("%d",&flag);
    if(flag==1)
        Newton(n,num,index);
    else
        Subtense(n,num,index);
    return 0;
}