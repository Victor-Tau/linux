  
  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/sem.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/shm.h>  
#define BUF_SIZE 1024  

#define MEMKEY 114  

#define SEMKEY1 110
#define SEMKEY2 111


union semun
{
   int val;
   struct semid_ds *buf;
   unsigned short int *array;
   struct seminfo *__buf;
};
int semaphore_p(int semid )
{
   struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    if(semop(semid, &sem_b, 1) == -1)
                {
                    fprintf(stderr, "sem_p failed/n");
                    return 0;
                }
        return 1;
  }

 int semaphore_v(int semid)
  {
       struct sembuf sem_b;
        sem_b.sem_num = 0;
        sem_b.sem_op = 1;
        if(semop(semid, &sem_b, 1) == -1)
                {
                    fprintf(stderr, "sem_p failed/n");
                    return 0;
                }
        return 1;
  }
  
  
int main(int argc, char** argv)  
{  
    int count=0;
    int shmid;  
    char *shmptr;  
    struct shmid_ds shmbuf; 
    int semid1,semid2; 
    union semun sem1,sem2;
    semid1 = semget(SEMKEY1, 1, 0666 | IPC_CREAT); 

    sem1.val = 1;
    semctl(semid1, 0, SETVAL, sem1); 

    semid2 = semget(SEMKEY2, 1, 0666 | IPC_CREAT); 
    sem2.val = 0;
    semctl(semid2, 0, SETVAL, sem2); 

    shmid = shmget(MEMKEY, BUF_SIZE, (IPC_CREAT |0777) );  
    if ( -1 == shmid )  
    {  
        printf("server shmget error!\n");  
        fprintf(stderr, "Error: %d - %s\n", errno, strerror(errno));  
        exit(1);  
    }  
  
    shmptr = (char *) (shmat(shmid, 0, 0)) ;  
    if ( (void*)-1 == shmptr  )  
    {  
        printf("server shmat error!\n");  
        fprintf(stderr, "Error: %d - %s\n", errno, strerror(errno));  
        if(shmctl(shmid,IPC_RMID,&shmbuf) < 0)   
        {  
            perror("shmctl error");  
            fprintf(stderr, "Error: %d - %s\n", errno, strerror(errno));  
        }  
        exit(1);  
    }  
    printf("start write:\n");
    while(count < 100){  
        semaphore_p(semid1);
        sprintf(shmptr,"%d",count++) ;  
        semaphore_v(semid2);
    }
    semaphore_p(semid1);
    sprintf(shmptr,"end") ;
    semaphore_v(semid2);
    return 0;  
}  
