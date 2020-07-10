#include"Bank.h"

int main()
{
	printf("请输入银行模拟时间：\n");
	scanf("%d",&CloseTime);
	//银行排队模拟
    OpenForDay();
    srand((unsigned)time(NULL));
    while(!ListEmpty(&ev)){
        DelFirst(&ev,&en);
        printf("**********事件**********\n");
        if(en.NType==0)
            Arrive();
        else
            Leave();
        PrintQueue();
        PrintEventList();
    }
    printf("\n顾客逗留总时间总时间: %d min,顾客平均逗留时间: %g min.\n",TotalTime,(float)TotalTime/Num);
	system("pause");
}


void OpenForDay(){
    //初始化操作
    int i;
    TotalTime=0;    Num=0;
    InitList(&ev);//初始化事件队列
    en.OccurTime=0;
    en.NType=0;
    OrderInsert(&ev,en);
    for(i=1;i<=4;i++)
        InitQueue(&q[i]);//初始化四个窗口队列
}//OpenForDay

void Arrive(){
    //顾客到达事件
    int durtime,Nexttime,i,t;
    QElemType e;
    Num++;
    Nexttime=rand()%5+1;   //间隔时间在5分钟内
    durtime=rand()%30+1;    //办理业务时间在30分钟内
    t=en.OccurTime+Nexttime;
    if( en.OccurTime<CloseTime){//银行尚未关门
        printf("顾客到达时间:%d,该顾客办理业务所需时间:%d,下一位顾客到达时间间隔:%d|\n",en.OccurTime,durtime,Nexttime);//下一位顾客达到时间
        OrderInsert(&ev,NewEvent(t,0));
        i=ShortestQueue();//最短队列
        e.ArriveTime=en.OccurTime;
        e.Duration=durtime;
        EnQueue(&q[i],e);
        if(Length(q[i])==1)
            OrderInsert(&ev,NewEvent(en.OccurTime+durtime,i));
    }else{
        printf("一位顾客到达:%d，但银行不再接受业务办理了！\n",en.OccurTime);
        Num--;
    }
}

void Leave(){
    //顾客离开事件
    int i=en.NType;
    DelQueue(&q[i],&customer);
    printf("一位顾客离开，现在时间:%d\n",en.OccurTime);//输出顾客离开时间
    TotalTime+=en.OccurTime-customer.ArriveTime;
    if(!EmptyQueue(&q[i])){
        GetHead(&q[i],&customer);
        OrderInsert(&ev,NewEvent(en.OccurTime+customer.Duration,i));
    }
}





void PrintQueue(){
    //打印当前队列
    int i;
    for(i=1;i<=4;i++){
        printf("窗口 %d 有 %d 名顾客:",i,Length(q[i]));
        QueueTraverse(&q[i]);
    }
    printf("\n");
}

void PrintEventList(){
    //输出事件队列
    printf("事件表:(时间,类型)\n");
    ListTraverse(&ev);
}
int Min(int a[],int n){
    //返回长度为n的数组a第一个最小值的下标，从0开始
    int i,tmp,ind=0;
    tmp=a[0];
    for(i=1;i<n;i++){
        if(a[i]<tmp){
            tmp=a[i];
            ind=i;
        }
    }
    return ind;
}

int ShortestQueue(){
    //获取最短队列的编号
    int i,a[4];
    for(i=1;i<=4;i++){
        a[i-1]=Length(q[i]);
        //printf("队%d的长度为%d\n",i,Length(q[i]));
    }
    return Min(a,4)+1;//队列从1开始编号
}




int OrderInsert(EventList *L,Event e){
    //将事件e按发生时间顺序插入有序链表L中
    ElemType *p,*q,*n;
    n=(ElemType *)malloc(sizeof(ElemType));
    if(!n){
        printf("申请空间失败\n");
        exit(0);
    }
    *n=e;
    if(TRUE==ListEmpty(L)){//链表为空
       L->head->next=n;
       L->tail=n;
       n->next=NULL;
       return OK;
    }
    q=L->head;
    p=L->head->next;
    while(p){//遍历整个链表
        if(p->OccurTime>=n->OccurTime)
            break;
        q=p;
        p=p->next;
    }
    q->next=n;
    n->next=p;
    if(!p)//插入位置为链表尾部
        L->tail=n;
    return OK;
}

