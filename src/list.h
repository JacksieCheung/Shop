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

// 顶层数组头节点，用于排序
typedef struct array{
	int Length;     // 逻辑长度
	int Containt;   // 物理长度
	listNode** Arr; // 数组
}Array;

Array* TOPARR = NULL;

// 数据库变量
listHead* DB=NULL;

// --request--
// 进货请求
typedef struct altRequest{
	char* Name;
	int Sum;
}altRequest;

// --func--
Status InitDB();
Status InitTOPARR();
void SortArr();
Status InsertArr();
listNode* SearchDBBefore();
listNode* SearchDBByName();
Status AddData();
Status AddDataNotExist();
Status DelData();
Status DelDataFromList();
void ListData();
void ListDataDesc();
void ListDataAsc();
