// --func--
int GetCommandHead(char* line,int len,char* head);
int ResolveCommand(char* line,char** goods,int* sum,int* index,int* order);
int GetAddParam(char* body,int len,char** goods,int* sum,int* index);
int GetSelectParam(char* body,int len,char** goods,int* index,int* order);
int GetTail(char* body,int len);