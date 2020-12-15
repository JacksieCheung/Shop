#include<stdio.h>
#include "terminal.h"
#include "command.h"
#include<string.h>

int main(){
	// outputHello();
	// 阻塞读取
	char line[101];
	while(1){
		fgets(line,100,stdin);
		line[strlen(line)-1] = '\0';
		switch (ResolveCommand(line)){ // 解析命令
			case 0: break;
			case 1: break;
			case 2: break;
			default: 
				printf("error\n");
				break;
		}
	}
	return 0;	
}
