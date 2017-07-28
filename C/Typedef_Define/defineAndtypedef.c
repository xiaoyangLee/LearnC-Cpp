#include <stdio.h>
//使用define规定运算要用括号，否则会输出错误，这里输出36
#define f(x) x*x
//正确写法为define f(x) (x*x)
//#define f(x) (x*x)
/*
*讲解define和typedef的差异和用法
*/
int main(void)
{
    int a = 6,b = 2,c;
    c = f(a) / f(b);
    //正确答案应该是输出9，define定义不正确会输出36
    printf("%d\n", c);
    return 0;
}