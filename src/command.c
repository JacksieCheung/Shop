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
            head[index] = '\0';
            return 1; // 有参数
        }
        if (line[index]==';'){
            for (int i=0;i<index;i++) head[i]=line[i];
            head[index] = '\0';
            return 0; // 无参数
        }
    }

    return -1;// 指令出错
}

// 获取后缀 desc/asc
int GetTail(char* body,int len){
    if (len>4) return -1;
    char tail[5]={0};
    for (int i=0;i<len;i++){
        tail[i]=body[i];
    }
    tail[len]='\0';
    if (strlen(tail)==4&&!strcmp(tail,"desc")) return 0;
    if (strlen(tail)==3&&!strcmp(tail,"asc")) return 1;
    return -1;
}

// 解析 select 指令
int GetSelectParam(char* body,int len,char** goods,int* index,int* order){
    *index = 0; // index 归0

    for(int i=0;i<len;i++){
        if (body[i]==' '||body[i]==',') {
            continue;
        }else if (body[i]=='\'') {
            // 开始读取 goods
            int j=0;
            i++;
            while(j<50&&i<len){
                if (body[i]=='\'') break;
                goods[*index][j]=body[i];
                i++;
                j++;
            }
            i++;
            goods[*index][j]='\0';
            (*index)++;
        }else{
            if (i+3==len||i+4==len){
                int status = GetTail(body+i,len-i);
                if (status<0) return status;
                *order = status;
                return 1;
            }
            return -1; // 错误
        }
    }
    if (*index==0) return -1;
    return 1;
}

// 解析 add 等指令
int GetParam(char* body,int len,char** goods,int* sum,int* index){
    *index = 0; // index 归0
    for(int i=0;i<len;i++){
        if (body[i]==' ') {
            continue;
        }else if (body[i]=='\'') {
            // 开始读取 goods
            int j=0;
            i++;
            while(j<50&&i<len){
                if (body[i]=='\'') break;
                goods[*index][j]=body[i];
                i++;
                j++;
            }
            i++;
            goods[*index][j]='\0';
            // 开始读取数字
            sum[*index]=0; // 初始化
            while(body[i]==' ') i++; // 跳过空格
            while(i<len){
                if(body[i]>='0'&&body[i]<='9'){
                    sum[*index]=sum[*index]*10+body[i]-'0';
                    i++;
                } else if(body[i]==',') {
                    break;
                } else {
                    return -1;
                }
            }
            (*index)++;
        }else{
            return -1; // 错误
        }
    }
    if (*index==0) return -1;
    return 1;
}

// 解析命令
int ResolveCommand(char* line,char** goods,int* sum,int* index,int* order){
    int len = strlen(line);
    char head[7];
    int status = 0;
    int headLen = 0;
    *order = -1; // order 归 "0"

    if (line[len-1]!=';') return -1;

    switch(GetCommandHead(line,len,head)){
        case 0: 
            headLen = strlen(head);
            if (headLen==4&&!strcmp(head,"save")){
                // save action
                return 0;
            } else if(headLen==4&&!strcmp(head,"read")){
                // read action
                return 1;
            } else if(headLen==4&&!strcmp(head,"sudo")){
                // sudo action
                return 2;
            } else if(headLen==4&&!strcmp(head,"exit")){
                // exit action
                return 3;
            } else if(headLen==4&&!strcmp(head,"menu")){
                return 4;  
            } else if(headLen==6&&!strcmp(head,"search")){
                return 10;    
            }else {
                return -1;
            }
            break;
        case 1:
            headLen = strlen(head);
            if (headLen==3&&!strcmp(head,"add")){
                // read paramaters
                status = GetParam(line+3,len-4,goods,sum,index);
                if (status<0) return status;
                return 5;
            } else if(headLen==3&&!strcmp(head,"new")){
                // read paramaters
                status = GetParam(line+3,len-4,goods,sum,index);
                if (status<0) return status;
                return 6;
            } else if(headLen==3&&!strcmp(head,"del")){
                // read paramaters
                status = GetParam(line+3,len-4,goods,sum,index);
                if (status<0) return status;
                return 7;
            } else if(headLen==6&&!strcmp(head,"select")){
                // read paramaters
                status = GetSelectParam(line+6,len-7,goods,index,order);
                if (status<0) return status;
                return 8;
            } else if(headLen==6&&!strcmp(head,"search")){
                // read paramaters
                status = GetTail(line+7,len-8);
                if (status<0) return status;
                *order = status;
                return 9;
            } else {
                return -1;
            }
            break;
        default: 
            return -1; 
            break;
    }
    return -1;
}