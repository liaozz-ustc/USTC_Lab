#include <stdio.h>
#include <math.h>

int main() {
    double I;
    I = -cos(8) + cos(0);
    printf("I=%.12e\n", I);
    int N;
    double h;
    double result[11];
    double e[11];
    for (int i = 0; i < 11; i++) {
        result[i] = 0;
        e[i] = 0;
    }
    for (int i = 0; i < 11; i++) {
        N = (int) pow(2, i);
        h = (double) 8 / N;
        //printf("%d,%f\n",N,h);
        for (int j = 0; j <= N; j++) {
            result[i] += sin(j * h);
        }
        result[i] -= sin(0) * 0.5;
        result[i] -= sin(8) * 0.5;
        result[i] = result[i] * h;
        e[i] = fabs(result[i] - I);
    }
    printf("复化梯形积分，数值积分、误差和误差阶为\n");
    for (int k = 0; k < 11; k++) {
        if (k == 0) {
            printf("k=%d,T(f)=%.12e,e%d=%.12e\n", k, result[k], k, e[k]);
        } else {
            printf("k=%d, T(f)=%.12e, e%d=%.12e, d%d=%f\n", k, result[k], k, e[k], k,
                   (-log(e[k] / e[k - 1]) / log(2)));
        }
    }



    for (int i = 0; i < 11; i++) {
        result[i] = 0;
        e[i] = 0;
    }
    for (int i = 1; i < 11; i++) {
        N = (int) pow(2, i);
        h = (double) 8 / N;
        //printf("%d,%f\n",N,h);
        for (int j = 0; j <= N; j++) {
            if(j==0){
                result[i] += sin(j * h);
            }else if(j==N){
                result[i] += sin(j * h);
            }else if(j%2==1){
                result[i] += 4*sin(j * h);
            }else{
                result[i] += 2*sin(j * h);
            }

        }
        result[i] = result[i] * h/3;
        e[i] = fabs(result[i] - I);
    }
    printf("复化Simpson积分，数值积分、误差和误差阶为\n");
    for (int k = 1; k < 11; k++) {
        if (k == 1) {
            printf("k=%d,T(f)=%.12e,e%d=%.12e\n", k, result[k], k, e[k]);
        } else {
            printf("k=%d, T(f)=%.12e, e%d=%.12e, d%d=%f\n", k, result[k], k, e[k], k,
                   (-log(e[k] / e[k - 1]) / log(2)));
        }
    }


    return 0;
}