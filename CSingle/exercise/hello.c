#include <stdio.h>

// Ctrl + Shift + B 编译
// F4 编译 + 运行
// F5 调试
int main() 
{
    char name[10]; 
    printf("Input your name: ");
    scanf("%s", name);
    printf("Hello %s, this is your world!", name);
    return 0;
}