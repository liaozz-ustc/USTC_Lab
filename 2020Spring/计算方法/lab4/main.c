#include <stdio.h>
#include <math.h>

double error(double *X0, double *X1, int k) {
    int i;
    double max, tmp;
    max = 0;
    for (i = 1; i <= k; i++) {
        tmp = fabs(X0[i] - X1[i]);
        if (tmp > max)
            max = tmp;
    }
    return max;
}

int exchange(double *X0, double *X1, int k) {
    int i;
    for (i = 1; i <= k; i++) {
        X0[i] = X1[i];
    }
    return 1;
}

int main() {
    int k, i, j;
    int num = 0;
    printf("请输入矩阵阶数:");
    //scanf("%d",&k);
    k = 10;
    printf("k=%d\n", k);
    double A[100][100];
    double B[100], X0[100], X1[100], x0[100], x1[100];
    printf("请输入矩阵A:");
    for (i = 1; i <= k; i++) {
        B[i] = 0;
        X0[i] = 0;
        X1[i] = 0;
        for (j = 1; j <= k; j++)
            A[i][j] = 0;
    }
    /*for(i=1;i<=k;i++){
        for(j=1;j<=k;j++)
            scanf("%lf",&A[i][j]);
    }*/
    for (i = 1; i <= k; i++) {
        for (j = 1; j <= k; j++) {
            if (i == j) {
                A[i][j] = 2;
            } else if (j == (i + 1) || j == (i - 1)) {
                A[i][j] = -1;
            } else {
                A[i][j] = 0;
            }
        }
    }
    for (i = 1; i <= k; i++) {
        printf("\n");
        for (j = 1; j <= k; j++) {
            printf("%lf ", A[i][j]);
        }
    }
    printf("\n请输入矩阵B:\n");
    /*for(i=1;i<=k;i++)
        scanf("%lf",&B[i]);*/
    for (i = 1; i <= k; i++)
        B[i] = 2;
    for (i = 1; i <= k; i++)
        printf("%lf ", B[i]);
    printf("\n");
    //printf("请输入初始迭代X:\n");
    for (i = 1; i <= k; i++) {
        //scanf("%lf", &X0[i]);
        X0[i] = 0;
    }
    for (i = 1; i <= k; i++)
        x0[i] = x1[i] = X1[i] = X0[i];

//    for (i = 1; i <= k; i++)
//        printf("x%d\t\t\t", i);
//    printf("\n");
//    printf("%d\t\t", num);
//    for (i = 1; i <= k; i++)
//        printf("%.12e\t", X0[i]);
    do {
        exchange(X0, X1, k);
        for (i = 1; i <= k; i++) {
            X1[i] = 0;
            for (j = 1; j <= k; j++) {
                if (j != i) {
                    X1[i] += A[i][j] * X0[j];
                }
            }
            X1[i] -= B[i];
            X1[i] = -X1[i] / A[i][i];
        }
        num++;

    } while (error(X0, X1, k) > pow(10, -5));
    printf("Jacobi迭代\n");
    printf("迭代次数:");
    printf("%d\n", num);
    for(i=1;i<=k/2;i++)
        printf("x%d\t\t\t",i);
    printf("\n");
    for (i = 1; i <= k/2; i++)
        printf("%.12e\t", X1[i]);
    printf("\n");
    for(i=(k/2)+1;i<=k;i++)
        printf("x%d\t\t\t",i);
    printf("\n");
    for (i=(k/2)+1; i <= k; i++)
        printf("%.12e\t", X1[i]);
    printf("\n");



    num = 0;
    exchange(X0, x0, k);
    exchange(X1, x1, k);
/*    printf("\nGauss-Seidel迭代\n");
    printf("迭代次数\t\t");
    for (i = 1; i <= k; i++)
        printf("x%d\t\t\t", i);
    printf("\n");
    printf("%d\t\t", num);
    for (i = 1; i <= k; i++)
        printf("%.12e\t", X0[i]);*/
    do {
        exchange(X0, X1, k);
        for (i = 1; i <= k; i++) {
            X1[i] = 0;
            for (j = 1; j < i; j++) {
                X1[i] += A[i][j] * X1[j];
            }
            for (j = i + 1; j <= k; j++) {
                X1[i] += A[i][j] * X0[j];
            }
            X1[i] -= B[i];
            X1[i] = -X1[i] / A[i][i];
        }
        num++;

    } while (error(X0, X1, k) > pow(10, -5));
    printf("\nGauss-Seidel迭代\n");
    printf("迭代次数:");
    printf("%d\n", num);
    for(i=1;i<=k/2;i++)
        printf("x%d\t\t\t",i);
    printf("\n");
    for (i = 1; i <= k/2; i++)
        printf("%.12e\t", X1[i]);
    printf("\n");
    for(i=(k/2)+1;i<=k;i++)
        printf("x%d\t\t\t",i);
    printf("\n");
    for (i=(k/2)+1; i <= k; i++)
        printf("%.12e\t", X1[i]);
    printf("\n");
    return 0;
}

