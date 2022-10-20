#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

/*
 * cdec.c 该程序为《C 编程专家》 第三章的实例， 也是原生的 C 语言声明解析函数。
 * cdecl 为 C Declaration 的缩写， 该程序可以用于解析一个变量的声明。
 * 
 * eg:
 *   1. char *(*c[10])(int **p);
 *   2. char * const * (*next)();
 * 
 * 2022-10-18 02:58:55
 * Coding by Miaocf
 */

#define MAX_TOKENS      100
#define MAX_TOKENS_LEN  64

enum typeTag {
    IDENTIFIER, // 标识符
    QUALIFIER,  // 限定符
    TYPE        // 类型
};

struct token {
    char type;
    char string[MAX_TOKENS_LEN];
};

int top = -1;
struct token stack[MAX_TOKENS_LEN];
struct token this;

#define pop stack[top--]
#define push(s)  stack[++top] = s

// 推断标识符的类型
enum typeTag classifyString(void) 
{
    char *s = this.string;
    if (!strcmp(s, "const")) {
        strcpy(s, "read-only");
        return QUALIFIER;
    }
    if (!strcmp(s, "volatile")) {
        return QUALIFIER;
    }
    if (!strcmp(s, "void") || !strcmp(s, "char") || !strcmp(s, "signed") ||
        !strcmp(s, "shrot") || !strcmp(s, "int") || !strcmp(s, "long") ||
        !strcmp(s, "float") || !strcmp(s, "double") || !strcmp(s, "struct") ||
        !strcmp(s, "union") || !strcmp(s, "enum") || !strcmp(s, "unsigned")) {
        return TYPE;
    }
    return IDENTIFIER;
}

void getToken(void) 
{
    char *p = this.string;

    // 跳过空白符
    while ((*p = getchar()) == ' ');

    if (isalnum(*p)) {
        // 读入标识符以 A-Z 或 0 - 9 开头
        while (isalnum(*++p = getchar()));
        ungetc(*p, stdin);
        *p = '\0';
        this.type = classifyString();
        return;
    }

    if (*p == '*') {
        strcpy(this.string, "pointer to");
        this.type == '*';
        return;
    }
    this.string[1] = '\0';
    this.type = *p;
    return;
}

// 理解所有分析过程的代码段
void readToFirstIdentifier()
{
    getToken();
    while (this.type != IDENTIFIER) {
        push(this);
        getToken();
    }
    printf("%s is ", this.string);
    getToken();
    return;
}

void dealWithArrays() 
{
    while (this.type == '[') {
        printf("arrar ");
        getToken();  // 数字 or ']'
        if (isdigit(this.string[0])) {
            printf("0..%d ", atoi(this.string) - 1);
            getToken();  // 读取']'
        }
        getToken();      // 读取 ']' 之后的一个标记
        printf("of ");
    }
}

void dealWithFunctionArgs()
{
    while (this.type != ')') {
        getToken();
    }
    getToken();
    printf("function returning");
}

void dealWithPointers() 
{
    while (stack[top].type == '*') {
        printf("%s ", pop.string);
    }
}

void dealWithDeclarator()
{
    // 处理标识符之后可能存在的数组 / 函数。
    switch (this.type) {
        case '[': 
            dealWithArrays();
            break;
        case '(':
            dealWithFunctionArgs();
            break;
    }

    dealWithPointers();

    // 处理在读入到标识符之前压入到堆栈中的符号
    while (top >= 0) {
        if (stack[top].type == '(') {
            pop;
            getToken();  // 读取 ')' 之前的符号
            dealWithDeclarator();
        } else {
            printf("%s ", pop.string);
        }
    }
}

int main() {
    printf("Input a declation:\n");
    
    // 将标记压入队长中，知道遇到标识符。
    readToFirstIdentifier();
    dealWithDeclarator();
    printf("\n");
    return 0;
}