#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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

// 删除缓冲池节点
typedef struct delNode{
	char* Name;      // 名字
	time_t DeleteAt; // 删除时间(1970到现在的秒数)
	time_t RemoveAt; // 移除时间，用于刷新不为0时
	struct delNode* Next;
}delNode;

// 删除缓冲池 (头结构)
typedef struct delpool{
	delNode* Pool; // 字符串数组
	int Sum;       // 总数
}delpool;

// --request--
// 进货请求
typedef struct altRequest{
	char* Name;
	int Sum;
}altRequest;

// --func--
Status InitDB();
Status InitTOPARR();
Status InitDelPool();
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
Status SetDeleteTIme(const char* Name);
Status SetRemoveTime(const char* Name);

// --CommandAction--
Status SearchAction(int order);
Status SelectAction(int order,char** goods,int index);
Status AddAction(int order,char** goods,int index,int* sum);
Status NewAction(int order,char** goods,int index,int* sum);
Status DelAction(char** goods,int index,int* sum);
Status CheckPool();