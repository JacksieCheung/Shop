#include "command.h"
#include "errno.h"
#include<string.h>
#include<stdio.h>

// 拆分字符串
Status GetCommandHead(char* line,int len,char* head){
    // 记录遇到的第一个 ';' 或' ';
    int index = 0;
    for(;index<len;index++){
        if (line[index]==' '){
            for(int i=0;i<index;i++) head[i]=line[i];
            head[index] = '\0';
            return OK; // 有参数
        }
        if (line[index]==';'){
            for (int i=0;i<index;i++) head[i]=line[i];
            head[index] = '\0';
            return WITHOUT_PARAM; // 无参数
        }
    }

    return ERR_GETCOMMANDHEAD;// 指令出错
}

// 获取后缀 desc/asc
Status GetTail(char* body,int len){
    if (len>4) return NO_TAIL;
    char tail[5]={0};
    for (int i=0;i<len;i++){
        tail[i]=body[i];
    }
    tail[len]='\0';
    if (strlen(tail)==4&&!strcmp(tail,"desc")) return DESC;
    if (strlen(tail)==3&&!strcmp(tail,"asc")) return ASC;
    return NO_TAIL;
}

// 解析 select 指令
Status GetSelectParam(char* body,int len,char** goods,int* index,int* order){
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
                *order = GetTail(body+i,len-i);
                return OK;
            }
            return ERR_COMMAND; // 错误
        }
    }
    if (*index==0) return ERR_COMMAND;
    return OK;
}

// 解析 add 等指令
Status GetParam(char* body,int len,char** goods,int* sum,int* index){
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
                    return ERR_COMMAND;
                }
            }
            (*index)++;
        }else{
            return ERR_COMMAND; // 错误
        }
    }
    if (*index==0) return ERR_COMMAND;
    return OK;
}

// 解析命令
Status ResolveCommand(char* line,char** goods,int* sum,int* index,int* order){
    int len = strlen(line);
    char head[7];
    int status = 0;
    int headLen = 0;
    *order = 0; // order 归 "0"

    if (line[len-1]!=';') return ERR_COMMAND;

    Status stat = GetCommandHead(line,len,head);

    switch(stat){
        case WITHOUT_PARAM: 
            headLen = strlen(head);
            if (headLen==4&&!strcmp(head,"save")){
                // save action
                return SAVE;
            } else if(headLen==4&&!strcmp(head,"read")){
                // read action
                return READ;
            } else if(headLen==4&&!strcmp(head,"sudo")){
                // sudo action
                return SUDO;
            } else if(headLen==4&&!strcmp(head,"exit")){
                // exit action
                return EXIT;
            } else if(headLen==4&&!strcmp(head,"menu")){
                return MENU;  
            } else if(headLen==6&&!strcmp(head,"search")){
                return SEARCH;    
            }else {
                return ERR_COMMAND;
            }
            break;
        case OK:
            headLen = strlen(head);
            if (headLen==3&&!strcmp(head,"add")){
                // read paramaters
                status = GetParam(line+3,len-4,goods,sum,index);
                if (status<0) return status;
                return ADD;
            } else if(headLen==3&&!strcmp(head,"new")){
                // read paramaters
                status = GetParam(line+3,len-4,goods,sum,index);
                if (status<0) return status;
                return NEW;
            } else if(headLen==3&&!strcmp(head,"del")){
                // read paramaters
                status = GetParam(line+3,len-4,goods,sum,index);
                if (status<0) return status;
                return DEL;
            } else if(headLen==6&&!strcmp(head,"select")){
                // read paramaters
                status = GetSelectParam(line+6,len-7,goods,index,order);
                if (status<0) return status;
                return SELECT;
            } else if(headLen==6&&!strcmp(head,"search")){
                // read paramaters
                *order = GetTail(line+7,len-8);
                return SEARCH;
            } else {
                return ERR_COMMAND;
            }
            break;
        default: 
            return ERR_COMMAND; 
            break;
    }
    return NONE;
}