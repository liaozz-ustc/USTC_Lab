#include <stdio.h>
#include <string.h>
void myadd(int *a,int *b,int *c){
    c[1]=-2;
    int i;int tmp=0;int flag=0;
    for(i=24;i>=2;i--){
        tmp=a[i]+b[i]+flag;
        if(tmp>=10){
            c[i]=tmp-10;
            flag=1;
        } else{
            c[i]=tmp;
            flag=0;
        }
    }
    c[0]=a[0]+b[0]+flag;
}
void mysub(int *a,int *b,int *c){
    c[1]=-2;
    int i;int tmp=0;int flag=0;
    for(i=24;i>=2;i--){
        tmp=a[i]-b[i]+flag;
        if(tmp<0){
            c[i]=10+tmp;
            flag=-1;
        } else{
            c[i]=tmp;
            flag=0;
        }
    }
    c[0]=a[0]-b[0]+flag;
}
int main() {
    char data1[25],data2[25],data3[25],data4[25],data5[25];
    int num1[25],num2[25],num3[25],num4[25],num5[25];

    printf("input:");
    scanf("%s",data1);
    scanf("%s",data2);
    scanf("%s",data3);
    scanf("%s",data4);
    scanf("%s",data5);
    int i;
    for(i=0;i<25;i++)
    {
        num1[i]=0;
        num2[i]=0;
        num3[i]=0;
        num4[i]=0;
        num5[i]=0;
    }
    for(i=0;i<strlen(data1);i++){
        num1[i]=data1[i]-'0';
    }
    for(i=0;i<strlen(data2);i++){
        num2[i]=data2[i]-'0';
    }
    for(i=0;i<strlen(data3);i++){
        num3[i]=data3[i]-'0';
    }
    for(i=0;i<strlen(data4);i++){
        num4[i]=data4[i]-'0';
    }
    for(i=0;i<strlen(data5);i++){
        num5[i]=data5[i]-'0';
    }
    for(i=0;i<25;i++){
        printf("%d",num1[i]);
    }
    printf("\n");
    for(i=0;i<25;i++){
        printf("%d",num2[i]);
    }
    printf("\n");
    for(i=0;i<25;i++){
        printf("%d",num3[i]);
    }
    printf("\n");
    for(i=0;i<25;i++){
        printf("%d",num4[i]);
    }
    printf("\n");
    for(i=0;i<25;i++){
        printf("%d",num5[i]);
    }
    printf("\n");
    int result[25];
    myadd(num1,num2,result);
    myadd(result,num3,result);
    mysub(result,num4,result);
    mysub(result,num5,result);
    for(i=0;i<25;i++){
        printf("%d",result[i]);
    }

    return 0;
}
