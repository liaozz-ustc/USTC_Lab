#include <bits/stdc++.h>

using namespace std;

char outFileName[25];
int num[9][9];//棋盘
int nodes = 0;
int degree[9][9];//每个格子的度

void setCell(int x, int y, int number) {
    //将num[x][y]赋为number，并调整degree,degree会增加
    num[x][y] = number;
    int i, j;
    int cellX = (x / 3) * 3, cellY = (y / 3) * 3;//粗线宫左上角格点坐标

    for (i = 0; i < 9; i++) {
        //对degree进行调整
        degree[x][i]++;
        degree[i][y]++;
    }

    if (x == y) {
        for (i = 0; i < 9; i++) {
            degree[i][i]++;
        }
    }
    if (x + y == 8) {
        for (i = 0; i < 9; i++) {
            degree[i][8 - i]++;
        }
    }
    if (x != 4 && y != 4) {
        for (i = cellX; i < cellX + 3; i++) {
            for (j = cellY; j < cellY + 3; j++) {
                if (x == y) {
                    if (i != x && j != y && i != j) {
                        degree[i][j]++;
                    }
                } else if (x + y == 8) {
                    if (i != x && j != y && (i + j) != 8) {
                        degree[i][j]++;
                    }
                } else {
                    if (i != x && j != y) {
                        degree[i][j]++;
                    }
                }
            }
        }
    }
    degree[x][y] = -1;
}


pair<int, int> select() {
    //选择degree最大的格子
    int row = -1, col = -1, max = -1;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (num[i][j] == 0 && degree[i][j] > max) {
                row = i;
                col = j;
                max = degree[i][j];
            }
        }
    }
    return make_pair(row, col);
}

bool check(int x, int y) {//对当前状态进行检查
    int i, j;
    int cellX = (x / 3) * 3, cellY = (y / 3) * 3;//粗线宫左上角格点坐标
    for (i = 0; i < 9; i++) {
        if (num[x][i] == num[x][y] && i != y) {
            return false;
        }
    }
    for (i = 0; i < 9; i++) {
        if (num[i][y] == num[x][y] && i != x) {
            return false;
        }
    }
    if (x == y) {
        for (i = 0, j = 0; i < 9; i++, j++) {
            if (num[x][y] == num[i][j] && i != x) {
                return false;
            }
        }
    }
    if (x + y == 8) {
        for (i = 0; i < 9; i++) {
            if (num[x][y] == num[i][8 - i] && x != i) {
                return false;
            }
        }
    }
    for (i = cellX; i < cellX + 3; i++) {
        for (j = cellY; j < cellY + 3; j++) {
            if (x != i || y != j) {
                if (num[i][j] == num[x][y]) {
                    return false;
                }
            }
        }
    }
    return true;
}

int backtrack() {
    //回溯搜索法，搜索到树的第t层，由第t层向第t+1层扩展
    pair<int, int> cell = select();
    if (cell.first == -1 && cell.second == -1) {
        FILE *fp;
        if ((fp = fopen(outFileName, "w")) == NULL) {
            printf("ERROR:can't create file!");
            exit(0);
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                fprintf(fp, "%d ", num[i][j]);
            }
            fprintf(fp, "\n");
        }
        printf("  %d  \n", nodes);
        return 1;
    }

    int tmp[9][9];
    memcpy(tmp, degree, sizeof(degree));

    for (int i = 1; i < 10; i++) {
        nodes++;
        num[cell.first][cell.second] = i;

        if (check(cell.first, cell.second)) {

            setCell(cell.first, cell.second, i);
            if (backtrack())
                return 1;
            memcpy(degree, tmp, sizeof(degree));
        }
        num[cell.first][cell.second] = 0;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    if (argc == 2) {


        FILE *fp;
        int i, j;
        if ((fp = fopen(argv[1], "r")) == NULL) {
            printf("ERROR:can't find %s", argv[1]);
            exit(0);
        }
        strcat(outFileName, strrchr(argv[1], '\\') + 1);

        clock_t timerBegin = clock();

        for (i = 0; i < 9; i++) {
            for (j = 0; j < 9; j++) {
                degree[i][j] = 0;
            }
        }

        for (i = 0; i < 9; i++) {
            for (j = 0; j < 9; j++) {
                fscanf(fp, "%d", &num[i][j]);
                if (num[i][j])
                    setCell(i, j, num[i][j]);
            }
        }


        backtrack();

        clock_t timerEnd = clock();
        double_t time = (double) (timerEnd - timerBegin) / CLOCKS_PER_SEC;
        printf("time:%fs,nodes:%d\n", time, nodes);
    }
    return 0;
}