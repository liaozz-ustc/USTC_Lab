#include<stdio.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<string.h>

int main(){
	printf("Give me a string:\n");
	char str[100];
	int str_len;
	int ret;
	scanf("%s",str);
	str_len = strlen(str);
	printf("GET:%s---length:%d\n",str,str_len);
	
	
	printf("In sys_print_val:\n");
	syscall(328,str,str_len,&ret);
	syscall(327,ret);
	return 1;
}
	
	
