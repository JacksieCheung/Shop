#include "errno.h"
#include "list.h"
#include<string.h>
#define INIT_SIZE 10
#define QUEUE_SIZE 10
#define TIME_INIT 1800 // 30分钟

// 顶层数组
Array* TOPARR = NULL;

// 数据库变量
listHead* DB=NULL;

// 软删除队列
delpool* DelPool = NULL;

// 初始化 DB
Status InitDB(){
	DB = (listHead*)malloc(sizeof(listHead));
	if (DB==NULL) return ERR_MALLOC;
	return OK;
}

// 销毁 DB
Status DestroyDB(){
	listNode* tmp = DB->Next;
	if(tmp==NULL){
		free(DB);
		return OK;
	}
	while(tmp->Next!=NULL){
		DB->Next = tmp->Next;
		free(tmp->Name);
		free(tmp);
		tmp = DB->Next;
	}

	free(DB->Next);
	free(DB);
	return OK;
}

// 重置 DB
Status ResetDB(){
	listNode* tmp = DB->Next;
	if (tmp==NULL) return OK;
	while(tmp->Next!=NULL){
		DB->Next = tmp->Next;
		free(tmp->Name);
		free(tmp);
		tmp = DB->Next;
	}

	free(DB->Next);
	DB->Next = NULL;
	return OK;
}

// 初始化 Array
Status InitTOPARR(){
	TOPARR = (Array*)malloc(sizeof(Array));
	if (TOPARR==NULL) return ERR_MALLOC;

	TOPARR->Arr = (listNode**)malloc(sizeof(listNode*)*INIT_SIZE);
	if (TOPARR->Arr==NULL) return ERR_MALLOC;

	TOPARR->Containt = INIT_SIZE;
	TOPARR->Length = 0;
	return OK;
}

// 销毁 Array
Status DestroyTOPARR(){
	free(TOPARR->Arr);
	free(TOPARR);
	return OK;
}

// 重置 TOPARR
Status ResetTOPARR(){
	TOPARR->Length = 0;
	return OK;
}

// 初始化删除队列
Status InitDelPool(){
	DelPool = (delpool*)malloc(sizeof(delpool));
	if (DelPool==NULL) return ERR_MALLOC;
	return OK;
}

// 销毁 DelPool
Status DestroyDelPool(){
	delNode* tmp = DelPool->Pool;
	if (tmp==NULL){
		free(DelPool);
		return OK;
	}
	while(tmp->Next!=NULL){
		DelPool->Pool = tmp->Next;
		free(tmp->Name);
		free(tmp);
		tmp = DelPool->Pool;
	}

	free(DelPool->Pool);
	free(DelPool);
	return OK; 
}

// 重置 DelPool
Status ResetDelPool(){
	delNode* tmp = DelPool->Pool;
	if (tmp==NULL) return OK;
	while(tmp->Next!=NULL){
		DelPool->Pool = tmp->Next;
		free(tmp->Name);
		free(tmp);
		tmp = DelPool->Pool;
	}

	free(DelPool->Pool);
	DelPool->Pool = NULL;
	return OK; 
}

// 查找池子,删除过期的节点
Status CheckPool(){
	delNode* tmp = DelPool->Pool;
	if (tmp==NULL) return OK;
	
	time_t now = time(NULL);

	delNode* tmp2 = tmp;
	while(tmp2!=NULL) {
		printf("%s,%ld,%ld\n",tmp2->Name,tmp2->DeleteAt,tmp2->RemoveAt);
		tmp2 = tmp2->Next;
	}

	// 检查自身
	while(tmp!=NULL){
		printf("time:%ld\n",time(NULL));
		if (now>tmp->DeleteAt&&tmp->DeleteAt>0){
			DelDataFromList(SearchDBByName(tmp->Name));
			DelPool->Pool = tmp->Next;
			// free tmp
			free(tmp->Name);
			free(tmp);
			tmp = DelPool->Pool;
			DelPool->Sum = DelPool->Sum-1;
		} else if(now>tmp->RemoveAt&&tmp->RemoveAt>0){
			DelPool->Pool = tmp->Next;
			// free tmp
			free(tmp->Name);
			free(tmp);
			tmp = DelPool->Pool;
			DelPool->Sum = DelPool->Sum-1;
		} else {
			break;
		}
	}

	if (tmp==NULL) return OK;

	// 如果当前时间超过两个节点，同时删除 Pool 和 DB
	while(tmp->Next!=NULL){
		if(now>tmp->Next->DeleteAt&&tmp->Next->DeleteAt>0){
			DelDataFromList(SearchDBByName(tmp->Next->Name));
			delNode* node = tmp->Next;
			tmp->Next = node->Next;
			free(node->Name);
			free(node);
			DelPool->Sum = DelPool->Sum-1;
		} else if (now>tmp->Next->RemoveAt&&tmp->Next->RemoveAt>0){
			delNode* node = tmp->Next;
			tmp->Next = node->Next;
			free(node->Name);
			free(node);
			DelPool->Sum = DelPool->Sum-1;
		}
		tmp = tmp->Next;
	}

	return OK;
}

