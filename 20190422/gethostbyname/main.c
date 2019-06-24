#include <func.h>
int main(int argc,char *argv[])
{
	struct hostent *p;
	p=gethostbyname(argv[1]);

	printf("host_name: %s\n",p->h_name);
	for(int i=0;p->h_aliases[i]!=NULL;i++){
		printf("alias    : %s\n",p->h_aliases[i]);
	}
	printf("addrtype : %d\n",p->h_addrtype);
	printf("length   : %d\n",p->h_length);

	char ip[16]={0};
	for(int i=0;p->h_addr_list[i]!=NULL;i++){
		bzero(&ip,sizeof(ip));
		inet_ntop(p->h_addrtype,p->h_addr_list[i],ip,sizeof(ip));
		printf("address  : %s\n",ip);
	}
	return 0;
}
