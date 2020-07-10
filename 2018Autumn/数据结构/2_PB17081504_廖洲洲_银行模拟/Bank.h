#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<process.h>

#define OK 1
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<process.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0


typedef struct QElemType { 	//顾客
	int ArriveTime;			//到达时间
	int Duration;			//办理业务所需时间
	struct QElemType *next;
}QElemType;

typedef struct {        //队列窗口
	QElemType *head;
	QElemType *tail;
}LinkQueue;

typedef struct Event {    //事件
	int OccurTime;       //事件发生时刻
	int NType;          //事件类型，0表示到达事件，1至4表示该窗口的顾客离开
	struct Event *next;
}Event, ElemType;

typedef struct EventList {       //事件表
	ElemType *head;
	ElemType *tail;
}EventList;




Event NewEvent(int occurT, int nType);
//根据OccurTime和NType值，创建新事件
int InitList(EventList *L);
//初始化事件链表
int OrderInsert(EventList *L, Event e);
//将事件e按发生时间顺序插入有序链表L中
int ListEmpty(EventList *L);
//判断链表L是否为空，为空返回TRUE，否则返回FALSE
int DelFirst(EventList *L, ElemType *e);
//链表L不为空，删除其首结点，用e返回，并返回OK；否则返回ERROR
int ListTraverse(EventList *L);
//遍历链表
int InitQueue(LinkQueue *Q);
//初始化队列Q
int EmptyQueue(LinkQueue *Q);
//若队列Q为空，返回TRUE，否则返回FALSE
int DelQueue(LinkQueue *Q, QElemType *e);
//若队列Q不为空，首结点出队，用e返回，并返回OK；否则返回ERROR
int EnQueue(LinkQueue *Q, QElemType e);
//结点e入队Q
int Length(LinkQueue Q);
//返回队列Q的长度，即元素个数
int GetHead(LinkQueue *Q, QElemType *e);
//若队列Q不为空，用e返回其首结点，并返回OK,否则返回ERROR
int QueueTraverse(LinkQueue *Q);
//遍历队列Q


int Min(int a[], int n);
//返回长度为n的数组a第一个最小值的下标，从1开始
int ShortestQueue();
//获取最短队列的编号
void OpenForDay();
//初始化操作
void Arrive();
//顾客达到事件
void Leave();
//顾客离开事件
void PrintEventList();
//输出事件队列
void PrintQueue();
//打印当前队列
//----全局变量-----//
EventList ev;
Event en;
LinkQueue q[5];
QElemType customer;
int TotalTime, Num;
int CloseTime;//关闭时间，即营业时间长度