// 设置删除时间
Status SetDeleteTime(const char* Name){
	delNode* tmp = DelPool->Pool;
	
	while(tmp!=NULL) {
		if (strlen(tmp->Name)==strlen(Name)
		&&!strcmp(tmp->Name,Name)){
			// exist
			tmp->DeleteAt = time(NULL)+TIME_INIT;
			tmp->RemoveAt = 0;
			return OK;
		}
		tmp = tmp->Next;
	}

	// set time
	delNode*  node = (delNode*)malloc(sizeof(delNode));
	node->Name = (char*)malloc(sizeof(char)*strlen(Name));
	strcpy(node->Name,Name);
	node->DeleteAt = time(NULL)+TIME_INIT;
	node->Next = DelPool->Pool;
	DelPool->Pool = node;
	return OK;
}

Status SetRemoveTime(const char* Name){
	delNode* tmp = DelPool->Pool;
	if (tmp==NULL) return OK; // empty
	
	while(tmp!=NULL) {
		if (strlen(tmp->Name)==strlen(Name)
		&&!strcmp(tmp->Name,Name)){
			tmp->DeleteAt = 0;
			tmp->RemoveAt = time(NULL)+TIME_INIT;
			return OK;
		}
	}
	
	return ERR_NODENOTFOUNDPOOL; // not found
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
		TOPARR->Arr = (listNode**)realloc(TOPARR->Arr,(TOPARR->Containt+INIT_SIZE)*sizeof(listNode*));
		TOPARR->Containt = TOPARR->Containt + INIT_SIZE;	
	}

	TOPARR->Arr[TOPARR->Length] = node;
	TOPARR->Length = TOPARR->Length + 1;
	printf("%d|%d\n",TOPARR->Containt,TOPARR->Length);
	return OK;
}

// 删除元素
Status DelArr(listNode* node){
	int len = TOPARR->Length;
	int index = 0;
	for (int i=0;i<len;i++){
		if (strlen(node->Name)==strlen(TOPARR->Arr[i]->Name)
		&&!strcmp(node->Name,TOPARR->Arr[i]->Name)){
			index = i;
			break;
		}
	}

	while(index<len-1){
		TOPARR->Arr[index]=TOPARR->Arr[index+1];
		index++;
	}

	TOPARR->Length = TOPARR->Length - 1;
	printf("%d|%d\n",TOPARR->Containt,TOPARR->Length);
	return OK;
}

// 查找数据库，返回前一个节点指针。
listNode* SearchDBBefore(const char* Name,int* ifFirst){
	listNode* tmp = DB->Next;
	if (tmp==NULL) return NULL;

	// 检查自身
	if (strlen(tmp->Name)==strlen(Name)
	&&!strcmp(tmp->Name,Name)){
		*ifFirst = 1;
		return tmp;
	}

	while(tmp->Next!=NULL){
		if(strlen(tmp->Next->Name)==strlen(Name)
		&&!strcmp(tmp->Next->Name,Name)){
			return tmp;
		}
	}

	return NULL;
}

// 查找数据库，找到返回指向它的指针，找不到返回 NULL
listNode* SearchDBByName(const char* Name){
	if (Name==NULL) return NULL;
	if (DB->Next==NULL) return NULL;
	listNode* tmp = DB->Next;
	
	while(tmp!=NULL){
		if (strlen(tmp->Name)==strlen(Name)
		&&!strcmp(tmp->Name,Name)){
			return tmp;
		}
		tmp = tmp->Next;
	}

	return NULL;
}

// 商店进货，先查找商品，再插入
Status AddData(altRequest* ptr){
	if (ptr==NULL) return ERR_EMPTYGOODS;

	// printf("%s\n",ptr->Name);
	listNode* tmp = SearchDBByName(ptr->Name);
	if (tmp==NULL) return ERR_NODENOTFOUNDDB; // record not found

	// if sum==0, set remove time
	if (tmp->Sum==0) SetRemoveTime(tmp->Name);

	tmp->Sum += ptr->Sum;
	SortArr();
	
	return OK;
}

