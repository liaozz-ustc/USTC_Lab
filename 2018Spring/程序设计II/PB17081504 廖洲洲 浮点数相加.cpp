#include<stdio.h>
#include<string.h>

int a1[110];
int a2[110];
int b1[110];
int b2[110];

char szLine1[110];
char szLine2[110];

int main() {
	scanf("%s",szLine1);    //printf("%c",szLine1[0]);
	scanf("%s",szLine2);
	int i,j=0;
	int count1=0,count2=0;   
	int countb1=0,countb2=0;
	int flag=-1;
	memset(a1,0,sizeof(a1));
	memset(b1,0,sizeof(b1));
	memset(a2,0,sizeof(a2));
	memset(b2,0,sizeof(b2));
	
	
	int nlen1=strlen(szLine1);   // printf("length--%d",nlen1);
	for(i=0 ; szLine1[i]!='.'&& i<=nlen1-1;i++);
	count1=i;
	count2=nlen1-count1-1;
	
//	printf("--count1=%d--count2=%d    ",count1,count2);
	
	for(i=0;i<=count1-1;i++) {
		a1[count1-1-i]=szLine1[i]-'0';

	}  //printf("%d--%d---%d---%d--\n",a1[3],a1[2],a1[1],a1[0]);

	for(i=count1+1;i<=nlen1-1;i++)
		a2[j++]=szLine1[i]-'0';
	
//	printf("%d--%d---%d---%d--\n",a2[0],a2[1],a2[2],a2[3]);


	int nlen2=strlen(szLine2);    //printf("length2--%d",nlen2);
	for(i=0 ; szLine2[i]!='.'&& i<=nlen2-1;i++);
	countb1=i;
	countb2=nlen2-countb1-1;
	
//	printf("--countb1=%d--countb2=%d    ",countb1,countb2);
	
	for(i=0;i<=countb1-1;i++) {
		b1[countb1-1-i]=szLine2[i]-'0';

	} // printf("%d--%d---%d---%d--\n",b1[3],b1[2],b1[1],b1[0]);

	j=0;
	for(i=countb1+1;i<=nlen2-1;i++)
		b2[j++]=szLine2[i]-'0';
	
//	printf("%d--%d---%d---%d--\n",b2[0],b2[1],b2[2],b2[3]);

	for(i=105;i>=0;i--)	{
		a2[i]+=b2[i];


		if(a2[i]>=10)	{
			if(i==0) {
				a1[0]++;
				a2[i]=a2[i]-10;
				}
			else {
				a2[i-1]++;
				a2[i]=a2[i]-10;
				}
			}
	}
	
	for(i=0;i<=105;i++)
		if(a2[i]>0)
			flag=i;
//	for(i=0;i<=20;i++)
//		printf("-%d-",a2[i]);
//	printf("flag=%d\n",flag);

	for(i=0;i<=107;i++) {
		a1[i]+=b1[i];

		if(a1[i]>=10) {
			a1[i+1]++;
			a1[i]=a1[i]-10;
			}
		}	

	int p=0;
	for(i=107;i>=0;i--) {
		if(p)
			printf("%d",a1[i]);
		else if(a1[i]) {
			printf("%d",a1[i]);
			p=1;
				}
			}
	
	if(p==0)
		printf("0");
	if(flag==-1)
		return 1;
	else {
	
	printf(".");
	
	for(i=0;i<=flag;i++)
	printf("%d",a2[i]);
	
	}

}
