#include "errno.h"
#include "list.h"
#include<string.h>
#define INIT_SIZE 10

// 初始化 DB
Status InitDB(){
	DB = (listHead*)malloc(sizeof(listHead));
	if (DB==NULL) return Error;
	return OK;
}

// 初始化 Array
Status InitTOPARR(){
	TOPARR = (Array*)malloc(sizeof(Array));
	if (TOPARR==NULL) return Error;

	TOPARR->Arr = (listNode**)malloc(sizeof(listNode*)*INIT_SIZE);
	if (TOPARR->Arr==NULL) return Error;

	TOPARR->Containt = INIT_SIZE;
	return OK;
}

// 插入排序
void SortArr(){                                                                                                                                                 
	int len = TOPARR->Length;
	for (int j=1;j<len;j++) {
        listNode* key = TOPARR->Arr[j];
        int i = j-1;
        while(i>=0&&(TOPARR->Arr[i])->Sum>key->Sum) {       
            TOPARR->Arr[i+1]=TOPARR->Arr[i];
            i--;
        }
        TOPARR->Arr[i+1] = key;
    }
}

// 插入元素
Status InsertArr(listNode* node){
	if (TOPARR->Length==TOPARR->Containt){
		// realloc
		TOPARR->Containt = TOPARR->Containt + INIT_SIZE;	
	}

	TOPARR->Arr[TOPARR->Length] = node;
	TOPARR->Length = TOPARR->Length + 1;
	return OK;
}

// 查找数据库，返回前一个节点指针。
listNode* SearchDBBefore(const char* Name){
	listNode* tmp = DB->Next;
	if (tmp==NULL) return NULL;

	while(tmp->Next!=NULL){
		if (strcmp(tmp->Next->Name,Name)){
			return tmp;
		}
	}

	return NULL;
}

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
	SortArr();
	
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

	InsertArr(DB->Next);
	SortArr();

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

	resp = NULL;
	return OK;
}

// 从链表删除，此为定时任务
Status DelDataFromList(listNode* node){
	if (node==NULL) return Error;
	
	listNode* tmp = SearchDBBefore(node->Name);
	if (tmp==NULL) return Error; // record not found
	
	tmp->Next = node->Next;
	free(node);

	return OK;
}

// 列出商品
void ListData(){
	listNode* tmp = DB->Next;

	while(tmp!=NULL) {
		printf("%s   ",tmp->Name);
		printf("%3d\n",tmp->Sum);
		tmp = tmp->Next;
	}
}

// 降序
void ListDataDesc(){
	for(int i=TOPARR->Length-1;i>=0;i--){
		printf("%s    ",TOPARR->Arr[i]->Name);
		printf("%3d\n",TOPARR->Arr[i]->Sum);
	}
}

// 升序
void ListDataAsc(){
	for(int i=0;i<TOPARR->Length;i++){
		printf("%s    ",TOPARR->Arr[i]->Name);
		printf("3d\n",TOPARR->Arr[i]->Sum);
	}
}