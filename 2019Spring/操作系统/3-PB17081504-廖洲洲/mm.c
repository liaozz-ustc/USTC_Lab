/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)   //Extend heap by 4k

#define MAX(x,y) ((x)>(y)? (x):(y))

#define PACK(size,alloc) ((size) | (alloc))  //将大小和是否分配按位或组合

#define GET(p) (*(unsigned int *)(p)) //read
#define PUT(p,val) (*(unsigned int *)(p) = (unsigned int)(val) )//write

#define GET_SIZE(p) (GET(p) & ~0x7) //get the size 
#define GET_ALLOC(p) (GET(p) & 0x1) //该块是否已分配
#define GET_PREALLOC(p) (GET(p) & 0x2)//该块的前邻居块是否已分配,未分配得到0,否则2

#define HDRP(bp) ((char *)(bp) - WSIZE)//head
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) -DSIZE)//foot

#define GET_PRE(p)  ( (char *)GET((p)+WSIZE) )
#define GET_SUC(p) ( (char *)GET((p)+DSIZE))//得到空块前驱和后继
#define PUT_PRE(p,val) (PUT((p)+WSIZE,(val)))
#define PUT_SUC(p,val) (PUT((p)+DSIZE,(val)))//更改前驱和后继

#define NEXT_BLKP(bp) ( (char *)(bp) + GET_SIZE(HDRP((bp))) )//return next bp
#define PREV_BLKP(bp) ( (char *)(bp) - GET_SIZE(((char *)(bp) -DSIZE))  )//只有在前邻居块空的时候才能使用return pre bp  

char *heap_list;
char *free_up;//可用内存上下界
char *free_down;
unsigned int  end_alloc=0;//判断最后一块是否已分配

int mm_init(void);
void *extend_heap(size_t words);
void *coalesce(char *bp,unsigned int pre_alloc);
void *get_pre_emptyp(char *bp);
void *find_fit(size_t asize);
void place(char *bp,size_t asize);
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{	
	if((heap_list = mem_sbrk(8*WSIZE)) == NULL)
		return -1;
//	printf("heap_list: %o\n",heap_list);
	PUT(heap_list,0);
	PUT(heap_list+WSIZE,PACK(8,3));
	PUT(heap_list+WSIZE+DSIZE,PACK(16,2));
//	printf("...%o....\n",(unsigned int)heap_list+DSIZE+DSIZE);
    PUT(heap_list+DSIZE+DSIZE,heap_list+DSIZE+DSIZE);
//    PUT(heap_list+DSIZE+DSIZE,026604530030);
	PUT(heap_list+DSIZE+DSIZE+WSIZE,0);
	PUT(heap_list+DSIZE+DSIZE+WSIZE,0);
    PUT(heap_list+DSIZE+DSIZE+DSIZE,PACK(16,2));
    PUT(heap_list+DSIZE+DSIZE+DSIZE+WSIZE,PACK(0,1));
	char *h=heap_list;
//	printf("%o,%o,%o,%o\n%o,%o,%o,%o\n",*h,*(h+WSIZE),*(h+DSIZE),*(h+12),*(unsigned int *)(h+16),*(h+20),*(h+24),*(h+28));
	//注意我们设置的heap_list是字符串型的指针，故在取值时要进行转换为int才能读出正确的数
	heap_list += DSIZE+WSIZE; 
	free_up = heap_list;
	free_down = free_up+DSIZE+DSIZE;
//	end_alloc=0;	
//	printf("heap_list:%o,free_down:%o\n",heap_list,free_down);
	if( extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;

	return 0;

}

void *extend_heap(size_t words)
{
	char  *bp;
	size_t size;
//	printf("extend_heap\n");
	size = (words%2) ? (words + 1)*WSIZE : words *WSIZE;
	if((long)(bp = mem_sbrk(size)) == -1)
		return NULL;
//	printf("bp=%o\n",bp);
//	printf("GET heap size:%o\n",size);
	free_down +=size;//扩堆成功，修改下界
	PUT(HDRP(bp),PACK(size,end_alloc));
	PUT(FTRP(bp),PACK(size,end_alloc));
	PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));
