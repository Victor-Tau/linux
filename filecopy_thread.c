#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <stdlib.h>
sem_t sem1,sem2;
int fd_in, fd_out;
char buf[1024];
int  len=0;
int  start_flag=0;
void pthread1(void *arg)
{
  printf("start file copy\n");
  start_flag=1;
  do{
     sem_wait(&sem1);
     len = read(fd_in, buf, sizeof(buf)); 
     sem_post(&sem2);
  }while(len>0);
}
void pthread2(void *arg)
{
   while(len>0||start_flag==0){ 
       sem_wait(&sem2);
       write(fd_out, buf,len);
       sem_post(&sem1);
   }
   printf("finished!\n");
}
int main(int argc, char **argv)
{
        int ret;
        pthread_t id1,id2;
        struct timeval t_start,t_end;
        float timeuse;
	
        if(argc != 3)
	{
		printf("Using : %s srcfilename decfilename\n", argv[0]);
		return -1;
	}
	if((fd_in = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open");
		return -1;
	}	
	if((fd_out = open(argv[2], O_WRONLY| O_CREAT | O_TRUNC, 0666))  < 0)
	{
		perror("open");
		return -1;
	}
        ret=sem_init(&sem1,0,1);

        if(ret!=0)
        {
                perror("sem_init");
        }

        ret=sem_init(&sem2,0,0);
        if(ret!=0)
        {
                perror("sem_init");
        }
        gettimeofday(&t_start, NULL);

        ret=pthread_create(&id1,NULL,(void *)pthread1, NULL);
        if(ret!=0)
                perror("pthread cread1");
        ret=pthread_create(&id2,NULL,(void *)pthread2, NULL);
        if(ret!=0)
                perror("pthread cread2");
        
        pthread_join(id1,NULL);
        pthread_join(id2,NULL);


	close(fd_in);
	close(fd_out);
        gettimeofday(&t_end, NULL);

        timeuse=1000000*(t_end.tv_sec-t_start.tv_sec)+t_end.tv_usec-t_start.tv_usec;
        timeuse/=1000000;
        printf("Used Time:%fs\n",timeuse);



	return 0;
}
