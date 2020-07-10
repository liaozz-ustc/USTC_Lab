//
// Created by 廖洲洲 on 2020/5/18.
//

#include <bits/stdc++.h>

using namespace std;

#define dimension 5
#define ZERO 26

#define GET_CELL(state, x, y) (state[((x)*dimension)+(y)]) //由坐标得到值

struct Node {
    char *state;//当前节点状态,编码为字符串
    pair<char, char> action;//父节点生成该节点时采取的行动
    int pathCost;//从初始状态到达该路径的耗散
    int manhattanCost;
    unsigned char zero[2][2];//0的位置
    unsigned char sevenX, sevenY;//7的右上角位置
    struct Node *parent;
} start;

int pathLen;
char *initState = new char[dimension * dimension + 1];
char *destState = new char[dimension * dimension + 1];
int destPos[dimension * dimension][2] = {0};//目标状态每个值的坐标,第0列为x，第1列为y,7以右上角为参照点
int manhattan[dimension * dimension][dimension][dimension] = {-1};//将曼哈顿距离以表的方式存储下来，减少计算
int initManhattan = 0;
deque<Node> path;

int readFromFile(char *fileName, char *state, int isDest) {
    FILE *fp;
    if ((fp = fopen(fileName, "r")) == NULL) {
        printf("ERROR:can't open %s", fileName);
        exit(0);
    }
    int num;
    int Order7 = 0;
    int Order0 = 0;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (j == dimension - 1) {
                fscanf(fp, "%d", &num);
            } else {
                fscanf(fp, "%d,", &num);
            }
            if (num)
                state[i * dimension + j] = char(num);
            else
                state[i * dimension + j] = char(ZERO);
            if (isDest) { //目标文件
                if (num == 7) {
                    Order7++;
                    if (Order7 == 2) {
                        destPos[num][0] = i;
                        destPos[num][1] = j;//记录下x，y坐标
                    }
                } else {
                    destPos[num][0] = i;
                    destPos[num][1] = j;//记录下x，y坐标
                }
            } else {//源文件
                if (num == 0) {//保存0的坐标
                    Order0++;
                    if (Order0 == 1) {
                        start.zero[0][0] = i;
                        start.zero[0][1] = j;
                    } else if (Order0 == 2) {
                        start.zero[1][0] = i;
                        start.zero[1][1] = j;
                    }
                } else if (num == 7) {//保存7的坐标
                    Order7++;
                    if (Order7 == 2) {
                        start.sevenX = i;
                        start.sevenY = j;
                    }
                }
            }
        }
    }
    return 1;
}


int input(char *inputFile, char *destFile) {
    readFromFile(inputFile, initState, 0);
    readFromFile(destFile, destState, 1);
    initState[dimension * dimension] = '\0';
    destState[dimension * dimension] = '\0';
    return 1;
}


int computeManhattan() {
    for (int i = 0; i < dimension * dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            for (int k = 0; k < dimension; k++) {
                if (i > 0 && i != 7) {
                    manhattan[i][j][k] = abs(j - destPos[i][0]) + abs(k - destPos[i][1]);
                } else if (i == 7 && j <= 3 && k >= 1) {
                    manhattan[i][j][k] = abs(j - destPos[i][0]) + abs(k - destPos[i][1]);
                } else {
                    manhattan[i][j][k] = 0;
                }
            }
        }
    }
    return 1;
}


int hFunc(char *state) {//计算h
    int h = 0;
    int order7 = 0;
    int num;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            num = (int) GET_CELL(state, i, j);
            if (num == 7) {
                order7++;
                if (order7 == 2) {
                    h += manhattan[num][i][j];
                }
            } else if (num != ZERO) {
                h += manhattan[num][i][j];
            }
        }
    }
    return h;
}

