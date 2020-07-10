#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double *X,*X2,*xx;
double *Y,*Y2,*yy;//存储坐标
struct XY{
    double x1;double y1;double x2;double y2;
};//最近点对的坐标
void exchange(double *x,int i,int j){
    double t;
    t=x[i];
    x[i]=x[j];
    x[j]=t;
}
int partition(double *x,double *y,int p,int r){
    double k=x[r];
    int i=p-1,j;
    for(j=p;j<=r-1;j++){
        if(x[j]<=k){
            i=i+1;
            exchange(x,i,j);
            exchange(y,i,j);
        }
    }
    exchange(x,i+1,r);
    exchange(y,i+1,r);
    return i+1;
}
int quick_sort(double *x,double *y,int p,int r){//以x中元素递增的顺序对x、y排序
    int q;
    if(p<r){
        q=partition(x,y,p,r);
        quick_sort(x,y,p,q-1);
        quick_sort(x,y,q+1,r);
    }
    return 1;
}

double distance(double x1,double y1,double x2,double y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

double Nearest(double *x,double *y,int n, struct XY *result){
    int mid;
    double d=99999999,tmp,tmp2;int i,j,m=0;
    struct XY result1,result2;
    if(n<=3){
        for(i=0;i<n-1;i++)
            for(j=i+1;j<=n-1;j++) {
                    tmp=distance(x[i], y[i], x[j], y[j]);
                if (tmp < d){
                    d =tmp;
                    result->x1=x[i];result->y1=y[i];
                    result->x2=x[j];result->y2=y[j];

                }
            }
    }
    else{
        mid=n/2;
        tmp=Nearest(x,y,mid,&result1);//左子集
        tmp2=Nearest(&x[mid],&y[mid],n-mid,&result2);//右子集
        if(tmp<=tmp2){
            d=tmp;
            result->x1=result1.x1;result->y1=result1.y1;
            result->x2=result1.x2;result->y2=result1.y2;
        }
        else{
            d=tmp2;
            result->x1=result2.x1;result->y1=result2.y1;
            result->x2=result2.x2;result->y2=result2.y2;
        }
        for(i=1;i<=n;i++)
            if(fabs(X2[i]-x[mid])<=d){//距离中间点在d以内的点，保存下来
                m++;
                xx[m]=X2[i];yy[m]=Y2[i];//由于Y2是有序的，故yy也是按y递增有序的
            }
        for(i=1;i<=m;i++){//对范围内的每个点计算其与后7个点的距离
            for(j=i+1;j<=i+7&&j<=m;j++) {
                tmp=distance(xx[i],yy[i],xx[j],yy[j]);
                if(tmp<d){
                    d=tmp;
                    result->x1=xx[i];result->y1=yy[i];
                    result->x2=xx[j];result->y2=yy[j];
                }
            }
        }

    }
    return d;
}
int main(){
    int i,n;
    double d;
    struct XY *result;
    printf("Please input:\nn=");
    scanf("%d",&n);
    result=(struct XY *)malloc(sizeof(struct XY));
    X=(double *)malloc((n+1)* sizeof(double));
    X2=(double *)malloc((n+1)* sizeof(double));
    Y=(double *)malloc((n+1)*sizeof(double));
    Y2=(double *)malloc((n+1)*sizeof(double));
    xx=(double *)malloc((n+1)*sizeof(double));
    yy=(double *)malloc((n+1)*sizeof(double));
    printf("Please input the x,y:\n");
    X[0]=Y[0]=0;X2[0]=0;Y2[0]=0;
   for(i=1;i<=n;i++){
        scanf("%lf %lf",&X[i],&Y[i]);
        X2[i]=X[i];Y2[i]=Y[i];
    }
/*    for(i=1;i<=n;i++){
        printf("(%lf,%lf)",X[i],Y[i]);
    }*/
    quick_sort(X,Y,1,n);//将X、Y以x递增排序
    quick_sort(Y2,X2,1,n);//将X2、Y2以y递增排序

    d=Nearest(&X[1],&Y[1],n,result);
    printf("Nearest distance:d=%lf\n",d);
    printf("(%lf,%lf) (%lf,%lf)",result->x1,result->y1,result->x2,result->y2);


    return 0;
}