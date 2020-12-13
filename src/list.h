#include<stdio.h>
#include<stdlib.h>
#include "errno.h"

// --structure--
// 节点结构
typedef struct listNode{
	char* Name; // 名字
	int Sum;    // 总数
	struct listNode* Next;
}listNode;

// 头结构
typedef struct listHead{
	int Nums;       // 成员个数
	listNode* Next; // 指向孩子
}listHead;

// 数据库变量
listHead* DB=NULL;

// --request--
// 进货请求
typedef struct altRequest{
	char* Name;
	int Sum;
}altRequest;

// --func--
// 初始化链表，malloc
Status InitDB();

// 插入表
Status InsertDB(int index,listNode* node);
