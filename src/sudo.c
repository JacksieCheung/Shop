#include<stdio.h>
#include<string.h>
#include "errno.h"

int Sudo = 0;

const char* password = "root";

Status checkPassword(char* tmp){
    if (strlen(tmp)==strlen(password)
    &&!strcmp(tmp,password)){
        return 1;
    }
    return -1;
}


Status SetSudo(){
    // 验证密码
    if(Sudo) return -1;
    char buff[100];
    printf("System:input your password=>");
    fgets(buff,99,stdin);
    buff[strlen(buff)-1]='\0';
    if (checkPassword(buff)>0) Sudo = 1;
    return OK;
}

Status IfExist(){
    if(Sudo) {
        Sudo = 0;
        return 0;
    }
    return 10;
}