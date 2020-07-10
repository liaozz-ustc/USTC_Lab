#include <bits/stdc++.h>

using namespace std;

#define dimension 5
#define ZERO 26

#define GET_CELL(state, x, y) (state[((x)*dimension)+(y)]) //?????????


struct Node {
    char *state;//????????,??????????
    pair<char, char> action;//???????????????????��?
    int pathCost;//???????????��??????
    //unsigned char x1, y1, x2, y2;//0??��??
    unsigned char zero[2][2];//0??��??
    unsigned char sevenX, sevenY;//7???????��??
    struct Node *parent;
} start;


char *initState = new char[dimension * dimension + 1];
char *destState = new char[dimension * dimension + 1];
int destPos[dimension * dimension][2] = {0};//???????????????,??0???x????1???y,7?????????????
unordered_set<string> closedSet;
priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<>> openList;
int manhattan[dimension * dimension][dimension][dimension] = {-1};//??????????????????��?????????????
int initManhattan = 0;

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
            if(num)
                state[i * dimension + j] = char(num);
            else
                state[i * dimension + j] = char(ZERO);
            if (isDest) { //??????
                if (num == 7) {
                    Order7++;
                    if (Order7 == 2) {
                        destPos[num][0] = i;
                        destPos[num][1] = j;//?????x??y????
                    }
                } else {
                    destPos[num][0] = i;
                    destPos[num][1] = j;//?????x??y????
                }
            } else {//????
                if (num == 0) {//????0??????
                    Order0++;
                    if (Order0 == 1) {
                        start.zero[0][0] = i;
                        start.zero[0][1] = j;
                    } else if (Order0 == 2) {
                        start.zero[1][0] = i;
                        start.zero[1][1] = j;
                    }
                } else if (num == 7) {//????7??????
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


int hFunc(char *state) {//????h
    int h = 0;
    int order7 = 0;
    int num;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            num = (int) GET_CELL(state, i, j);
            //printf("%d:%d\n", num, manhattan[num][i][j]);
            if (num == 7) {
                order7++;
                if (order7 == 2) {
                    h += manhattan[num][i][j];
                }
            } else if(num!=ZERO){
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
    //??????????????closedSet?��?????????
    vector<pair<int, Node>> result;
    int tmp;
    int childH;
    int x, y;
  /*  for(int i=0;i<dimension;i++){
        for(int j=0;j<dimension;j++)
            printf("%d ",GET_CELL(node->state,i,j));
    } printf("\n");*/
    for (int i = 0; i < 2; i++) {
        //????????????7?????????????
        x = (int) node->zero[i][0];
        y = (int) node->zero[i][1];
        //printf("why:%d\n",GET_CELL(node->state, x - 1, y));
        if (x >= 1 && (tmp = ((int) GET_CELL(node->state, x - 1, y))) != ZERO && tmp != 7) {
            //??????????????????
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0]=node->zero[0][0];child.zero[0][1]=node->zero[0][1];
            child.zero[1][0]=node->zero[1][0];child.zero[1][1]=node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x - 1, y) = ZERO;
            if (!closedSet.count(child.state)) {
                child.action = make_pair(char(tmp), 'd');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x - 1;
                child.zero[i][1] = y;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x - 1][y] + manhattan[tmp][x][y];
                result.push_back(make_pair(childH, child));
            }
        }
        if (x <= dimension - 2 && (tmp = ((int) GET_CELL(node->state, x + 1, y))) != ZERO && tmp != 7) {
            //??????????????????
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0]=node->zero[0][0];child.zero[0][1]=node->zero[0][1];
            child.zero[1][0]=node->zero[1][0];child.zero[1][1]=node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x + 1, y) = ZERO;
            if (!closedSet.count(child.state)) {
                child.action = make_pair(char(tmp), 'u');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x + 1;
                child.zero[i][1] = y;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x + 1][y] + manhattan[tmp][x][y];
                result.push_back(make_pair(childH, child));
            }
        }
        if (y >= 1 && (tmp = ((int) GET_CELL(node->state, x, y - 1))) != ZERO && tmp != 7) {
            //??????????????????
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0]=node->zero[0][0];child.zero[0][1]=node->zero[0][1];
            child.zero[1][0]=node->zero[1][0];child.zero[1][1]=node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x, y - 1) = ZERO;
            if (!closedSet.count(child.state)) {
                child.action = make_pair(char(tmp), 'r');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x;
                child.zero[i][1] = y - 1;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x][y - 1] + manhattan[tmp][x][y];
                result.push_back(make_pair(childH, child));
            }
        }
        if (y <= dimension - 2 && (tmp = ((int) GET_CELL(node->state, x, y + 1))) != ZERO && tmp != 7) {
            //??????????????????
            Node child;
            child.state = new char[dimension * dimension + 1];
            strcpy(child.state, node->state);
            child.zero[0][0]=node->zero[0][0];child.zero[0][1]=node->zero[0][1];
            child.zero[1][0]=node->zero[1][0];child.zero[1][1]=node->zero[1][1];
            GET_CELL(child.state, x, y) = tmp;
            GET_CELL(child.state, x, y + 1) = ZERO;
            if (!closedSet.count(child.state)) {
                child.action = make_pair(char(tmp), 'l');
                //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
                child.pathCost = node->pathCost + 1;
                child.zero[i][0] = x;
                child.zero[i][1] = y + 1;
                child.sevenX = node->sevenX;
                child.sevenY = node->sevenY;
                child.parent = node;
                childH = parentH - manhattan[tmp][x][y + 1] + manhattan[tmp][x][y];
                result.push_back(make_pair(childH, child));
            }
        }
    }

    //???��??7?????
    x = node->sevenX;
    y = node->sevenY;
    if (x <= dimension - 3 && y >= 1 &&
        (int) GET_CELL(node->state, x + 1, y - 1) == ZERO && (int) GET_CELL(node->state, x + 2, y) == ZERO) {
        //7????????
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x + 1, y - 1) = 7;
        GET_CELL(child.state, x + 2, y) = 7;
        GET_CELL(child.state, x, y - 1) = ZERO;
        GET_CELL(child.state, x, y) = ZERO;
        if (!closedSet.count(child.state)) {
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
            result.push_back(make_pair(childH, child));
        }
    }
    if (x >= 1 && y >= 1 &&
        (int) GET_CELL(node->state, x - 1, y - 1) == ZERO && (int) GET_CELL(node->state, x - 1, y) == ZERO) {
        //7????????
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x - 1, y - 1) = 7;
        GET_CELL(child.state, x - 1, y) = 7;
        GET_CELL(child.state, x, y - 1) = ZERO;
        GET_CELL(child.state, x + 1, y) = ZERO;
        if (!closedSet.count(child.state)) {
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
            result.push_back(make_pair(childH, child));
        }
    }
    if (y >= 2 && (int) GET_CELL(node->state, x , y - 2) == ZERO
        && (int) GET_CELL(node->state, x + 1, y-1) == ZERO) {
        //7????????
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x , y - 2) = 7;
        GET_CELL(child.state, x +1, y-1) = 7;
        GET_CELL(child.state, x, y ) = ZERO;
        GET_CELL(child.state, x+1, y) = ZERO;
        if (!closedSet.count(child.state)) {
            child.action = make_pair(char(7), 'l');
            //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
            child.pathCost = node->pathCost + 1;
            child.zero[0][0] = x;
            child.zero[0][1] = y;
            child.zero[1][0] = x+1;
            child.zero[1][1] = y;
            child.sevenX = x;
            child.sevenY = y-1;
            child.parent = node;
            childH = parentH - manhattan[7][x][y] + manhattan[7][x][y-1];
            result.push_back(make_pair(childH, child));
        }
    }
    if (y <= dimension-2 && (int) GET_CELL(node->state, x , y +1) == ZERO
        && (int) GET_CELL(node->state, x + 1, y+1) == ZERO) {
        //7????????
        Node child;
        child.state = new char[dimension * dimension + 1];
        strcpy(child.state, node->state);
        GET_CELL(child.state, x , y +1 )= 7;
        GET_CELL(child.state, x +1, y+1) = 7;
        GET_CELL(child.state, x, y-1 ) = ZERO;
        GET_CELL(child.state, x+1, y) = ZERO;
        if (!closedSet.count(child.state)) {
            child.action = make_pair(char(7), 'r');
            //printf("childNode:(%d,%c)\n",child.action.first,child.action.second);
            child.pathCost = node->pathCost + 1;
            child.zero[0][0] = x;
            child.zero[0][1] = y-1;
            child.zero[1][0] = x+1;
            child.zero[1][1] = y;
            child.sevenX = x;
            child.sevenY = y+1;
            child.parent = node;
            childH = parentH - manhattan[7][x][y] + manhattan[7][x][y+1];
            result.push_back(make_pair(childH, child));
        }
    }
    return result;
}

