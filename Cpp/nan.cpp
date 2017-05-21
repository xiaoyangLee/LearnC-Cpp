#include<stdio.h>
int main()
{
	int i;
	double s,t;
	s=0;
	t=1;
	for(i=1;i<=20;i++)
	{
		t=t*i;
		s=s+t;
	}
	printf("%d",s);
	return 0;
}



