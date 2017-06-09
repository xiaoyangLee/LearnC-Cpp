#include <stdio.h>
/*
uppertolower实现大写转换为小写
*/
void uppertolower(char str[])
{
	//用strlen(str)可以求出字符数组长度
	for (int i = 0; i <strlen(str); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 32;
		}
		printf("%c", str[i]);
	}
	printf("\n");
}
/*
lowertoupper函数小写转大写
*/
void lowertoupper(char str[])
{
	//用strlen(str)可以求出字符数组长度
	for (int i = 0; i <strlen(str); i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 32;
		}
		printf("%c", str[i]);
	}
	printf("\n");
}


void main()
{
	char str[30],str2[30];
	printf("请输入一字符串:\n");
	//scanf("%s", str);
	gets(str);
	printf("大写转小写为:\n");
	uppertolower(str);
	printf("请输入一字符串:\n");
	gets(str2);
	printf("小写转大写为:\n");
	lowertoupper(str2);
	
}