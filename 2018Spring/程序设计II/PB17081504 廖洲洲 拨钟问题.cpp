#include<stdio.h>
#include<string.h>
int clocks[10];

int guess()	{
		int i;
		for(i=1;i<=9;i++)	{
				if(clocks[i]%4)
					return 0;
			}
			
		return 1;
}

int work(int a,int b,int c,int d,int e,int f,int g,int h,int i)	{
		for(;a>0;a--)	{
			clocks[1]++;clocks[2]++;clocks[4]++;clocks[5]++;			
		}
		for(;b>0;b--)	{
			clocks[1]++;clocks[2]++;clocks[3]++;
		}
		for(;c>0;c--)	{
			clocks[2]++;clocks[3]++;clocks[5]++;clocks[6]++;			
		}
		for(;d>0;d--)	{
			clocks[1]++;clocks[4]++;clocks[7]++;			
		}
		for(;e>0;e--)	{
			clocks[2]++;clocks[4]++;clocks[5]++;clocks[6]++;clocks[8]++;			
		}
		for(;f>0;f--)	{
			clocks[3]++;clocks[6]++;clocks[9]++;			
		}
		for(;g>0;g--)	{
			clocks[4]++;clocks[5]++;clocks[7]++;clocks[8]++;
		}
		for(;h>0;h--)	{
			clocks[7]++;clocks[8]++;clocks[9]++;			
		}
		for(;i>0;i--)	{
			clocks[5]++;clocks[6]++;clocks[8]++;clocks[9]++;			
		}
		return 1;
}
int main() {
	
	int j,k,l,m=0,min=40;
	int judge=0;
	int a,b,c,d,e,f,g,h,i;
	int remember[10];
	int p[10];
		
	for(i=1;i<=9;i++)	
		scanf("%d",&clocks[i]);
//	for(i=1;i<=9;i++)
//		printf("%d ",clocks[i]);

	for(i=1;i<=9;i++)
		remember[i]=clocks[i];	
	
	//work(0,0,0,1,1,0,0,1,1);
	//	for(i=1;i<=9;i++)
	//	printf("%d ",clocks[i]);
//	j=guess();
//	printf("j= %d",j);
		for(a=0;a<4;a++)
			for(b=0;b<4;b++)	
				for(c=0;c<4;c++)
					for(d=0;d<4;d++)
						for(e=0;e<4;e++)
							for(f=0;f<4;f++)
								for(g=0;g<4;g++)
									for(h=0;h<4;h++)
										for(i=0;i<4;i++) {
												k=a+b+c+d+e+f+g+h+i;
										//	printf("%d--",k);
												
												work(a,b,c,d,e,f,g,h,i);
												judge=guess();
												if(judge==1&&k<min){
													min=k; //printf("--%d--",min);m++;
													p[1]=a;p[2]=b;p[3]=c;p[4]=d;p[5]=e;p[6]=f;p[7]=g;p[8]=h;p[9]=i;
													
												}
												judge=0;
												for(l=1;l<=9;l++)
													clocks[l]=remember[l];
									}
																																	
										
	//	printf("(%d)",m);
	
//	printf("--%d--\n",j-1);
	    	for(;p[1]>=1;p[1]--)
				printf("1 ");
			for(;p[2]>=1;p[2]--)
				printf("2 ");
			for(;p[3]>=1;p[3]--)
				printf("3 ");
			for(;p[4]>=1;p[4]--)
				printf("4 ");
			for(;p[5]>=1;p[5]--)
				printf("5 ");
			for(;p[6]>=1;p[6]--)
				printf("6 ");
			for(;p[7]>=1;p[7]--)
				printf("7 ");
			for(;p[8]>=1;p[8]--)
				printf("8 ");
			for(;p[9]>=1;p[9]--)
				printf("9 ");
		
			
	
	
	
}
