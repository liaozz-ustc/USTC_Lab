/* stack.c */

/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ATTACK_LEN 1024*1024*2
int bof(char *str)
{
    char buffer[24];

    /* The following statement has a buffer overflow problem */ 
    strcpy(buffer, str);

    return 1;
}

int main(int argc, char **argv)
{
    char str[ATTACK_LEN+1];
    FILE *badfile;

    badfile = fopen("lbadfile", "r");
    fread(str, sizeof(char), ATTACK_LEN, badfile);
	str[ATTACK_LEN]=0;
    bof(str);

    printf("Returned Properly\n");
    return 1;
}
