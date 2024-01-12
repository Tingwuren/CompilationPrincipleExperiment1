#include "lex.h"
#include "token.h"

int main() {
	FILE *inputfile, *outputfile;
    inputfile = fopen("input.txt", "r");
	if(inputfile == NULL) {
        printf("无法打开 input.txt 文件。\n");
        return 0;
    }
	outputfile = fopen("output.txt", "w");
	if (outputfile == NULL) {
        printf("无法打开 output.txt 文件。\n");
        fclose(inputfile);
        return 0;
    }
	// 初始化
	state= 0;
	initbuffer(inputfile);
	forward = &buffer1[0];
	if (*forward == EOF) {
		putfileinfor(outputfile);
		fclose(inputfile);
		fclose(outputfile);
		return 0;
	}
	lexemebegin = &buffer1[0];

    char *gettoken;
    while(endofthefile == false) {
		gettoken = lex(inputfile);
		printf("gettoken = %s\n", gettoken);
		if (gettoken != NULL) {
			fputs(gettoken, outputfile);
		}
    }

	putfileinfor(outputfile);
	fclose(inputfile);
	fclose(outputfile);

	// 清理符号表内存
    for (int i = 0; i < tablesize; i++) {
        free(table[i]);
    }
    free(table);
	return 0;
}

void putfileinfor(FILE *outputfile){
	static char fileinfor[50];
	sprintf(fileinfor, "/*********************************/\n");
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the line: %d\n", linenumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the identifier: %d\n", identifiernumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the keyword: %d\n", keywordnumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the number: %d\n", numbernumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the operator: %d\n", operatornumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the character: %d\n", characternumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the string: %d\n", stringnumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the file: %d\n", filenumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the separator: %d\n", separatornumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor,"The number of the error: %d\n", errornumber);
	fputs(fileinfor, outputfile);

	totalnumber = identifiernumber + keywordnumber +
		numbernumber + operatornumber +
		characternumber + stringnumber +
		filenumber + separatornumber + errornumber;
	sprintf(fileinfor,"The total number: %d\n", totalnumber);
	fputs(fileinfor, outputfile);

	sprintf(fileinfor, "/*********************************/\n");
	fputs(fileinfor, outputfile);
}

bool letter(char C) {
	if (C >= 'a' && C <= 'z') 
		return true;
	else if (C >= 'A' && C <= 'Z')
		return true;
	return false;
}

bool digit(char C) {
	if (C >= '0' && C <= '9')
		return true;
	return false;
}

int reserve(char token[]) {
    int i = 0;
    while (keywords[i] != NULL) {
        if (strcmp(token, keywords[i]) == 0)
            return i;
        i++;
    }
    return -1;
}


char* table_insert(char token[]) {
	// 从 table 中查找 token
	int i = 0;
    while (table != NULL && i < tablesize) {
        if (strcmp(token, table[i]) == 0)
            return table[i];
        i++;
    }
	// 扩大数组大小
	tablesize += 1;
	table = realloc(table, tablesize * sizeof(char*));

	// 将新字符串添加到数组末尾
    table[tablesize - 1] = malloc(strlen(token) + 1);
    strcpy(table[tablesize - 1], token);

	return table[tablesize - 1];
}

