#include<stdio.h>
#include<string.h>

int board[15][15];			//棋盘 
int count,n,m;
void search(int x,int y,int flag);
int work[8][2]={{1,-2},{-1,-2},{1,2},{-1,2},{2,-1},{2,1},{-2,-1},{-2,1}};

int main() {
	int t;
	int x,y;
	int i,j;
	scanf("%d",&t);
	
	while(t)	{
		memset(board,0,sizeof(board));			//将棋盘都赋值为0 没走过的为0 
		scanf("%d%d%d%d",&n,&m,&x,&y);
		
		board[x][y]=1;								//初始点为1 走过的代表1 
	//	for(i=0;i<m;i++)
	//		for(j=0;j<n;j++)
	//			printf("%d\n",board[i][j]);
		
		 count=0;
		if(n==0||m==0){
			count=0;
		//	printf("n==0;m==0\n");
			}
		else 
			search(x,y,1);
		
		printf("%d\n",count);	
		t--;	
	}
	
}


 
void search(int x,int y,int flag) {
	//	printf("#"); 
		int k,l;
		if(flag==n*m)	{ 
					count++;
					
					return ; }
					 // printf("x=%dy=%d",x,y);
		for(k=0;k<8;k++)  {
			x=x+work[k][0];
			y=y+work[k][1];
		//	printf("#");printf("x=%d y=%d b=%d\n",x,y,board[x][y]);
			if(x>=0&&y>=0&&x<n&&y<m&&board[x][y]==0)	{
				board[x][y]=1;
		       // printf("--flag=%d--",flag);
				search(x,y,flag+1);
				board[x][y]=0;
				 //返回 
			}
			
			x=x-work[k][0];
			y=y-work[k][1];					//返回上一步		
		}
				
}




