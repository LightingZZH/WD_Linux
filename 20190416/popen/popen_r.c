#include <func.h>
int main()
{
	FILE *fp=popen("./print","r");
	char buf[128]={0};
	fgets(buf,sizeof(buf),fp);
	printf("New : %s\n",buf);
	return 0;
}
