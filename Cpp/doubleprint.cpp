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
	
	//如果要打印double类型的数据，用%f,使用%d只会打印出0.
	printf("%f",s);
	return 0;
}



