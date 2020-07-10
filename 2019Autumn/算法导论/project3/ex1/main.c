#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *s1;
char *s2;
int **c;//c是一个二维数组，存储LCS的长度
int **b;//b是一个二维数组，存储计算c时所选择的子问题的最优解
int m;//s1的长度
int n;//s2的长度

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
int LCS_LENGTH(){//自底向上计算LCS的长度，利用b数组帮助构造最优解
    int i,j;
    for(i=1;i<=m;i++)//初始化
        c[i][0]=0;
    for(j=0;j<=n;j++)
        c[0][j]=0;
    for(i=1;i<=m;i++)
        for(j=1;j<=n;j++){
            if(s1[i]==s2[j]){//若s1 s2最后字符相同，这个最后字符是它们LCS的一个字符，LCS长度加1
                c[i][j]=c[i-1][j-1]+1;
                b[i][j]=0;//Z(k-1)是s1(i-1) s2(j-1)的LCS
            }
            else if(c[i-1][j]>=c[i][j-1]){//若s1(i-1) s2(j)的LCS长于s1(i) s2(j-1)
                c[i][j]=c[i-1][j];
                b[i][j]=1;//记录Z是s1(i-1) s2(j)的LCS
            }
            else{
                c[i][j]=c[i][j-1];
                b[i][j]=2;
            }
        }
    return 1;

}
void PRINT_LCS(int i,int j){
    if(i==0||j==0)
        return ;
    if(b[i][j]==0){
        PRINT_LCS(i-1,j-1);
        printf("%c",s1[i]);
    }
    else if(b[i][j]==1)
        PRINT_LCS(i-1,j);
    else
        PRINT_LCS(i,j-1);
    return ;
}

int main() {
    int i;
    printf("Please input the length of s1:\nm=");
    scanf("%d",&m);
    printf("Please input the length of s2:\nn=");
    scanf("%d",&n);
    s1=(char *)malloc((m+2)*sizeof(char));
    s2=(char *)malloc((n+2)* sizeof(char));
    srand(time(NULL));
    Generate_Str(s1,m+1);
    Generate_Str(s2,n+1);
    printf("s1:\n%s\n",s1+1);
    printf("s2:\n%s\n",s2+1);
    c=(int **)malloc((m+1)*sizeof(int *));
    for(i=0;i<m+1;i++)
        c[i]=(int *)malloc((n+1)* sizeof(int));
    b=(int **)malloc((m+1)*sizeof(int *));
    for(i=0;i<m+1;i++)
        b[i]=(int *)malloc((n+1)* sizeof(int));
    printf("LSC:\n");
    LCS_LENGTH();
    PRINT_LCS(m,n);
}