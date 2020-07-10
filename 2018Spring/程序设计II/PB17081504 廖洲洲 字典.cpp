#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Swords *Createditionary(void);
struct Sword *Createword(void);
int Search(struct Swords *ps,struct Sword *p);
int Igetline(char *s);

struct Swords {
	char ceng[13];
	char cfor[13];
	struct 	Swords *next;
};                 
struct Sword {
	char cfor[13];
	struct Sword *next;
};                                                 																	  

main() {
	struct Swords *ps;
	struct Sword *p;
	ps=Createditionary();
	p=Createword();
//	while(ps!=NULL){	
	//	printf("%s %s",ps->ceng,ps->cfor);
//		ps=ps->next;
//	}
//		while(p!=NULL){
//		printf("%s",p->cfor);
//		p=p->next;
//	}		
	while(p!=NULL) {
		Search(ps,p);
		p=p->next;
	}
	
}	

struct Swords *Createditionary(void)	{					
	struct Swords *head;
	struct Swords *rear;
	struct Swords *p;
	            
	head=NULL;
	rear=NULL;
	
	while(1) {
		p=(struct Swords*)malloc(sizeof(struct Swords));
		Igetline(p->ceng);  // printf("ditionary----%s\n",p->ceng);
		 
		if(strlen(p->ceng)==0)
			break;
			
		Igetline(p->cfor);  ///printf("ditionary----%s\n",p->cfor);
		
		if(head==NULL)
			head=p;
		else	
			rear->next=p;
		rear=p;
	}  
	if(rear!=NULL)
		rear->next=NULL;
	return head;
}	

struct Sword *Createword(void) {
	struct Sword *head;
	struct Sword *rear;
	struct Sword *p;
             
	head=NULL;
	rear=NULL;
	
	while(1) {
		p=(struct Sword*)malloc(sizeof(struct Sword));
		Igetline(p->cfor); //   printf("word--%s\n",p->cfor);
	//	scanf("%s",p->cfor);
		if(strlen(p->cfor)==0)
			break;
			
		if(head==NULL)
			head=p;
		else	
			rear->next=p;
		rear=p;
	}
	if(rear!=NULL)
		rear->next=NULL;
	return head;
}				

int Search(struct Swords *ps,struct Sword *p)	{
	while(ps!=NULL) {
		if(strcmp(ps->cfor,p->cfor)==0) {
			printf("%s\n",ps->ceng);
			return 0;
			}
		else
			ps=ps->next;
	}	
	printf("eh\n");
	return 0;
}				
int Igetline(char *s) {
	int i=0; char c;
	if((c=getchar())==EOF)
		*(s+i)='\0'; 
	else
	{			if(c!='\n'&&c!=' ')
						{
					*(s+(i++))=c;
		//			printf("read----%c\n",c);
					}	
				else{
						*(s+i)='\0';
						return 1;
					
				}
	}
	
	
	while((c=getchar())!='\n'&&c!=' '&&c!=EOF)
	{
		*(s+(i++))=c;
	//	printf("read----%c\n",c);
	}
		
	if(c==EOF||c=='\n'||c==' ')
		*(s+i)='\0';
	return 1;
		
}			     
