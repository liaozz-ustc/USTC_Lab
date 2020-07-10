#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
char *T;
char *T_found;
char *P;
int n;
int m;

void Generate_Str(char *s,int len){
    s[0]='0';
    s[len]='\0';
    int flag;
    for(int i=1;i<len;i++){
        flag=rand()%3;
        if(flag==0){
            s[i] = rand()%26 +'a';
        }
        else if(flag==1){
            s[i] = rand()%26 +'A';
        }
        else{
            s[i] = rand()%10 +'0';
        }
    }
}
int STR_CMP(char *s1,char*s2,int len){
    int i;
    for(i=1;i<=m;i++){
        T_found[i]=s2[i];
        if(s1[i]!=s2[i])
            return 0;
    }
    return 1;
}
int RABIN_KARP_MATCHER(int d,int q){//将字符串视为d进制数，求出字符串对q求模的值，然后将模式串的值与要比较的字符串的值比较
    //之后再对伪命中点进行再检测
    int h=1;//最高位数为1时的值
    int p=0;
    int t=0;
    int s=0;//偏移量
    for(int i=1;i<=m-1;i++)
        h=(h*d)%q;//即d的m-1次方模q的值
    printf("d=%d,q=%d,h=%d\n",d,q,h);
    for(int i=1;i<=m;i++){
        p=(d*p+P[i]-'0')%q;//求得模式串对应的值
        t=(d*t+T[i]-'0')%q;//求得t0
    }
    for(s=0;s<=n-m;s++) {
        if (p == t) {//伪命中点
            if (STR_CMP(P, T + s, m)) {
                printf("FOUND:s=%d\n", s);
                printf("String in T:%s\n", T_found + 1);
            }
        }
        if (s < (n - m)) {
            t = (d * (t - (T[s + 1] - '0') * h) + T[s + 1 + m] - '0') % q;
            if(t<0)
                t+=q;
        }
    }
    return 1;
}
int main() {
    int i;
    printf("Please input the length of T:\nn=");
    scanf("%d",&n);
    printf("Please input the length of P:\nm=");
    scanf("%d",&m);
    T=(char *)malloc((n+2)*sizeof(char));
    P=(char *)malloc((m+2)* sizeof(char));

    T_found=(char *)malloc((m+2)* sizeof(char));
    T_found[m+1]='\0';
    T_found[0]=0;
    srand(time(NULL));
    Generate_Str(T,n+1);
    //Generate_Str(P,m+1);
    for(i=1;i<=m;i++)
        P[i]=T[12+i];
    P[i]='\0';
    printf("T:\n%s\n",T+1);
    printf("P:\n%s\n",P+1);
    int d='z'-'0'+1;//d=75,75进制
    int q=79;//选取的素数为13
    RABIN_KARP_MATCHER(d,q);
}