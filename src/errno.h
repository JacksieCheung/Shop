// 最基本的情况
#define OK 1
#define Status int
#define Error -1

// --status--
// 不是错误的特殊情况
#define WITHOUT_PARAM 3
#define NO_TAIL 0
#define DESC 5
#define ASC 6
#define EXIT_NOW 7
#define ERR_EMPTYDB 8

// --none--
#define NONE 0

// --error--
// 错误
#define ERR_GETCOMMANDHEAD -2
#define ERR_COMMAND -3
#define ERR_SUDO -4
#define ERR_MALLOC -5
#define ERR_NODENOTFOUNDPOOL -6
#define ERR_EMPTYGOODS -7
#define ERR_NODENOTFOUNDDB -8
#define ERR_NODEEXISTEDDB -9
#define ERR_SUMOVERFLOW -10

// --action--
#define SAVE 10
#define READ 11
#define SUDO 12
#define EXIT 13
#define MENU 14
#define SEARCH 15
#define ADD 16
#define NEW 17
#define DEL 18
#define SELECT 19

// --func--
void HandleStatus(Status stat);