char* lex(FILE *inputfile) {
	do {
		switch (state) {
			// 初始状态，还没有开始识别任何词法单元。
			case 0:
				printf("state = 0\n");
				token[0]= '\0';
				get_char(inputfile);
				lexemebegin = forward;
				get_nbc(inputfile);

				if (letter(C) || C == '_') {
					state = 1;
					break;
				}
				else if (digit(C)){
					state = 2;
					break;
				}
				switch (C) {
				case '<':
					state = 8;
					break;
				case '>':
					state = 9;
					break;
				case '=':
					state = 10;
					break;
				case '+':
					state = 11;
					break;
				case '-':
					state = 12;
					break;
				case '*':
					state = 13;
					break;
				case '/':
					state = 14;
					break;
				case '\'':
					cat();
					state = 18;
					break;
				case '"':
					cat();
					state = 21;
					break;
				case '#':
					cat();
					state = 22;
					break;
				case '\n':
					state = 0;
					return gettuple("separator", "换行符");
					break;
				case '\t':
					state = 0;
					return gettuple("separator", "制表符");
				case '\r':
					state = 0;
					return gettuple("separator", "回车符");
				case '(':
					state = 0;
					return gettuple("separator", "(");
					break;
				case ')':
					state = 0;
					return gettuple("separator", ")");
					break;
				case ';':
					state = 0;
					return gettuple("separator", ";");
					break;
				case '\\':
					state = 0;
					return gettuple("separator", "\\");
				case '{':
					state = 0;
					return gettuple("separator", "{");
					break;
				case '}':
					state = 0;
					return gettuple("separator", "}");
					break;
				case '[':
					state = 0;
					return gettuple("separator", "[");
					break;
				case ']':
					state = 0;
					return gettuple("separator", "]");
					break;
				
				case ',':
					state = 0;
					return gettuple("separator", ",");
					break;
				case '!':
					state = 27;
					break;
				case ':':
					state = 0;
					return gettuple("separator", ":");
					break;
				case '|':
					state = 28;
					break;
				case '&':
					state = 29;
					break;
				default:
					return gettuple("error","识别出了错误字符");
					break;
				}
				break;
				
			// 正在识别一个标识符。
			case 1: // letter/_ letters
				printf("state = 1\n");
				cat(); // 将 C 连接到 token 后。
				get_char(inputfile);
				if (letter(C) || digit(C) || C == '_') {
					state = 1;
					break;
				} else {
					retract();
					state = 0;
					iskey = reserve(token);
					
					// 查询到关键字
					if (iskey != -1) {
						if (keywords[iskey] == "#include") {
							state = 23;
						}
						else if (keywords[iskey] == "#define") {
							state = 24;
						}
						return gettuple("keyword", keywords[iskey]);
					}
					else {
						char* identry = table_insert(token);
						return gettuple("identifier", identry);
					}
					break;
				}
			case 2: // digits
				printf("state = 2\n");
				cat();
				get_char(inputfile);
				if (digit(C)){
					state = 2;
					break;
				}
				switch (C) {
				case '.':
					state = 3;
					break;
				case 'e':
				case 'E':
					state = 5;
					break;
				default:
					retract();
					state = 0;	
					return gettuple("number",token);
					break;
				}
				break;
			case 3: // digits.
				printf("state = 3\n");
				cat();
				get_char(inputfile);
				if (digit(C))
					state =4;
				else {
					retract();
					state = 0;
					return gettuple("error", "digits.需要更多digit");
				}
				break;
			case 4: // digits.digit
				printf("state = 4");
				cat();
				get_char(inputfile);
				if (digit(C)) {
					state = 4;
					break;
				}
				switch (C) {
				case 'e':
				case 'E':
					state = 5;
					break;
				default:
					retract();
					state = 0;
					return gettuple("number", token);
					break;
				}
				break;
			case 5: // digits e/E || digits.digits. e/E
				printf("state = 5\n");
				cat();
				get_char(inputfile);
				if (digit(C)) {
					state = 7;
					break;
				}
				switch (C)
				{
				case '+':
				case '-':
					state = 6;
					break;
				default:
					retract();
					state = 0;
					return gettuple("error", "digits e/E || digits.digits. e/E后需要digit | + | -");
					break;
				}
			case 6: // digits e/E +/- || digits.digits. e/E +/-
				cat();
				get_char(inputfile);
				if (digit(C)){
					state = 7;
				} else {
					retract();
					return gettuple("error", "digits e/E +/- || digits.digits. e/E +/-后需要更多digit");
					state = 0;
				}
				break;
			case 7: // digits e/E digits || digits.digits. e/E digits
				printf("state = 7\n");
				cat();
				get_char(inputfile);
				if(digit(C)) {
					state = 7;
				} else {
					retract();
					state = 0;
					return gettuple("number", token);
				}
				break;
			case 8: // <
				printf("state = 8\n");
				cat();
				get_char(inputfile);
				switch (C) {
				case '=':
					cat();
					state = 0;
					return gettuple("operator", token);
					break;
				default:
					retract();
					state = 0;
					return gettuple("operator", token);
					break;
				}
				break;
			case 9: // >
				printf("state = 9\n");
				cat();
				get_char(inputfile);
				if (C == '=') {
					cat();
					state = 0;
					return gettuple("operator", token);
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token);
				}
				break;
			case 10: // =
				printf("state = 10\n");
				cat();
				get_char(inputfile);
				if (C == '=') {
					cat();
					state = 0;
					return gettuple("operator", token); // ==
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token); // =
				}
				break;
			case 11: // +
				printf("state = 11\n");
				cat();
				get_char(inputfile);
				if (C == '=') {
					cat();
					state = 0;
					return gettuple("operator", token); // +=
				}
				else if (C == '+') {
					cat();
					state = 0;
					return gettuple("operator", token); // ++
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token); // +
				}
				break;
			case 12: // -
				printf("state = 12\n");
				cat();
				get_char(inputfile);
				if (C == '=') {
					cat();
					state = 0;
					return gettuple("operator", token); // -=
				}
				else if (C == '-') {
					cat();
					state = 0;
					return gettuple("operator", token); // --
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token); // -
				}
				break;
			case 13: // *
				printf("state = 13\n");
				cat();
				get_char(inputfile);
				if (C == '=') {
					cat();
					state = 0;
					return gettuple("operator", token); // *=
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token); // *
				}
				break;
			case 14: // /
				printf("state = 14\n");
				cat();
				get_char(inputfile);
				if (C == '=') {
					cat();
					state = 0;
					return gettuple("operator", token); // /=
					break;
				}
				else if (C == '/') {
					state = 15;
					break;
				}
				else if (C == '*') {
					state = 16;
					break;
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token); // *
				}
				break;
			case 15: // //（单行注释）
				printf("state = 15\n");
				get_char(inputfile);
				if (C == '\n') {
					state = 0;
					linenumber++;
					return "\0";
					break;
				}
				else if (endofthefile == true) {
					state = 0;
					return "\0";
					break;
				}
				if (C != '\n') {
					state = 15;
					break;
				}
				break;
			case 16: // /* (多行注释)
				printf("state = 16\n");
				get_char(inputfile);
				if (C == '\n') {
					linenumber++;
				}
				while (C != '*') {
					get_char(inputfile);
					if (C == '\n') {
						linenumber++;
					}
				}
				if (C == '*') {
					state = 17;
					break;
				}
				break;
			case 17: // /* *
				printf("state = 17\n");
				get_char(inputfile);
				if (C == '\n') {
					linenumber++;
				}
				
				while (C == '*') {
					get_char(inputfile);
					if (C == '\n') {
						linenumber++;
					}
				}
				if (C != '/') {
					state = 16;
					break;
				}
				if (C == '/') {
					state = 0; // /* */
					return "\0"; // 自动略过多行注释
					break;
				}
				break;
			case 18: // '
				printf("state = 18\n");
				get_char(inputfile);
				if (C == '\\') {
					cat();
					state = 20;
					break;
				}
				else if (C == '\n' || C == '\'') {
					if (C == '\n') {
						linenumber++;
					}
					state = 0;
					return gettuple("error", "读取到了错误的字符");
					break;
				}
				cat();
				state =19;
				break;
			case 19: // 'char
				printf("state = 19\n");
				get_char(inputfile);
				if (C == '\'') {
					cat();
					state = 0;
					return gettuple("character", token);
					break;
				}
				else {
					retract();
					state = 0;
					return gettuple("error", "读取到了错误的字符");
					break;
				}
				break;
			case 20: // '反斜扛
				printf("state = 20\n");
				get_char(inputfile);
				if (C == '\'' || C == 'n' || C == 'r' || C == 't' || C == '\"' || C == '\\' || C == '0') {
					cat();
					state = 19;
					break;
				}
				else {
					return gettuple("error", "读取到了错误的转义字符");
					retract();
					state = 0;
					break;
				}
				break;
			case 21: // 识别了一个双引号
				printf("state = 21\n");
				get_char(inputfile);
				if (C == '"') {
					cat();
					state = 0;
					return gettuple("string", token);
					break;
				}
				else if (C == '\\') {
					cat();
					get_char(inputfile);
					if (C == '\"') {
						cat();
						state = 21;
						break;
					}
					cat();
					state = 21;
					break;
				}
				else if (endofthefile == false) {
					cat();
					state = 21;
					break;
				}
				else if (endofthefile == true) {
					return gettuple("error", "字符串需要匹配双引号");
					state = 0;
					return "\0";
					break;
				}
				break;
			case 22: // #
				printf("state = 22\n");
				get_char(inputfile);
				get_nbc(inputfile);
				state = 1;
				break;
			case 23: // #include
				printf("state = 23\n");
				token[0] = '\0';
				get_nbc(inputfile);
				if (C == '<') {
					cat();
					state = 25;
					break;
				}
				else if (C == '\"') {
					cat();
					state = 26;
					break;
				}
				else {
					retract();
					state = 0;
					return gettuple("error", "宏定义文件名称格式有误");
					break;
				}
				break;
			case 24: // #define
				printf("state = 24\n");
				token[0] = '\0';
				get_nbc(inputfile);
				state = 1;
				break;
			case 25: // #include <
				printf("state = 25\n");
				get_char(inputfile);
				if (C == '>') {
					cat();
					state = 0;
					return gettuple("file", token);
					break;
				}
				else if (C != '>') {
					cat();
					state = 25;
					break;
				}
				break;
			case 26: // #include "
				printf("state = 26\n");
				get_char(inputfile);
				if (C == '\"') {
					cat();
					state = 0;
					return gettuple("file", token);
				}
				else {
					cat();
					state = 26;
					break;
				}
				break;
			case 27: // !
				printf("state = 27\n");
				cat();
				get_char(inputfile);
				if (C == '=') {
					cat();
					state = 0;
					return gettuple("operator", token);
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token);
				}
				break;
			case 28: // |
				printf("state = 28\n");
				cat();
				get_char(inputfile);
				if (C == '|') {
					cat();
					state = 0;
					return gettuple("operator", token);
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token);
				}
				break;
			case 29: // &
				printf("state = 29\n");
				cat();
				get_char(inputfile);
				if (C == '&') {
					cat();
					state = 0;
					return gettuple("operator", token);
				}
				else {
					retract();
					state = 0;
					return gettuple("operator", token);
				}
				break;
		}
	} while(1);
}