Node *Astar() {
    initManhattan = hFunc(initState);
    start.state = initState;
    start.pathCost = 0;
    start.parent = NULL;//????????????
    //printf("(%d,%d),(%d,%d)\n",start.zero[0][0],start.zero[0][1],start.zero[1][0],start.zero[1][1]);
    openList.push(make_pair(initManhattan + start.pathCost, &start));
    while (!openList.empty()) {
        auto node = openList.top();
        openList.pop();
        //printf("(%d,%d),(%d,%d)",node.second->zero[0][0],node.second->zero[0][1],node.second->zero[1][0],node.second->zero[1][1]);
        if (isEqual(node.second->state, destState)) {
            return node.second;
        }
        closedSet.insert(node.second->state);
        auto childNodes=getChildNode(node.second,node.first);
        //printf("childNodeNum:%d\n",childNodes.size());
        for(auto &s:childNodes){
            Node *nodeTmp= new Node;
            nodeTmp->state=s.second.state;
            nodeTmp->action=s.second.action;
            nodeTmp->pathCost=s.second.pathCost;
            nodeTmp->zero[0][0]=s.second.zero[0][0];nodeTmp->zero[0][1]=s.second.zero[0][1];
            nodeTmp->zero[1][0]=s.second.zero[1][0];nodeTmp->zero[1][1]=s.second.zero[1][1];
            nodeTmp->sevenX=s.second.sevenX;nodeTmp->sevenY=s.second.sevenY;
            nodeTmp->parent=s.second.parent;
            //printf("len=%d,(%d,%c),f:%d\n",strlen(nodeTmp->state),nodeTmp->action.first,nodeTmp->action.second,s.first+nodeTmp->pathCost);
            //_sleep(1000);
            //printf("(%d,%d),(%d,%d)\n",nodeTmp->zero[0][0],nodeTmp->zero[0][1],nodeTmp->zero[1][0],nodeTmp->zero[1][1]);
            openList.push(make_pair(s.first+nodeTmp->pathCost,nodeTmp));
        }
        //printf("\n");
    }
    return NULL;
}

void printAction(Node *result,FILE *fp){
    if(result->parent!=NULL){
        printAction(result->parent,fp);
        fprintf(fp,"(%d,%c);",(int)result->action.first,result->action.second);
    }
}


int main(int argc, char *argv[]) {
    if (argc == 3) {
        input(argv[1], argv[2]);

        clock_t timerBegin = clock();

        computeManhattan();
        Node* result=Astar();

        clock_t timerEnd = clock();

        FILE *fp;char outFileName[11];
        strcat(outFileName, strrchr(argv[1],'\\')+1);

        if ((fp = fopen(outFileName, "w")) == NULL) {
        printf("ERROR:can't create file!");
        exit(0);
        }
        printAction(result,fp);


        int time = (timerEnd - timerBegin) / CLOCKS_PER_SEC;
        printf("\n%ds\n", time);
    }
    return 0;
}