#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int state; // 当前状态指示
char C; // 存放当前读入的字符
int iskey= -1; // 表示识别出的单词是用户自定义标志符，否则识别出的单词是关键字，其值为关键字的记号。
char token[100] = ""; // 存放当前正在识别的单词字符串
char *lexemebegin; // 指向输入缓冲区中当前单词的开始位置
char *forward; // 向前指针
#define MAXLENGTH 10 // 缓冲区大小
char buffer1[MAXLENGTH] = "\0";
char buffer2[MAXLENGTH] = "\0"; // 缓冲区大小是相同的两个4096字节大小的数组 
void cat();// 把C中的字符连接到token后面
bool letter(char C); // 判断C中的字符是否是字母，是则返回true，否则返回false
bool digit(char C); // 判断C中的字符是否是数字，是则返回true，否则返回false
void retract(); // 向前指针forward后退一个字符
bool notreloadbuffer = false;
int reserve(char token[]); // 根据token中的单词查关键字表，若token是关键字则返回该关键字的记号，否则返回“-1”
char* table_insert(char token[]); // 将识别出来的用户自定义标志符插入符号表，返回该单词在符号表中的指针位置
void initbuffer(FILE *inputfile);
void loadbuffer1(FILE *inputfile);
void loadbuffer2(FILE *inputfile);
char* lex(FILE *inputfile);
char *changeforward(FILE *inputfile);
bool endofthefile = false;
void get_char(FILE *inputfile);
void get_nbc(FILE *inputfile);
// 将token中的字符串转换为整数
int SToI(char *token);
// 将token中的字符串转换为浮点数
float SToF(char *token);
char* gettuple(char *tag, char *attribute);

void putfileinfor(FILE *outputfile);
int linenumber = 1;
int totalnumber = 0;
int identifiernumber = 0;
int keywordnumber = 0;
int numbernumber = 0;
int operatornumber = 0;
int characternumber = 0;
int stringnumber = 0;
int filenumber = 0;
int separatornumber = 0;
int errornumber = 0;


