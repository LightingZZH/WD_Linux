#include "function.h"

int tranFile(int new_fd)
{
	Train train;
	
	train.dataLen=strlen(FILENAME);
	strcpy(train.buf,FILENAME);
	send(new_fd,&train,4+train.dataLen,0);
	int fd=open(FILENAME,O_RDONLY);
	ERROR_CHECK(fd,-1,"open");

	while((train.dataLen=read(fd,train.buf,sizeof(train.buf))))
	{
		send(new_fd,&train,4+train.dataLen,0);
	}

	send(new_fd,&train,4,0);
	return 0;
}
