#include "command.h"
#include<string.h>
#include<stdio.h>

// 拆分字符串
int GetCommandHead(char* line,int len,char* head){
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
int GetAddParam(char* body,int len){
    char goods[50][51]={0};
    int sum[50]={0};
    int index = 0;
    for(int i=0;i<len;i++){
        if (body[i]==' ') {
            continue;
        }else if (body[i]=='\'') {
            // 开始读取 goods
            int j=0;
            i++;
            while(j<50&&i<len){
                if (body[i]=='\'') break;
                goods[index][j]=body[i];
                i++;
                j++;
            }
            i++;
            goods[index][j+1]='\0';
            // 开始读取数字
            while(body[i]==' ') i++; // 跳过空格
            while(i<len){
                if(body[i]>='0'&&body[i]<='9'){
                    sum[index]=sum[index]*10+body[i]-'0';
                    i++;
                } else if(body[i]==',') {
                    break;
                } else {
                    return -1;
                }
            }
            index++;
        }else{
            printf("2");
            return -1; // 错误
        }
    }
    for (int i=0;i<index;i++){
        printf("%s    ",goods[i]);
        printf("%d\n",sum[i]);
    }
    return 1;
}

// 解析命令
int ResolveCommand(char* line){
    int len = strlen(line);
    char head[7];

    if (line[len-1]!=';') return -1;

    switch(GetCommandHead(line,len,head)){
        case 0: 
            if (strcmp(head,"save")){
                // save action
                return 0;
            } else if(strcmp(head,"read")){
                // read action
                return 1;
            } else if(strcmp(head,"sudo")){
                // sudo action
                return 2;
            } else if(strcmp(head,"exit")){
                // exit action
                return 3;
            } else {
                return -1;
            }
            break;
        case 1: 
            if (strcmp(head,"add")){
                // read paramaters
                return GetAddParam(line+3,len-4);
            } else if(strcmp(head,"new")){
                // read paramaters
            } else if(strcmp(head,"del")){
                // read paramaters
            } else if(strcmp(head,"select")){
                // read paramaters
            } else if(strcmp(head,"search")){
                // read paramaters
            } else {
                return -1;
            }
            break;
        default: 
            return -1; 
            break;
    }
    return 0;
}