#include <stdio.h>
#include <math.h>
int main() {
    float f=0,g=0,x=0;
    printf("\tx\t\t\t(x^2+9)^0.5-3\t\t(x^2)/((x^2+9)^0.5+3)\n");
    for(int i=1;i<=10;i++){
        x=pow(8,-i);
        f=sqrt(pow(x,2)+9)-3;
        g=pow(x,2)/(sqrt(pow(x,2)+9)+3);
        printf("\t%.12e\t%0.12e\t%0.12e\n",x,f,g);
    }
    return 0;
}