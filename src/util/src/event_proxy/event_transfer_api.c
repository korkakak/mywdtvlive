/*
 *Copyright (C) 2003-2006, Alpha Networks Inc. All rights reserved.
 *
 *$Id: event_transfer_api.c,v 1.1.2.5 2007-07-25 13:05:01 divers_dai Exp $
 *vim:cindent:ts=8 sts=8 sw=8
 */
#include <stdio.h>
#include <memory.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include  <termios.h>
#include  <unistd.h>
#include  <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "event_transfer_api.h"

#define FILE_MODE   (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

//#define DBG_TRANSFER_INFO
//create the communicate channel:two channel for each peer.one for read another for write.
unsigned char transfer_fifo_init(char *fifo_read,char *fifo_write,int *readfd,int *writefd,int *dummy_rdfd,int *dummy_wrfd)
{
	if ((fifo_read == NULL)||(fifo_write == NULL)||(readfd == NULL)||(writefd == NULL)||(dummy_rdfd == NULL)||(dummy_wrfd == NULL)){
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}
	#ifdef DBG_TRANSFER_INFO
	fprintf(stderr,"%s:%d,:fifo read:%s,fifo write:%s\n",__FUNCTION__,__LINE__,fifo_read,fifo_wriet);
	#endif
	//to create the two fifo:name pipe;
	if((mkfifo(fifo_read, FILE_MODE) < 0) && (errno != EEXIST)){
		fprintf(stderr,"%s:%d:can't creat fifo:%s\n",__FUNCTION__,__LINE__, fifo_read);
		return EVENT_PROXY_FAIL;
	}

	if((mkfifo(fifo_write, FILE_MODE) < 0) && (errno != EEXIST)){
		fprintf(stderr,"%s:%d:can't creat fifo:%s\n",__FUNCTION__,__LINE__, fifo_write);
		return EVENT_PROXY_FAIL;
	}

	
	*readfd = open(fifo_read, O_RDONLY|O_NDELAY);
	fcntl( *readfd, F_SETFD, FD_CLOEXEC );

	*dummy_rdfd = open(fifo_write, O_RDONLY|O_NDELAY);
	fcntl( *dummy_rdfd, F_SETFD, FD_CLOEXEC );

	*writefd = open(fifo_write,  O_WRONLY|O_NDELAY);
	fcntl( *writefd, F_SETFD, FD_CLOEXEC );

	*dummy_wrfd = open(fifo_read,O_WRONLY|O_NDELAY);
	fcntl( *dummy_wrfd, F_SETFD, FD_CLOEXEC );

	return EVENT_PROXY_SUCCESS;

}


//close the fd's pipe.
unsigned char transfer_fifo_close(int fd)
{
	if(fd >= 0)
		close(fd);
	return EVENT_PROXY_SUCCESS;

}


//send the event_size's event to the writefd's pipe.
int transfer_fifo_write(int writefd, void *event,unsigned int event_size)
{
	int ret;
	if((ret=write(writefd, event, event_size)) <= 0)
		return -1;
	return ret;
}

//read the event_size data len from the pipe.
int transfer_fifo_read(int readfd, void *event,unsigned int event_size)
{
	int ret;
	ret = read(readfd,event,event_size);
	if (ret <=0)
		return -1;
	return ret;
}

//poll the read event.
unsigned char transfer_inquire_read(int readfd,unsigned char *result)
{
	fd_set         rset;
	struct timeval tv;


	if((readfd<0)||(result == NULL)){	
		fprintf(stderr,"***%s:%d parameter***\n",__FUNCTION__,__LINE__);
		return EVENT_PROXY_PARAERR;
	}

	FD_ZERO(&rset);
	FD_SET(readfd, &rset);
	tv.tv_sec = tv.tv_usec = 0;
	if (select(readfd + 1, &rset, NULL, NULL, &tv) == 1){
		*result = 1;
		return EVENT_PROXY_SUCCESS;
	}else{
		*result = 0;
		return EVENT_PROXY_SUCCESS;
	}



}

//clear the data in the pipe.
unsigned char transfer_fifo_clear_all_data(int readfd)
{
	if(readfd >= 0){
		fd_set         rset;
		struct timeval tv;
		unsigned char key_value;
		int rslt;
		for(;;){

			FD_ZERO(&rset);
			FD_SET(readfd, &rset);
			tv.tv_sec = tv.tv_usec = 0;
			rslt = select(readfd + 1, &rset, NULL, NULL, &tv);
			if (rslt > 0){
				read(readfd, &key_value, sizeof(key_value));
			}else
				return 0;
		}
	}
	#ifdef DBG_TRANSFER_INFO
	fprintf(stderr,"%s:%d end\n",__FUNCTION__,__LINE__);
	#endif
	return 0;

}