//	printf("%o,%o,%o\n",*(unsigned int *)(bp-4),*(unsigned int *)(FTRP(bp)),*(unsigned int *)(FTRP(bp)+4));
	return coalesce(bp,end_alloc);//前块未被分配,合并
}

void *get_pre_emptyp(char *bp)
{//-1说明前面无空块

	char *p_list;char *next_p;
	if(heap_list){
		if(heap_list > bp)
			return NULL;
		else {
			p_list = heap_list;
			while( (next_p=GET_SUC(p_list)) && p_list < bp)
				p_list =  next_p;
			if(p_list < bp)
				return p_list;   //在所有空块后面
			else 
				return GET_PRE(p_list);  //有空块在后面
			}
	}	
	else
		return NULL;


}
void *coalesce(char *bp,unsigned int pre_alloc)
{
	unsigned int next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));
	size_t newsize;
	void  *pre_emptyp;
	void  *next_emptyp;
//	printf("coalesce %o \n",bp);
	if(pre_alloc && next_alloc) { //case1
//		printf("case1\n");
		pre_emptyp = get_pre_emptyp(bp);
		PUT(HDRP(bp),PACK(size,2));
        PUT(FTRP(bp),PACK(size,2));
		if( pre_emptyp == NULL){ //自己作为第一个空块
		//	 PUT(HDRP(bp),PACK(size,2));
		//   PUT(FTRP(bp),PACK(size,2));
			 PUT_PRE(HDRP(bp),HDRP(bp));
			 PUT_SUC(HDRP(bp),heap_list);
			 if(heap_list)//作为唯一一个空块
				PUT_PRE(heap_list,HDRP(bp));
			 heap_list = HDRP(bp);
			 return (void *) bp;
				}
		else if(GET_SUC(pre_emptyp) == 0){  //作为最后一个空块
			PUT_PRE(HDRP(bp),pre_emptyp);
			PUT_SUC(HDRP(bp),GET_SUC(pre_emptyp));
			PUT_SUC(pre_emptyp,HDRP(bp));
			return(void*) bp;
			}
		else { // 其他普通情况
            PUT_PRE(HDRP(bp),pre_emptyp);
            PUT_SUC(HDRP(bp),GET_SUC(pre_emptyp));
			PUT_PRE(GET_SUC(pre_emptyp),HDRP(bp));
	        PUT_SUC(pre_emptyp,HDRP(bp));
			return (void*)bp;
			}
		}

	else if(pre_alloc && !next_alloc) {//case2,先修改大小一类
//		printf("case 2\n");
		next_emptyp = HDRP(NEXT_BLKP(bp));
		newsize=size+GET_SIZE(next_emptyp);
        PUT(HDRP(bp),PACK(newsize,2));
        PUT(FTRP(bp),PACK(newsize,2));
		if(heap_list == next_emptyp) {  //后空块是第一空块，即前面无空块
             PUT_PRE(HDRP(bp),HDRP(bp));
             PUT_SUC(HDRP(bp),GET_SUC(heap_list));
			 if(GET_SUC(heap_list))
             	PUT_PRE(GET_SUC(heap_list),HDRP(bp));
             heap_list = HDRP(bp);
				}
		else {   					//前面有空块
			pre_emptyp = GET_PRE(next_emptyp);
            PUT_PRE(HDRP(bp),pre_emptyp);
            PUT_SUC(HDRP(bp),GET_SUC(next_emptyp));
			if(GET_SUC(next_emptyp))
				PUT_PRE(GET_SUC(next_emptyp),HDRP(bp));  			
           	PUT_SUC(pre_emptyp,HDRP(bp));			
//			PUT_PRE(next_emptyp,HDRP(bp));
			}
		return (void *)bp;
		}
	//case ok
	else if( !pre_alloc && next_alloc){ //case 3,前面是空块，后面是分配块
//		printf("case 3\n");
		pre_emptyp = HDRP(PREV_BLKP(bp));
//		printf("pre_empty:%o\n",pre_emptyp);
		newsize=size+GET_SIZE(pre_emptyp);
		PUT(pre_emptyp,PACK(newsize,2));
// 		PUT(pre_emptyp,PACK(size,2));
		PUT(FTRP(pre_emptyp+WSIZE),PACK(newsize,2));//ERROR&&&&****这里不能用size+GET_SIZE(pre_empyp),因为pre_emptyp的大小在前面已经被修改了
//		next_emptyp = GET_SUC(pre_emptyp);//合并了，后继前驱都不用变，直接改大小即可
//		printf("%o=>%o,%o=>%o\n",pre_emptyp,*(unsigned int *)pre_emptyp,FTRP(bp),*(unsigned int *)FTRP(bp));
		return (void *) (pre_emptyp + WSIZE);
//		if((int)next_emptyp == 0){	//前面的空块是最后一个空块,
//            PUT_PRE(HDRP(bp),pre_emptyp);
//            PUT_SUC(HDRP(bp),next_emptyp);
//            PUT_SUC(pre_emptyp,HDRP(bp));
//			}
		}
		
	else { //前后都是空块,直接可得前后空块，修改前空块大小和后继即可
//		printf("case 4\n");
        next_emptyp = HDRP(NEXT_BLKP(bp));
        pre_emptyp = HDRP(PREV_BLKP(bp));
		void *next_emptyp_next=GET_SUC(next_emptyp);
		newsize=size+GET_SIZE(pre_emptyp)+GET_SIZE(next_emptyp);
        PUT(pre_emptyp,PACK(newsize,2));
        PUT(FTRP(pre_emptyp+WSIZE),PACK(newsize,2));
		PUT_SUC(pre_emptyp,next_emptyp_next);  //修改后继为下一个空块的后继
		if(next_emptyp_next)  //下一空块的后继不为空，修改前驱
			PUT_PRE(next_emptyp_next,pre_emptyp);
		return (void*)(pre_emptyp +WSIZE);
		}

}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	size_t asize;
	size_t extendsize;
	char *bp;
