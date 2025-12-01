#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
int main(void)
{
	int fdr,fdw;
	char buffer[1];
        int fpos,i=0;
        struct timeval  start_time,end_time;
        double time;
        gettimeofday(&start_time,NULL);
        fdr=open("poster.jpg",O_RDONLY);
        if(fdr < 0)
            exit(0);

        fdw=open("test.jpg",O_WRONLY|O_CREAT|O_TRUNC,0666);
        if(fdw < 0 )
            exit(0);
        fpos=lseek(fdr,0,SEEK_END);
        lseek(fdr,0,SEEK_SET);
        while(i<fpos){
           read(fdr,buffer,sizeof(buffer));
           write(fdw,buffer,sizeof(buffer));
           i++;
        }
	close(fdr);
        close(fdw);

        gettimeofday(&end_time,NULL); 
        time=(end_time.tv_sec-start_time.tv_sec)*1000000;
        time=time+(end_time.tv_usec-start_time.tv_usec);
        time=time/1000000;
	printf("use time= %lf s done\n",time);
} 
