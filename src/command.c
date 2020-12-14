#include "command.h"
#include<string.h>

// 拆分字符串
int GetCommendHead(char* line,int len,char* head){
    // 记录遇到的第一个 ';' 或' ';
    int index = 0;
    for(;index<len;index++){
        if (line[index]==' '){
            for(int i=0;i<index;i++) head[i]=line[i];
            return 1; // 有参数
        }
        if (line[index]==';'){
            for (int i=0;i<index;i++) head[i]=line[i];
            return 0; // 无参数
        }
    }

    return -1;// 指令出错
}

// 解析 add 指令
int GetAddParam(char* line,int len){

}

// 解析命令
int ResolveCommend(char* line){
    int len = strlen(line);
    char head[7];

    if (line[len-1]!=';') return -1;

    switch(GetCommendHead(line,len,head)){
        case 0: 
            if (strcmp(head,"save")){
                // save action
                return 0;
            } else if(strcmpy(head,"read")){
                // read action
                return 1;
            } else if(strcmpy(head,"sudo")){
                // sudo action
                return 2;
            } else if(strcmpy(head,"exit")){
                // exit action
                return 3;
            } else {
                return -1;
            }
            break;
        case 1: 
            if (strcmpy(head,"add")){
                // read paramaters
            } else if(strcmpy(head,"new")){
                // read paramaters
            } else if(strcmpy(head,"del")){
                // read paramaters
            } else if(strcmpy(head,"select")){
                // read paramaters
            } else if(strcmpy(head,"search")){
                // read paramaters
            } else {
                return -1;
            }
            break;
        default: 
            return -1; 
            break;
    }
}