//	size_t end_alloc;
//	printf("malloc %o\n",size);
	if(size == 0)
		return NULL;
	
	if(size <= WSIZE)
		asize = DSIZE;
	else 
		asize = DSIZE * ((size + WSIZE + (DSIZE -1)) / DSIZE);   //调整后的大小
  
 	if( (bp = find_fit(asize)) != NULL) {
//		printf("AFTER FIND_FIT\n");
		place(bp,asize);
		return (void *) bp;
		}

	extendsize = MAX(asize,CHUNKSIZE);
	if((bp = extend_heap(extendsize/WSIZE)) == NULL)
		return NULL;
//	printf("AFTER EXTEND_HEAP");
	place(bp,asize);
	return (void *) bp; 
}

void *find_fit(size_t asize)
{
//	printf("find_fit %o\n",asize);
	char *bp;
	char *p_list;char *next_list;
	if(heap_list==0)
		return NULL;
	p_list = heap_list;
	while( (GET_SIZE(p_list) < asize) &&  (next_list = GET_SUC(p_list)))
		p_list = next_list;
	if(GET_SIZE(p_list) >+asize)		
//	if(!GET_ALLOC(p_list) && GET_SIZE(p_list) >= asize)
		return (void*)(p_list + WSIZE);
	return NULL;
}  

