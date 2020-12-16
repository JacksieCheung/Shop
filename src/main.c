#include "terminal.h"
#include "command.h"
#include "list.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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
	int order = -1;
	int resp = 0;

	while(1){
		fgets(line,100,stdin);
		if (strlen(line)>100) continue;
		line[strlen(line)-1] = '\0';// 末尾有个 '\n'
		switch (ResolveCommand(line,goods,sum,&index,&order)){ // 解析命令
			case 0: break;
			case 1: break;
			case 2: break;
			case 3: break;
			case 4: break;
			case 5: resp = AddAction(order,goods,index,sum); break;
			case 6: resp = NewAction(order,goods,index,sum); break;
			case 7: resp = DelAction(goods,index,sum); break;
			case 8: resp = SelectAction(order,goods,index); break;
			case 9: resp = SearchAction(order); break;
			case 10: resp = SearchAction(order); break;
			default: 
				printf("error\n");
				break;
		}

		// 结果处理
		printf("resp:%d\n",resp);
		CheckPool();
		printf(">");
	}

	// save

	// free
	free(sum);
	for (int i=0;i<51;i++){
		free(goods[i]);
	}
	free(goods);
	return 0;	
}
