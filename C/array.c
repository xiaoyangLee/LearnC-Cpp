/*
用于说明数组的定义和参数传递
*/
#include <stdio.h>

/*
使用指针进行参数传递
*/
void print_Array(int *arr,int size)
{   
    int i;
    for(i = 0;i<size;i++)
    {
        printf("%d ",arr[i]);
    }
}
/*
如果使用数组作为参数传递,函数定义如下,函数体如上,就不再写了。
*/
void print_Array_(int array[],int size);
int main()
{
    //定义一个数组,存放5个元素
    int array_a[5] = {1,2,3,4,5};
    //还可以这样定义,数组长度为5
    int array_b[] = {5,6,7,8,9,10};
    //求出数组array_a的长度
    int len_a = sizeof(array_a)/sizeof(array_a[0]);
    //求出数组array_b的长度
    int len_b = sizeof(array_b)/sizeof(array_b[0]);

    //打印array_a[5]的数组
    printf("长度为 %d 的array_a[5]数组的值为:\n",len_a);
    print_Array(array_a,len_a);
    printf("\n");
    //打印array_b[]的数组
    printf("长度为 %d 的array_b[]数组的值为:\n",len_b);
    print_Array(array_b,len_b);

    //定义一个二维数组

}