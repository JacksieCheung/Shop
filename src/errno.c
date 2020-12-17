#include "errno.h"
#include<stdio.h>

void HandleStatus(Status stat){
    printf("System:Error_Message:");
    switch(stat){
        case ERR_COMMAND: 
            printf("Command wrong, you can type 'menu;' to check corret comand.");
            break;
        case ERR_SUDO: 
            printf("Access denied, you can type 'sudo' to get right.");
            break;
        case ERR_NODENOTFOUNDDB:
               printf("Can not find the node in DB.");
            break;
        case ERR_NODEEXISTEDDB: 
            printf("The node is already existed.");
            break;
        case ERR_SUMOVERFLOW: 
            printf("Sum is overflowed.");
            break;
        case ERR_EMPTYDB: 
            printf("DB is empty.");
            break;
        default:
            printf("Unkown error.");
            break;
    }
    printf("\n");
}