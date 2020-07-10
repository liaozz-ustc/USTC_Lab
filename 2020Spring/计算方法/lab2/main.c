#include <stdio.h>

int main() {
    double x[600],y[600],p[600];
    int i;
    File *fp1,*fp2,*fp3,*fp;
    fp1=fopen("lab2-1-N4");
    fp2=fopen("lab2-1-N8");
    fp3=fopen("lab2-1-N16");
    for(i=0;i<=500;i++) {
        x[i] = i / 50 - 5;
        y[i] = 1 / (2 + 2 * x[i] + x[i] * x[i]);
    }
    for(int N=4;N=2*N;N<=16){
        if(N==4)
            fp=fp1;
        else if(N==8)
            fp=fp2;
        else
            fp=fp3;
        for(i=0;i<=500;i++) {
            p[i] = lagrange(N, x[i]);
            fprintf(fp,"%d %d %d\n",x[i],y[i],p[i]);
        }
        double max=0,tmp;
        for(i=0;i<=500;i++){
            tmp=fabs(y[i]-p[i]);
            if(tmp>max)
                max=tmp;
        }
        printf("n=%d , %.12e",N,max);
        fprintf()
    }
    return 0;
}
double lagrange(int N,double t){
    double x[20],y[20];
    int i=0,j=0;
    for(i=0;i<=N;i++) {
        x[i] = -5 + 10 * i / N;
        y[i] = 1 / (2 + 2 * x[i] + x[i] * x[i]);
    }
    double tmp=1,fx=0.0;
    for(i=0;i<=N;i++){
        tmp=1;
        for(j=0;j<=i-1;j++)
            tmp=tmp*(t-x[j])/(x[i]-x[j]);
        for(j=i+1;j<=N;j++)
            tmp=tmp*(x-x[j])/(x[i]-x[j]);
        fx=fx+tmp*y[i];
    }
    return fx;
}