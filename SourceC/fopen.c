#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define PERMS 0666
#define BUFSZ 1024
#define OPEN_MAX 20
#define EOF (-1)


typedef struct _iobuf {
	int cnt;
	char *ptr;
	char *base;
	struct {
		unsigned int _READ : 1;
		unsigned int _WRITE : 1;
		unsigned int _EOF : 1;
		unsigned int _ERR : 1;
		unsigned int _UNBUF : 1;
	} flags;
	int fd;
} FILE;

FILE _iob[OPEN_MAX] = {
	{0, (char *) 0, (char *) 0, {1,0,0,0,0}, 0},
	{0, (char *) 0, (char *) 0, {0,1,0,0,0}, 1},
	{0, (char *) 0, (char *) 0, {0,1,0,0,1}, 2}
};

#define stdi ($_iob[0])
#define stdou ($_iob[1])
#define stder ($_iob[2])

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if(*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for(fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if((fp->flags._READ == 0 && fp->flags._WRITE == 0))
			break;
	if(fp >= _iob + OPEN_MAX)
		return NULL;
	if(*mode == 'w')
		fd = creat(name, PERMS);
	else if(*mode == 'a') {
		if((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	} else
		fd = open(name, O_RDONLY, 0);
	if(fd == -1)
		return NULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
	if(*mode == 'r')
		fp->flags._READ = 1;
	else
		fp->flags._WRITE = 1;
	return fp;
}

int _fillbuf(FILE *fp)
{
	int bufsize;
	if(!fp->flags._READ || fp->flags._EOF || fp->flags._ERR)
		return EOF;
	bufsize = fp->flags._UNBUF ? 1 : BUFSZ;
	if(fp->base == NULL)
		if((fp->base = (char *)malloc(bufsize)) == NULL)
			return EOF;
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if(--fp->cnt < 0) {
		if(fp->cnt == -1)
			fp->flags._EOF = 1;
		else
			fp->flags._ERR = 1;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char) *fp->ptr++;
}

int _flashbuf(char c, FILE *fp)
{
	int res;
	if(!fp->flags._WRITE || fp->flags._EOF || fp->flags._ERR)
		return EOF;
	if(!(res = write(fp->fd, fp->base, fp->cnt)))
		return -1;
	return res;
}

void zero_buf(FILE);

int fflush() //provided that READ and WRITE dont be together
{
	int i;
	for(i = 0; _iob[i].fd && _iob[i].base; i++)
	    if(_iob[i].flags._WRITE) {
	        while(_iob[i].cnt > 0)
		    _iob[i].cnt -= write(_iob[i].fd, _iob[i].base, _iob[i].cnt);
		zero_buf(_iob[i]);
	    } else if(_iob[i].flags._READ) {
		zero_buf(_iob[i]);
	    } else 
	    	return -1;
	return 0;
}
	

		
void zero_buf(FILE node)
{
	node.cnt = 0;
	node.ptr = node.base;
}

