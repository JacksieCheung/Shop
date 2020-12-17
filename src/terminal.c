#include<stdio.h>
#include "terminal.h"
#include "errno.h"

// 程序启动时 say hello 界面。
Status outputHello() {
#include "ascii_art.h"
	printf("%s",ascii_hello);
	printf(">");
	return OK;
}

Status outputMenu(){
#include "ascii_menu.h"
	printf("%s",ascii_menu);
	return OK;
}

Status outputGoodbye(){
	printf("System:data has been saved, see you next time^_^\n");
	return OK;
}