char* gettuple(char *tag, char *attribute) {
	
	if (strcmp(tag, "identifier") == 0) {
		identifiernumber++;
	}
	if (strcmp(tag, "keyword") == 0) {
		keywordnumber++;
	}
	if (strcmp(tag, "number") == 0) {
		numbernumber++;
	}
	if (strcmp(tag, "operator") == 0) {
		operatornumber++;
	}
	if (strcmp(tag, "character") == 0) {
		characternumber++;
	}
	if (strcmp(tag, "string") == 0) {
		stringnumber++;
	}
	if (strcmp(tag, "file") == 0) {
		filenumber++;
	}
	if (strcmp(tag, "separator") == 0) {
		separatornumber++;
		if (strcmp(attribute, "换行符") == 0) {
			linenumber++;
			// printf("linenumber++\n");
		}
	}
	if (strcmp(tag, "error") == 0) {
		errornumber++;
		static char tuple[50];
		sprintf(tuple, "%s in line: %d, %s\n", tag, linenumber, attribute);
		return tuple;
	}
	static char tuple[50];
	sprintf(tuple, "<%s , %s>\n", tag, attribute);
	return tuple;
}

int SToI(char *token) {
	return atoi(token);
}

float SToF(char *token) {
	return atof(token);
}

void get_char(FILE *inputfile) {
	C = *forward;
	printf("get_char: '%c'form %p\n", C, forward);
	forward = changeforward(inputfile);

	// 如果取到了哨兵 EOF ，则自动取下一个字符。
	if (C == EOF && endofthefile == false) {
		C = *forward;
		forward = changeforward(inputfile);
	}
}

