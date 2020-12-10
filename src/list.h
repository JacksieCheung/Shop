#include<stdio.h>
#include<stdlib.h>
#include "errno.h"

// --structure--

typedef struct listNode{
	int ID;     // 唯一标志
	char* Name; // 名字
	int Sum;    // 总数
}listNode;

listNode* DB=NULL;

// --func--
// 初始化链表，malloc
Status InitDB(){
	DB = (listNode*)malloc(sizeof(listNode));
	if (DB==NULL) return Error;
	return OK;
}

// 插入表
