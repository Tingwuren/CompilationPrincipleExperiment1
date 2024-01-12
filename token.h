// keyword 关键字标记
char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while", "#include", "#define", NULL
};
// 符号表初始化
char **table = NULL;
int tablesize = 0; // 符号表大小