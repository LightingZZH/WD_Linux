#include <stdio.h>
int multi(int,int);
int main()
{
	int a=3;
	int b=4;
	int ret=0;
	ret=multi(a,b);
	printf("%d * %d = %d\n",a,b,ret);
	return 0;
}
