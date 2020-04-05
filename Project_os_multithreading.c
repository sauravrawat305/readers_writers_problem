#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex, writerThread;
int globalSharedVariable, readersCount = 0;

void *readerfunction(void *argument){
        //Entry Part
        pthread_mutex_lock(&mutex);
        readersCount++;
        if(readersCount==1){
                pthread_mutex_lock(&writerThread);//it is locked ,No writer should come
        }
        pthread_mutex_unlock(&mutex);//it is unlocked so next reader can come
        //Exit Part
        int n = rand() % 10;
        int d = ((int)argument);
        printf("Reader %d wait for Random time= %d\n", d, n);
        sleep(n);
        printf("Enter the number of time Reader %d want to read:\n", d);
        int notime;
        scanf("%d", &notime);
        printf("Now Reader %d is reading....\n", d);
        int j;
        for(j=0; j<notime; j++){
                printf("Reader %d read the shared value = %d\n", d, globalSharedVariable);
        }
        printf("Number of Readers present = %d\n", readersCount);
        pthread_mutex_lock(&mutex);
        readersCount--;
        if(readersCount==0) //Now writer can come if they want
		{
                pthread_mutex_unlock(&writerThread);
        }
        pthread_mutex_unlock(&mutex);
}

void *writerfunction(void *argument){
        pthread_mutex_lock(&writerThread);
        int n = rand() % 10;
        int d = ((int)argument);
        printf("==============================================\n");
        printf("W%d Wait for Random time(0-10ns) = %d\n", d, n);
        sleep(n);
        printf("Enter the no. of times Writer %d wants to write:\n", d);
        int notime;
        scanf("%d", &notime);
        printf("Now, Writer %d is writing..........\n", d);
        int j;
        for(j=0; j<notime; j++){
                printf("Enter the value of %dth integer to write:\n", (j+1));
                int u;
                scanf("%d", &u);
                globalSharedVariable = u;
        }
        printf("New updated value of global shared variable is : %d \n", globalSharedVariable);
        printf("==============================================\n");
        pthread_mutex_unlock(&writerThread);
}

void main(){
        printf("Enter the value of global shared variable: \n");
        scanf("%d", &globalSharedVariable);
        printf("\n==============================================\n");
        int noReaders, noWriters, i;
        printf("Enter the no. of Readers:\n");
        scanf("%d", &noReaders);
        for(i=0; i<noReaders; i++){
                printf("Reader %d\n", i);
        }
        printf("==============================================\n\n");
        printf("Enter the no. of Writers:\n");
        scanf("%d", &noWriters);
        for(i=0; i<noWriters; i++){
                printf("Writer %d\n", i);
        }
        printf("==============================================\n");

        pthread_t r[noReaders], w[noWriters];
        pthread_mutex_init(&writerThread, NULL);
        pthread_mutex_init(&mutex, NULL);

        if(noReaders<0 || noWriters<0)
		{
                printf("Warning:  READER or WRITER can't be negative'\n");
                printf("Program is Terminating....\n");
                printf("Try again later....\n");
				return;
        }
		else if(noWriters == 0)
		{
                printf("Sorry: You haven't taken any WRITER\n");
                printf("No writer thread will be creaded\n");
        }
		else
		{
                printf("Creating Threads ....\n");
        }
        printf("\n==============================================\n");

        if(noWriters==noReaders){
                for(i=0; i<noWriters; i++){
                        pthread_create(&w[i], NULL, &writerfunction, (int *)i);
                        pthread_create(&r[i], NULL, &readerfunction, (int *)i);
                }
                for(i=0; i<noWriters; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
        }else if(noWriters>noReaders){
                for(i=0; i<noReaders; i++){
                        pthread_create(&w[i], NULL, &writerfunction, (int *)i);
                        pthread_create(&r[i], NULL, &readerfunction, (int *)i);
                }
                for(i=noReaders; i<noWriters; i++){
                        pthread_create(&w[i], NULL, &writerfunction, (int *)i);
                }
                for(i=0; i<noReaders; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
                for(i=noReaders; i<noWriters; i++){
                        pthread_join(w[i], NULL);
                }
        }else{
                for(i=0; i<noWriters; i++){
                        pthread_create(&w[i], NULL, &writerfunction, (int *)i);
                        pthread_create(&r[i], NULL, &readerfunction, (int *)i);
                }
                for(i=noWriters; i<noReaders; i++){
                        pthread_create(&r[i], NULL, &readerfunction, (int *)i);
                }
                for(i=0; i<noWriters; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
                for(i=noWriters; i<noReaders; i++){
                        pthread_join(r[i], NULL);
                }
        }
        printf("\n\n=============After execution of all the threads=============\n");
        printf("Final value of global shared variable = %d\n", globalSharedVariable);
}
