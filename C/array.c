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

/*
使用一维数组指针传递二维数组,必须制定一维数组的长度,如果没有指定,函数编译通过,但是无法调用。
*/
void print_Array_twodimen(int (*pArray)[3],int size)
{
    int i,j;
    for(i=0;i<size;i++){
        for(j=0;j<3;j++){
            printf("%d ",pArray[i][j]);
        }
        printf("\n");
    }
}
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

    //定义一个二维数组,第二维的长度必须指定
    //因为初始化的时候，第二维的数字代表分配内存的长度，第一维的数字代表分配内存倍数；倍数可以让机器去数，但长度没有的话就不好开辟空间了。
    int array_two_dimen[][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
    };
    //求出二维数组的长度,行列数
    int len_row = sizeof(array_two_dimen)/sizeof(array_two_dimen[0]);
    int len_col = sizeof(array_two_dimen[0])/sizeof(array_two_dimen[0][0]);
    printf("\n");
    printf("二维数组行的长度为 %d ",len_row);
    printf("\n");
    printf("二维数组列的长度为 %d ",len_col);
    

}