bool isEqual(char *str1, char *str2) {
    for (int i = 0; i < dimension * dimension + 1; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

vector<pair<int, Node>> getChildNode(Node *node, int parentH) {
    //返回给定节点的不在closedSet中的所有子节点
    vector<pair<int, Node>> result;
    int tmp;
    int childH;
    int x, y;
    for (int i = 0; i < 2; i++) {
        //处理可能的字节点，7的移动单独考虑
        x = (int) node->zero[i][0];
        y = (int) node->zero[i][1];
        //printf("why:%d\n",GET_CELL(node->state, x - 1, y));
        if (x >= 1 && (tmp = ((int) GET_CELL(node->state, x - 1, y))) != ZERO && tmp != 7) {
            //空格上面的格子可以下移
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0] = node->zero[0][0];
            child.zero[0][1] = node->zero[0][1];
            child.zero[1][0] = node->zero[1][0];
            child.zero[1][1] = node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x - 1, y) = ZERO;
            if (!isEqual(child.state, node->state)) {
                child.action = make_pair(char(tmp), 'd');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x - 1;
                child.zero[i][1] = y;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x - 1][y] + manhattan[tmp][x][y];
                child.manhattanCost = childH;
                result.emplace_back(childH, child);
            }
        }
        if (x <= dimension - 2 && (tmp = ((int) GET_CELL(node->state, x + 1, y))) != ZERO && tmp != 7) {
            //空格下面的格子可以上移
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0] = node->zero[0][0];
            child.zero[0][1] = node->zero[0][1];
            child.zero[1][0] = node->zero[1][0];
            child.zero[1][1] = node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x + 1, y) = ZERO;
            if (!isEqual(child.state, node->state)) {
                child.action = make_pair(char(tmp), 'u');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x + 1;
                child.zero[i][1] = y;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x + 1][y] + manhattan[tmp][x][y];
                child.manhattanCost = childH;
                result.emplace_back(childH, child);
            }
        }
        if (y >= 1 && (tmp = ((int) GET_CELL(node->state, x, y - 1))) != ZERO && tmp != 7) {
            //空格左面的格子可以右移
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0] = node->zero[0][0];
            child.zero[0][1] = node->zero[0][1];
            child.zero[1][0] = node->zero[1][0];
            child.zero[1][1] = node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x, y - 1) = ZERO;
            if (!isEqual(child.state, node->state)) {
                child.action = make_pair(char(tmp), 'r');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x;
                child.zero[i][1] = y - 1;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x][y - 1] + manhattan[tmp][x][y];
                child.manhattanCost = childH;
                result.emplace_back(childH, child);
            }
        }
        if (y <= dimension - 2 && (tmp = ((int) GET_CELL(node->state, x, y + 1))) != ZERO && tmp != 7) {
            //空格右面的格子可以左移
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0] = node->zero[0][0];
            child.zero[0][1] = node->zero[0][1];
            child.zero[1][0] = node->zero[1][0];
            child.zero[1][1] = node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x, y + 1) = ZERO;
            if (!isEqual(child.state, node->state)) {
                child.action = make_pair(char(tmp), 'l');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x;
                child.zero[i][1] = y + 1;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x][y + 1] + manhattan[tmp][x][y];
                child.manhattanCost = childH;
                result.emplace_back(childH, child);
            }
        }
    }

    //下面处理7的移动
    x = node->sevenX;
    y = node->sevenY;
    if (x <= dimension - 3 && y >= 1 &&
        (int) GET_CELL(node->state, x + 1, y - 1) == ZERO && (int) GET_CELL(node->state, x + 2, y) == ZERO) {
        //7可以下移
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x + 1, y - 1) = 7;
        GET_CELL(child.state, x + 2, y) = 7;
        GET_CELL(child.state, x, y - 1) = ZERO;
        GET_CELL(child.state, x, y) = ZERO;
        if (!isEqual(child.state, node->state)) {
            child.action = make_pair(char(7), 'd');
            //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
            child.pathCost = node->pathCost + 1;
            child.zero[0][0] = x;
            child.zero[0][1] = y - 1;
            child.zero[1][0] = x;
            child.zero[1][1] = y;
            child.sevenX = x + 1;
            child.sevenY = y;
            child.parent = node;
            childH = parentH - manhattan[7][x][y] + manhattan[7][x + 1][y];
            child.manhattanCost = childH;
            result.emplace_back(childH, child);
        }
    }
    if (x >= 1 && y >= 1 &&
        (int) GET_CELL(node->state, x - 1, y - 1) == ZERO && (int) GET_CELL(node->state, x - 1, y) == ZERO) {
        //7可以上移
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x - 1, y - 1) = 7;
        GET_CELL(child.state, x - 1, y) = 7;
        GET_CELL(child.state, x, y - 1) = ZERO;
        GET_CELL(child.state, x + 1, y) = ZERO;
        if (!isEqual(child.state, node->state)) {
            child.action = make_pair(char(7), 'u');
            //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
            child.pathCost = node->pathCost + 1;
            child.zero[0][0] = x;
            child.zero[0][1] = y - 1;
            child.zero[1][0] = x + 1;
            child.zero[1][1] = y;
            child.sevenX = x - 1;
            child.sevenY = y;
            child.parent = node;
            childH = parentH - manhattan[7][x][y] + manhattan[7][x - 1][y];
            child.manhattanCost = childH;
            result.emplace_back(childH, child);
        }
    }
    if (y >= 2 && (int) GET_CELL(node->state, x, y - 2) == ZERO
        && (int) GET_CELL(node->state, x + 1, y - 1) == ZERO) {
        //7可以左移
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x, y - 2) = 7;
        GET_CELL(child.state, x + 1, y - 1) = 7;
        GET_CELL(child.state, x, y) = ZERO;
        GET_CELL(child.state, x + 1, y) = ZERO;
        if (!isEqual(child.state, node->state)) {
            child.action = make_pair(char(7), 'l');
            //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
            child.pathCost = node->pathCost + 1;
            child.zero[0][0] = x;
            child.zero[0][1] = y;
            child.zero[1][0] = x + 1;
            child.zero[1][1] = y;
            child.sevenX = x;
            child.sevenY = y - 1;
            child.parent = node;
            childH = parentH - manhattan[7][x][y] + manhattan[7][x][y - 1];
            child.manhattanCost = childH;
            result.emplace_back(childH, child);
        }
    }
    if (y <= dimension - 2 && (int) GET_CELL(node->state, x, y + 1) == ZERO
        && (int) GET_CELL(node->state, x + 1, y + 1) == ZERO) {
        //7可以右移
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x, y + 1) = 7;
        GET_CELL(child.state, x + 1, y + 1) = 7;
        GET_CELL(child.state, x, y - 1) = ZERO;
        GET_CELL(child.state, x + 1, y) = ZERO;
        if (!isEqual(child.state, node->state)) {
            child.action = make_pair(char(7), 'r');
            //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
            child.pathCost = node->pathCost + 1;
            child.zero[0][0] = x;
            child.zero[0][1] = y - 1;
            child.zero[1][0] = x + 1;
            child.zero[1][1] = y;
            child.sevenX = x;
            child.sevenY = y + 1;
            child.parent = node;
            childH = parentH - manhattan[7][x][y] + manhattan[7][x][y + 1];
            child.manhattanCost = childH;
            result.emplace_back(childH, child);
        }
    }
    return result;
}


