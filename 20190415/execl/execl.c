#include <func.h>
int main()
{
	printf("hello\n");
	execl("./add","add","3","4",NULL);
	printf("you can't see me\n");
	return 0;
}