// 新增商品，先查找，再插入
Status AddDataNotExist(altRequest* ptr){
	if (ptr==NULL) return ERR_EMPTYGOODS;

	listNode* tmp = SearchDBByName(ptr->Name);
	if (tmp!=NULL) return ERR_NODEEXISTEDDB; // record is existed

	tmp = (listNode*)malloc(sizeof(listNode));
	if (tmp==NULL) return ERR_MALLOC;

	tmp->Name = (char*)malloc(sizeof(strlen(ptr->Name)));
	strcpy(tmp->Name,ptr->Name);
	tmp->Sum = ptr->Sum;
	tmp->Next = DB->Next;
	DB->Next = tmp;

	InsertArr(DB->Next);
	SortArr();

	return OK;
}

// 商品--,先查找，再改。修改队列
Status DelData(altRequest* ptr){
	if (ptr==NULL) return ERR_EMPTYGOODS;

	listNode* tmp = SearchDBByName(ptr->Name);
	if (tmp==NULL) return ERR_NODENOTFOUNDDB; // record not found

	if (tmp->Sum<ptr->Sum) return ERR_SUMOVERFLOW; // sum overflow

	tmp->Sum = tmp->Sum - ptr->Sum;

	if (tmp->Sum==0){
		// set delete time
		SetDeleteTime(tmp->Name);
	}

	return OK;
}

// 从链表删除，此为定时任务
Status DelDataFromList(listNode* node){
	if (node==NULL) return ERR_EMPTYGOODS;

	int ifFirst = 0;

	listNode* tmp = SearchDBBefore(node->Name,&ifFirst);

	// Del from TOPARR
	DelArr(tmp);

	if (ifFirst){
		DB->Next = tmp->Next;
		free(tmp->Name);
		free(tmp);
	} else{
		tmp->Next = node->Next;
		free(node->Name);
		free(node);
	}

	DB->Nums = DB->Nums-1;

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
		printf("%3d\n",TOPARR->Arr[i]->Sum);
	}
}

// --CommandAction--
// search
Status SearchAction(int order){
	if (DB->Next==NULL) return ERR_EMPTYDB;
	switch(order){
		case NONE: ListData();break;
		case DESC: ListDataDesc();break;
		case ASC: ListDataAsc();break;
	}
	return OK;
}

// select
Status SelectAction(int order,char** goods,int index){
	if (DB->Next==NULL) return ERR_EMPTYDB;
	if (order==1){
		for (int i=0;i<TOPARR->Length;i++){
			for (int j=0;j<index;j++){
				if (strlen(TOPARR->Arr[i]->Name)==strlen(goods[j])
				&&!strcmp(TOPARR->Arr[i]->Name,goods[j])){
					printf("%s   ",TOPARR->Arr[i]->Name);
					printf("%d\n",TOPARR->Arr[i]->Sum);
				}
			}
		}
	} else {
		for (int i=TOPARR->Length-1;i>=0;i--){
			for (int j=0;j<index;j++){
				if (strlen(TOPARR->Arr[i]->Name)==strlen(goods[j])
				&&!strcmp(TOPARR->Arr[i]->Name,goods[j])){
					printf("%s   ",TOPARR->Arr[i]->Name);
					printf("%d\n",TOPARR->Arr[i]->Sum);
				}
			}
		}
	}
	return OK;
}

// add
Status AddAction(int order,char** goods,int index,int* sum){
	int status = 0;
	for (int i=0;i<index;i++){
		altRequest tmp;
		tmp.Name = goods[i];
		tmp.Sum = sum[i];
		status = AddData(&tmp);	
		if (status!=OK) return status;
	}
	return OK;
}

// new
Status NewAction(int order,char** goods,int index,int* sum){
	int status = 0;
	for (int i=0;i<index;i++){
		altRequest tmp;
		tmp.Name = goods[i];
		tmp.Sum = sum[i];
		status = AddDataNotExist(&tmp);
		if (status!=OK) return status;
	}
	return OK;
}

// del
Status DelAction(char** goods,int index,int* sum){
	int status = 0;
	for (int i=0;i<index;i++){
		altRequest tmp;
		tmp.Name = goods[i];
		tmp.Sum = sum[i];
		status = DelData(&tmp);
		if (status!=OK) return status;
	}
	return OK;
}