void get_nbc(FILE *inputfile) {
	while(C == ' '){
		get_char(inputfile);
	}
}
// 使forward指针指向上一个字符。
void retract() {
	if (forward == &buffer2[0]) {
		printf("forward retract to &buffer1[MAXLENGTH - 2] \n");
		notreloadbuffer = true;
		forward = &buffer1[MAXLENGTH - 2];
	}
	else if (forward == &buffer1[0]) {
		printf("forward retract to &buffer2[MAXLENGTH - 2] \n");
		notreloadbuffer = true;
		forward = &buffer2[MAXLENGTH - 2];
	}
	else {
		forward--;
	}
}

void cat() {
	if (token != "\0") {
		sprintf(token, "%s%c", token, C);
	}
	else {
		token[0] = '\0';
		sprintf(token, "%s%c", token, C);
	}
	printf("token = \"%s\" \n", token);
}

char *changeforward(FILE *inputfile) {
	forward++;
	if(*forward == EOF){

		printf("*forward == EOF,forward = %p \n", forward);

		if (forward == &buffer1[MAXLENGTH-1]) {
			if (notreloadbuffer == false) {
				loadbuffer2(inputfile);
			}

			forward = &buffer2[0];
			printf("forward = &buffer2[0] \n");
			if(notreloadbuffer == true) {
				notreloadbuffer = false;
			}
			if (*forward == EOF) {
				endofthefile = true;
				printf("\033[1;31m"); // 设置输出为红色
				printf("End Of the File  \n");
				printf("\033[0m"); // 重置输出颜色
			}
			return forward;
		}
		else if (forward == &buffer2[MAXLENGTH-1]) {
			if (notreloadbuffer == false) {
				loadbuffer1(inputfile);
			}

			forward = &buffer1[0];
			printf("forward = &buffer1[0] \n");
			if(notreloadbuffer == true) {
				notreloadbuffer = false;
			}
			if (*forward == EOF) {
				endofthefile = true;
				printf("\033[1;31m"); // 设置输出为红色
				printf("End Of the File  \n");
				printf("\033[0m"); // 重置输出颜色
			}
			return forward;
		}
		else {
			endofthefile = true;
			printf("\033[1;31m"); // 设置输出为红色
			printf("End Of the File  \n");
			printf("\033[0m"); // 重置输出颜色
			return forward;
		}
	}
	else {
		return forward;
	}
}