int ListEmpty(EventList *L){
    //判断链表L是否为空，为空返回TRUE，否则返回FALSE
    if((L->head==L->tail)&&(L->head!=NULL))
        return TRUE;
    else
        return FALSE;
}

int DelFirst(EventList *L,ElemType *e){
    //链表L不为空，删除其首结点，用e返回，并返回OK；否则返回ERROR
    ElemType *p=L->head->next;
    if(!p)
        return ERROR;
    L->head->next=p->next;
    *e=*p;
    free(p);
    if(L->head->next==NULL)
        L->tail=L->head;
    return OK;
}

int ListTraverse(EventList *L){
    //遍历链表
    Event *p=L->head->next;
    if(!p){
        printf("事件表为空！\n");
        return ERROR;
    }
   
    while(p!=NULL){
    
        printf("(%d,%d)\n",p->OccurTime,p->NType);
        p=p->next;
       
   		 }

    printf("\n");
    return OK;
}

int InitQueue(LinkQueue *Q){
    //初始化队列Q
    Q->head=Q->tail=(QElemType *)malloc(sizeof(QElemType));
    if(!Q->head){
        printf("申请空间失败\n");
        exit(0);
    }
    Q->head->next=NULL;
    return OK;
}



int Length(LinkQueue Q){
    //返回队列Q的长度，即元素个数
    int count=0;
    QElemType *p=Q.head->next;
    while(p){
        p=p->next;
        count++;
    }
    return count;
}

int GetHead(LinkQueue *Q,QElemType *e){
    //若队列Q不为空，用e返回其首结点，并返回OK,否则返回ERROR
    if(EmptyQueue(Q))
        return ERROR;
    *e=*(Q->head->next);
        return OK;
}

int DelQueue(LinkQueue *Q,QElemType *e){
    //若队列Q不为空，首结点出队，用e返回，并返回OK；否则返回ERROR
    QElemType *p=Q->head->next;
    if(!p)
        return ERROR;
    *e=*p;
    Q->head->next=p->next;//修正队首指针
    free(p);
    if(!Q->head->next)//队空
        Q->tail=Q->head;
    return OK;
}


int QueueTraverse(LinkQueue *Q){
    //遍历队列Q
    QElemType *p=Q->head->next;
    if(!p){
        printf("--空.\n");
        return ERROR;
    }
    while(p){
        printf("(%d,%d) ",p->ArriveTime,p->Duration);
        p=p->next;
    }
    printf("\n");
    return OK;
}
int EmptyQueue(LinkQueue *Q){
    //若队列Q为空，返回TRUE，否则返回FALSE
    if(Q->head==Q->tail&&Q->head!=NULL)
        return TRUE;
    else
        return FALSE;
}
int EnQueue(LinkQueue *Q,QElemType e){
    //结点e入队Q
    QElemType *p=(QElemType *)malloc(sizeof(QElemType));
    if(!p){
        printf("申请空间失败\n");
        exit(0);
    }
    *p=e;
    p->next=NULL;
    Q->tail->next=p;//插入队尾
    Q->tail=p;//修改队尾指针
    return OK;
}
Event NewEvent(int occurT,int nType){
    //根据OccurTime和NType值，创建新事件
    Event e;
    e.OccurTime=occurT;
    e.NType=nType;
    return e;
}

int InitList(EventList *L){
    //初始化事件链表
    L->head=L->tail=(ElemType *)malloc(sizeof(ElemType));
    if(!L->head){
        printf("申请空间失败\n");
        exit(0);
    }
    L->head->next=NULL;
    return OK;
}
