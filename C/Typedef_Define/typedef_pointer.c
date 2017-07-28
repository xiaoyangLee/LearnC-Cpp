#include <stdio.h>
/*
* 使用typedef定义指针
*/
int main(void)
{
    typedef char *pStr; //typedef将char* 重命名为pStr

    char string[4]="abc";
    const char *p1=string;//限定*p1 p1是可变的
    const pStr p2=string; //限定p2 p2不可变
    p1++;
    p2++;//报错,p2是只读变量，只不过类型是我们自己定义的
    return 0;
}