// 初始化缓冲区
void initbuffer(FILE *inputfile) {
	loadbuffer1(inputfile);
}
// 填充buffer1
void loadbuffer1(FILE *inputfile) {
	// 填充一号缓冲区
	int i;
	for (i = 0; i < MAXLENGTH - 1; i++) {
		char ch = fgetc(inputfile);
		if (ch == EOF) {
			buffer1[i] = EOF;
			break;
		}
		buffer1[i] = ch;
	}
	buffer1[MAXLENGTH-1] = EOF;

	printf("\033[1;34m"); // 设置输出为蓝色
	printf("/********** loadbuffer1 **********/\n");
	printf("&buffer1[0] = %p \n", &buffer1[0]);
	printf("buffer1 = \"%s\" \n", buffer1);
	if(buffer1[MAXLENGTH-1] == EOF){
		printf("buffer1[MAXLENGTH-1] == EOF \n");
	}
	printf("&buffer1[MAXLENGTH-1] = %p \n", &buffer1[MAXLENGTH-1]);
	printf("/*********************************/\n");
	printf("\033[0m"); // 重置输出颜色
}
// 填充buffer2
void loadbuffer2(FILE *inputfile) {
	// 填充二号缓冲区
	int i;
	for (i = 0; i < MAXLENGTH - 1; i++) {
		char ch = fgetc(inputfile);
		if (ch == EOF) {
			buffer2[i] = EOF;
			break;
		}
		buffer2[i] = ch;
	}
	buffer2[MAXLENGTH-1] = EOF;

	printf("\033[1;32m"); // 设置输出为绿色
	printf("/********** loadbuffer2**********/\n");
	printf("&buffer2[0] = %p \n", &buffer2[0]);
	printf("buffer2 = \"%s\" \n", buffer2);
	if(buffer2[MAXLENGTH-1] == EOF){
		printf("buffer2[MAXLENGTH-1] == EOF \n");
	}
	printf("&buffer2[MAXLENGTH-1] = %p \n", &buffer2[MAXLENGTH-1]);
	printf("/*********************************/\n");
	printf("\033[0m"); // 重置输出颜色
}