#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
int main(int argc, char **argv)
{
	int fd1, fd2;
	char buf[1024];
	int nbyte;
	if(argc != 3)
	{
		printf("Using : %s srcfilename decfilename\n", argv[0]);
		return -1;
	}
        struct timeval  start_time,end_time;
        double time;
        gettimeofday(&start_time,NULL);
	if((fd1 = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open");
		return -1;
	}	
	if((fd2 = open(argv[2], O_WRONLY| O_CREAT | O_TRUNC, 0666))  < 0)
	{
		perror("open");
		return -1;
	}
	while((nbyte = read(fd1, buf, sizeof(buf))) > 0)
	{
		write(fd2, buf, nbyte);
	}
	close(fd1);
	close(fd2);
        
        gettimeofday(&end_time,NULL);
        time=(end_time.tv_sec-start_time.tv_sec)*1000000;
        time=time+(end_time.tv_usec-start_time.tv_usec);
        time=time/1000000;

        printf("use time= %lf s done\n",time);
	return 0;
}