void print(char *state){
    for(int i=0;i<dimension;i++){
        for(int j=0;j<dimension;j++){
            printf("%d ",(int)GET_CELL(state,i,j));
        }
        printf("\n");
    }
}


void printAction(FILE *fp) {
    path.pop_front();
    while (!path.empty()) {
        auto result = path.front();
        path.pop_front();
        pathLen++;
        fprintf(fp, "(%d,%c);", (int) result.action.first, result.action.second);
    }
}

int dfs(int g, int bound) {
    auto node = path.back();
    //print(node.state);
    int f = node.pathCost + node.manhattanCost;
    if (f > bound)
        return f;
    if (node.manhattanCost == 0)
        return -1;
    int next_bound = INT32_MAX;
    auto childNodes = getChildNode(&node, node.manhattanCost);
    int t;
    for (auto &s:childNodes) {
        path.push_back(s.second);
        t = dfs(g + 1, bound);
        if (t < 0)
            return -1;
        if (t < next_bound)
            next_bound = t;
        path.pop_back();
    }
    return next_bound;
}


int main(int argc, char *argv[]) {
    if (argc == 3) {
        input(argv[1], argv[2]);

        clock_t timerBegin = clock();

        computeManhattan();

        initManhattan = hFunc(initState);
        //print(initState);
        start.state = initState;
        start.pathCost = 0;
        start.manhattanCost = initManhattan;
        start.parent = NULL;//根节点的父亲为空
        int bound = start.manhattanCost;
        path.push_back(start);

        while (true) {
            int t = dfs(0, bound);
            if (t < 0) {
                break;
            }
            if (t > INT32_MAX - 1) {
                return 0;
            }
            bound = t;
        }


        clock_t timerEnd = clock();

        FILE *fp;
        char outFileName[25];
        strcat(outFileName, strrchr(argv[1], '\\') + 1);
        if ((fp = fopen(outFileName, "w")) == NULL) {
            printf("ERROR:can't create file!");
            exit(0);
        }
        printAction(fp);


        double_t time = (double) (timerEnd - timerBegin) / CLOCKS_PER_SEC;
        printf("\npath:%d",pathLen);
        printf("\ntime:%fs\n", time);
    }
    return 0;
}