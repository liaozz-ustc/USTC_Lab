#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846
double lagrange(int N,double t){
    double x[20],y[20];
    int i=0,j=0;
    for(i=0;i<=N;i++) {
        x[i] = -5*cos((2*(double)i+1)*PI/(2*N+2));
        //x[i]=-5+10*((double)i)/(double)N;
        y[i] = 1 / (2 + 2 * x[i] + x[i] * x[i]);
    }
    double tmp=1,fx=0.0;
    for(i=0;i<=N;i++){
        tmp=1;
        for(j=0;j<=i-1;j++)
            tmp=tmp*(t-x[j])/(x[i]-x[j]);
        for(j=i+1;j<=N;j++)
            tmp=tmp*(t-x[j])/(x[i]-x[j]);
        fx=fx+tmp*y[i];
    }
    return fx;
}
int main() {
    double x[600],y[600],p[600];
    int i;
    FILE *fp;
    FILE *fp1=fopen("E:\\Numercial Method\\lab2\\lab2-1\\lab2-2-N4.txt","w");
    FILE *fp2=fopen("E:\\Numercial Method\\lab2\\lab2-1\\lab2-2-N8.txt","w");
    FILE *fp3=fopen("E:\\Numercial Method\\lab2\\lab2-1\\lab2-2-N16.txt","w");
    for(i=0;i<=500;i++) {
        x[i] = ((double)i) / 50 - 5;
        y[i] = 1 / (2 + 2 * x[i] + x[i] * x[i]);
    }
    for(int N=4;N<=16;N=2*N){
        if(N==4)
            fp=fp1;
        else if(N==8)
            fp=fp2;
        else
            fp=fp3;
        for(i=0;i<=500;i++) {
            p[i] = lagrange(N, x[i]);
            fprintf(fp,"%.12lf %.12lf %.12lf\n",x[i],y[i],p[i]);
        }
        double max=0,tmp;
        for(i=0;i<=500;i++){
            tmp=fabs(y[i]-p[i]);
            if(tmp>max)
                max=tmp;
        }
        printf("n=%d , %.12e\n",N,max);
    }
    return 0;
}
