#include<stdio.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(){
	char cmdline[256];
	int i,j,k;
	int cmd_num=1,cmd_len,pipeflag,pipeptr;
	char cmd[256];                          //用于存储单条命令
	char out[256];
	FILE *fp;
	while(1){
		printf("OSLab2->");
		fgets(cmdline,256,stdin);
		cmd_len=strlen(cmdline);
		cmd_num=1;                      //每次都要将命令个数置1
		for(i=0;i<cmd_len;i++)		//the number of commands
			if(cmdline[i]==';')  
				cmd_num++;
		j=0;				//j用于分割命令行
		for(i=0;i < cmd_num ;i++) {
			pipeflag=0;		//判断是否有管道
			for(k=0;cmdline[j]!=';'&&j<cmd_len;j++,k++){
				cmd[k]=cmdline[j];
				if(cmdline[j]=='|'){
					pipeflag=1;
					pipeptr=k;
					}
				}
			cmd[k]='\0'; 
			j++;
//			printf("%d:%s\n",i,cmd);
			if(pipeflag)	{ 	//有管道
				fp=popen(cmd,"r");
				while(fgets(out,256,fp)!=NULL)
					fputs(out,stdout);					
				pclose(fp);
				}
			else     //无管道
				system(cmd);
					
		}
	}
}	
