#include <stdio.h>
/*
uppertolowerʵ�ִ�дת��ΪСд
*/
void uppertolower(char str[])
{
	//��strlen(str)��������ַ����鳤��
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
lowertoupper����Сдת��д
*/
void lowertoupper(char str[])
{
	//��strlen(str)��������ַ����鳤��
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
	printf("������һ�ַ���:\n");
	//scanf("%s", str);
	gets(str);
	printf("��дתСдΪ:\n");
	uppertolower(str);
	printf("������һ�ַ���:\n");
	gets(str2);
	printf("Сдת��дΪ:\n");
	lowertoupper(str2);
	
}