void place(char *bp,size_t asize){ //分割操作
	char *pre_emptyp;char *next_emptyp;
	char *new_emptyp;
	size_t size;//剩下的大小
	pre_emptyp = GET_PRE(HDRP(bp));
	next_emptyp = GET_SUC(HDRP(bp));
//	printf("error:alloc:%d,prealloc:%d,heap_list:%o,free_up:%o,free_down:%o\nbp:%o,pre_emptyp:%o,next_empty:%o\n",GET_ALLOC(HDRP(bp)),GET_PREALLOC(HDRP(bp)),heap_list,free_up,free_down,bp,pre_emptyp,next_emptyp);
//	printf("place %o asize %o case",bp,asize);
	if(bp==0)
		return ;
	if( GET_SIZE(HDRP(bp)) <=  (asize+4*WSIZE)) { //由于最小空闲块的大小为4字（16字节）若找的的空闲块分配后小于4字则全部分配完
//尾部块的分配情况判断，当分配的是最后一个空闲块，且该块的下一块是尾端时，说明最后一块是被分配的
		if(HDRP(NEXT_BLKP(bp)) == free_down)
			end_alloc=2;//最后一块被分配
		PUT(HDRP(bp),PACK(GET_SIZE(HDRP(bp)),3));
		if(HDRP(bp)==heap_list){  //第一个空块被分配
//			printf("!!!!!\n");
			heap_list = next_emptyp;
			if(next_emptyp)
				PUT_PRE(next_emptyp,next_emptyp);
			}
		else {//在中间的块或最后一块被分配
//			printf("@@@@@\n");

			PUT_SUC(pre_emptyp,next_emptyp);
			if(next_emptyp)
				PUT_PRE(next_emptyp,pre_emptyp);
		}
		
	}
	
	else{ //空闲块足够大，需要分割，这种情况下不会出现将最后端分配的情况
		size = GET_SIZE(HDRP(bp)) - asize;
		new_emptyp = HDRP(bp) +asize;  //新的空块
		PUT(HDRP(bp),PACK(asize,3));
		PUT(new_emptyp,PACK(size,2));
		PUT(FTRP(new_emptyp+WSIZE),PACK(size,2));
		if(HDRP(bp) == heap_list){//是第一个空块,修改首空块地址
//			printf("#####\n");
			heap_list=new_emptyp;
			PUT_PRE(new_emptyp,new_emptyp);
			PUT_SUC(new_emptyp,next_emptyp);
			if(next_emptyp)   //下一空块存在，修改前驱
				PUT_PRE(next_emptyp,new_emptyp);
		}
		else{ //是中间空块或最后空块
//			printf("$$$$$\n");
			PUT_PRE(new_emptyp,pre_emptyp);
			PUT_SUC(new_emptyp,next_emptyp);
			PUT_SUC(pre_emptyp,new_emptyp);
			if(next_emptyp)
				PUT_PRE(next_emptyp,new_emptyp);
		}
	}

}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{//这里会需要修改下一块的判断上一块是否已分配，若下一邻居块是被分配的，修改其上一块为为分配
	if( HDRP(bp) < free_up || HDRP(bp) > free_down)
		return NULL;
//	printf("free %o\n",bp);
	if(GET_ALLOC(HDRP(bp)) ==0)
		return;
	size_t size = GET_SIZE(HDRP(bp));
	size_t pre_alloc = GET_PREALLOC(HDRP(bp));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	if(HDRP(NEXT_BLKP(bp))==free_down)
		end_alloc=0;
	if(next_alloc){//下一邻居块被分配的
		next_alloc = GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(NEXT_BLKP(bp)),PACK(next_alloc,1));
		}
	PUT(HDRP(bp),PACK(size,pre_alloc));
	PUT(FTRP(bp),PACK(size,pre_alloc));
//	printf("!!!@@@###$$$ free################################$$$$$$$$$free %o\n @@@###$$$\n",GET(HDRP(bp)));
	coalesce(bp,pre_alloc);
//	return 1;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *bp, size_t size)
{
    void *oldbp = bp;
    void *newbp;
    size_t copySize;
  //  printf("realloc %o size %o\n",bp,size);
    copySize = GET_SIZE(HDRP(oldbp)) - WSIZE;
 	if(size<=copySize)
		return bp;
   	newbp = mm_malloc(size);

    if (newbp  == NULL)
      return NULL;

//    if (size < copySize)
//      copySize = size;
    memcpy(newbp, oldbp, copySize);
    mm_free(oldbp);
    return newbp;
}














