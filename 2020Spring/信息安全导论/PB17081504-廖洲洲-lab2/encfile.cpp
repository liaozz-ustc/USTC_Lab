// cryptoDemo.cpp : Defines the entry point for the console application.
// Windows: cl cryptoDemo.cpp
// Linux: gcc -o cryptoDemo cryptoDemo.cpp -lcrypto

#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "/usr/include/openssl/aes.h"

#pragma comment(lib,"libeay32.lib")


void EncFile(char *FileName,char *passwd,int pwdLen){
    int i,len,ReadLen;
    char OutFileName[1024];

    unsigned char buf[16];
    unsigned char buf2[16];
    unsigned char buf3[16]="";
    unsigned char aes_keybuf[32];
    AES_KEY aeskey;

    
    memset(aes_keybuf,0x90,32);
    if(pwdLen<32){ len=pwdLen; } else { len=32;}
    for(i=0;i<len;i++) aes_keybuf[i]=passwd[i];
   


    AES_set_encrypt_key(aes_keybuf,256,&aeskey);
    FILE *fp_in,*fp_out,*fp_test;
    if((fp_in=fopen(FileName,"rb"))==NULL){
        printf("ERROR:Can't open this file!");
        exit(0);
    }

    strcpy(OutFileName,FileName);
    strcat(OutFileName,".enc");
    if((fp_out=fopen(OutFileName,"wb"))==NULL){
        printf("ERROR:Can't open this file!");
        exit(0);
    }
/* if((fp_test=fopen("file_test.htm","wb"))==NULL){
        printf("ERROR:Can't open this file!");
        exit(0);
    }*/

    while (!feof(fp_in)){
        memset(buf,0,16);
        ReadLen=fread(buf, sizeof(unsigned char),16,fp_in);
//	fwrite(buf,sizeof(unsigned char ),ReadLen,fp_test);
//	if(ReadLen<16)
//		printf("ReadLen=%d\n",ReadLen);
        AES_encrypt(buf,buf2,&aeskey);
        fwrite(buf2, sizeof(unsigned char),16,fp_out);
    }
	buf2[0]=(unsigned char)ReadLen;
//	printf("@@@%d@@@\n",(int)buf2[0]);
	fwrite(buf2,sizeof(unsigned char),1,fp_out);
    fclose(fp_in);fclose(fp_out);

}
void DecFile(char *FileName,char *passwd,int pwdLen){
    int i,j,len,ReadLen,EndLen;
    char OutFileName[1024]="dec-";
    char NameBuffer[1024]="";

    unsigned char buf[16];
    unsigned char buf2[16];
    unsigned char buf3[16];
    unsigned char aes_keybuf[32];
    AES_KEY aeskey;

    memset(aes_keybuf,0x90,32);
    if(pwdLen<32){ len=pwdLen; } else { len=32;}
    for(i=0;i<len;i++) aes_keybuf[i]=passwd[i];

    AES_set_decrypt_key(aes_keybuf,256,&aeskey);
    FILE *fp_in,*fp_out,*fp_test;
    if((fp_in=fopen(FileName,"rb"))==NULL){
        printf("ERROR:Can't open this file!");
        exit(0);
    }
//    printf("%s\n",FileName);
    strncpy(NameBuffer,FileName,strlen(FileName)-4);
//    printf("%s\n",NameBuffer);
    strcat(OutFileName,NameBuffer);
//    printf("%s\n",OutFileName);
    if((fp_out=fopen(OutFileName,"wb"))==NULL){
        printf("ERROR:Can't open this file!");
        exit(0);
    }
   memset(buf,0,16);
   fseek(fp_in,-1L,2);
   fread(buf,sizeof(unsigned char),1,fp_in);
   EndLen=(int)buf[0];
   fseek(fp_in,0L,0);
    memset(buf,0,16);
    ReadLen=fread(buf, sizeof(unsigned char),16,fp_in);
    for(i=0;i<16;i++)
	buf3[i]=buf[i];
    while((ReadLen=fread(buf, sizeof(unsigned char),16,fp_in))!=1){
	AES_decrypt(buf3,buf2,&aeskey);
	fwrite(buf2,sizeof(unsigned char),16,fp_out);
	for(i=0;i<16;i++)
		buf3[i]=buf[i];
	memset(buf,0,16);
}
	AES_decrypt(buf3,buf2,&aeskey);
	fwrite(buf2,sizeof(unsigned char),EndLen,fp_out);
    fclose(fp_in);fclose(fp_out);


}
int main(int argc, char* argv[])
{
    if(strcmp(argv[1],"enc")==0)
        EncFile(argv[2],argv[3],strlen(argv[3]));
    else if(strcmp(argv[1],"dec")==0)
        DecFile(argv[2],argv[3],strlen(argv[3]));
    else{
        printf("ERROR!\n");
    }
    return 0;
}
