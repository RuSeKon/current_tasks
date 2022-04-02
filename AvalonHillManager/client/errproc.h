#ifndef ERRPROCHSENTRY
#define ERRPROCHSENTRY

int Socket(int, int, int);
void Connect(int, const struct sockaddr*, socklen_t);
void Memove(void *dst, const void* src, int size);
int Write(int, const void*, size_t);
int Read(int, void*, size_t);
//int Send(int, const void*, size_t);
int Recv(int, void*, size_t);

#endif