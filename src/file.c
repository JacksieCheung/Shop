#include<stdio.h>
#include<string.h>
#include "list.h"
#include "errno.h"

extern listHead* DB;

int getLengthOfInt(int tmp){
    int count = 0;
    while(tmp>0){
        tmp/=10;
        count++;
    }
    return count;
}

void changeIntToString(char* str,int tmp,int len){
    for(int i=len-1;i>=0;i--){
        str[i]=tmp%10+'0';
        tmp/=10;
    }
    str[len]='\0';
}

int changeStringToInt(char* str){
    int sum=0;
    for (int i=0;i<strlen(str);i++){
        sum = sum*10+str[i]-'0';
    }
    return sum;
}

// save the data
Status Save(){
    FILE* fp = NULL;
    fp = fopen("../database/data.csv","w+");
    if (fp==NULL) return -1; // error

    listNode* tmp = DB->Next;
    while(tmp!=NULL){
        int len = getLengthOfInt(tmp->Sum);
        char sum[len+1];
        changeIntToString(sum,tmp->Sum,len);
        fwrite(tmp->Name,1,strlen(tmp->Name),fp);
        fwrite(",",1,1,fp);
        fwrite(sum,1,strlen(sum),fp);
        fwrite(",\n",1,2,fp);
        tmp = tmp->Next;
    }
    fclose(fp);
    return OK;
}

// read the data
Status Read(){
    // clear the DB
    ResetDB();

    // clear the TOPARR
    ResetTOPARR();

    // clear DelPool
    ResetDelPool();

    // read
    FILE *fp = NULL;
    char *line;
    char *record;
    char buffer[1024];
    fp = fopen("../database/data.csv", "at+");
    if (fp== NULL) return -1;

    int status = 0;    
    while ((line = fgets(buffer, sizeof(buffer), fp))!=NULL){
        record = strtok(line, ",");
        // set DB
        altRequest tmp;
        tmp.Name = record;
        // get sum
        record = strtok(NULL, ",");
        tmp.Sum = changeStringToInt(record);

        if(AddDataNotExist(&tmp)<0){
            ResetDB();
            ResetTOPARR();
            return -1;
        }
    }
    fclose(fp);
    return OK;
}