#include <func.h>
int main()
{
	FILE *fp=popen("./scanf","w");
	char buf[128]="3 4";
	fputs(buf,fp);
	pclose(fp);
	return 0;
}
