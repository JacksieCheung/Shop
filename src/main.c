#include<stdio.h>
#include "terminal.h"
#include "command.h"

int main(){
	outputHello();
	// 阻塞读取
	char line[100];
	while(scanf("%[^\n]",line)!=EOF){
		switch (ResolveCommend(line)){ // 解析命令
			case 0: break;
			case 1: break;
			case 2: break;
			default: break;
		}
	}
	return 0;	
}
