#include "errno.h"
#include "list.h"
#include<string.h>

// 初始化 DB
Status InitDB(){
	DB = (listHead*)malloc(sizeof(listHead));
	if (DB==NULL) return Error;
	return OK;
}

/*Status InsertDBList(int index,listNode* node){
	if (DB->Nums<index) return Error;

	listNode* tmp = DB->Next;
	for (int i=0;i<index;i++){
		tmp = tmp->Next;
	}

	node->Next = NULL;
	tmp->Next = node;

	DB->Nums++;
	return OK;
}*/

// 查找数据库，找到返回指向它的指针，找不到返回 NULL
listNode* SearchDBByName(const char* Name){
	listNode* tmp = DB->Next;
	
	while(tmp!=NULL){
		if (strcmp(tmp->Name,Name)){
			return tmp;
		}
	}

	return NULL;
}

// 商店进货，先查找商品，再插入
Status AddData(altRequest* ptr){
	if (ptr==NULL) return Error;

	listNode* tmp = SearchDBByName(ptr->Name);
	if (tmp==NULL) return Error; // record not found

	tmp->Sum += ptr->Sum;
	return OK;
}

// 新增商品，先查找，再插入
Status AddDataNotExist(altRequest* ptr){
	if (ptr==NULL) return Error;

	listNode* tmp = SearchDBByName(ptr->Name);
	if (tmp!=NULL) return Error; // record is existed

	tmp = (listNode*)malloc(sizeof(listNode));
	if (tmp==NULL) return Error;

	strcpy(tmp->Name,ptr->Name);
	tmp->Sum = ptr->Sum;
	tmp->Next = DB->Next;
	DB->Next = tmp;

	return OK;
}

// 商品--,先查找，再改。resp 返回 sum 为 0 的节点，用于设置定时任务
Status DelData(altRequest* ptr,listNode* resp){
	if (ptr==NULL||resp!=NULL) return Error;

	listNode* tmp = SearchDBByName(ptr->Name);
	if (tmp==NULL) return Error; // record not found

	if (tmp->Sum<ptr->Sum) return Error; // sum overflow

	tmp->Sum = tmp->Sum - ptr->Sum;

	if (tmp->Sum==0){
		resp = tmp;
		return Error; // sum = 0
	}

	return OK;
}

// 从链表删除，此为定时任务
Status DelDataFromList(listNode* node){
	
}