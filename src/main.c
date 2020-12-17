#include "terminal.h"
#include "command.h"
#include "file.h"
#include "list.h"
#include "sudo.h"
#include "errno.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TIME_LOOP 1800 // 30分钟

extern int Sudo;

int main(){
	// 启动界面
	outputHello();

	// 用于阻塞读取
	char line[101];

	// 初始化 DB
	if (!InitDB()) exit(1);
	if (!InitTOPARR()) exit(1);
	if (!InitDelPool()) exit(1);

	// 用于解析命令
	char** goods = (char**)malloc(sizeof(char*)*50);
	for (int i=0;i<50;i++){
		goods[i]=(char*)malloc(sizeof(char)*51);
	}
	int* sum = (int*)malloc(sizeof(int)*50);
	int index = 0;
	int order = 0;
	Status resp = 0;
	Status stat = 0;
	time_t Pivot = time(NULL)+TIME_LOOP;
	time_t Now = 0;

	while(1){
		fgets(line,100,stdin);
		if (strlen(line)>100) continue;
		line[strlen(line)-1] = '\0';// 末尾有个 '\n'
		stat = ResolveCommand(line,goods,sum,&index,&order);	
		switch (stat){ // 解析命令
			case SAVE: 
				if(Sudo){
					resp = Save(); 
				} else {
					resp = ERR_SUDO;
				}
				break;
			case READ: resp = Read(); break;
			case SUDO: resp = SetSudo(); break;
			case EXIT: resp = IfExist(); break;
			case MENU: resp = outputMenu(); break;
			case ADD: 
				if(Sudo){
					resp = AddAction(order,goods,index,sum);
				} else {
					resp = ERR_SUDO;
				}
				break;
			case NEW: 
				if (Sudo){
					resp = NewAction(order,goods,index,sum); 
				} else {
					resp = ERR_SUDO;
				}
				break;
			case DEL: 
				if(Sudo){
					resp = DelAction(goods,index,sum); 
				} else {
					resp = ERR_SUDO;
				}
				break;
			case SELECT: resp = SelectAction(order,goods,index); break;
			case SEARCH: resp = SearchAction(order); break;
			default: 
				resp = stat;
				break;
		}

		// 结果处理
		if (resp==EXIT_NOW) break;
		if (resp!=OK) {
			HandleStatus(resp);
		} else {
			printf("System:OK.\n");
		}

		// checkpool 伪定时任务
		Now = time(NULL);
		if (Now>Pivot){
			CheckPool();
			Pivot = Now+TIME_LOOP;
		}
		
		// next command
		if (Sudo) {
			printf("=>");
		} else {
			printf(">");
		}
	}

	// save
	Save();

	// free
	free(sum);
	for (int i=0;i<51;i++){
		free(goods[i]);
	}
	free(goods);
	
	// free DB
	DestroyDB();

	// free TOPARR
	DestroyTOPARR();

	// free DelPool
	DestroyDelPool();

	outputGoodbye();

	return 0;	
}
