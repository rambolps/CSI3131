//Student 1: Ryan Rambali (300235460)
//Student 2: Nelson Wang (300174019)
//CSI 3131
//Assignment 3
//July 1st, 2023

/*
How To Run Program -> './a3.out n'
Where N = number of students
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


pthread_mutex_t chair1 = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t chair2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t chair3 = PTHREAD_MUTEX_INITIALIZER;  
pthread_mutex_t chairTA = PTHREAD_MUTEX_INITIALIZER;  
sem_t TASleep;
sem_t studentNotify; 

//Student thread
void *thread_Student(void *arg) {
    //Student thread alternates between programming for a period of time and asking for help from TA.
    int chair = 4; //4 is no chair
    int isTASleeping;
    int *studentNum = (int *)arg; 
    while (1) {


        //Assume student starts off programming. 
        printf("Student %d is programming\n", *studentNum);        

        sleep(rand() % 5 + 1); //sleep for 1 to 4 seconds. 

        //check chair 3
        if (chair == 4 && pthread_mutex_trylock(&chair3) == 0)
        {
            chair = 3;
            printf("Student %d is in chair 3\n", *studentNum);        

        } 

        //check chair 2
        if (chair == 3 && pthread_mutex_trylock(&chair2) == 0)
        {
            chair = 2;
            printf("Student %d is in chair 2\n", *studentNum);    
            pthread_mutex_unlock(&chair3);
        } 

        //check chair 1
        if (chair == 2 && pthread_mutex_trylock(&chair1) == 0)
        {
            chair = 1;
            printf("Student %d is in chair 1\n", *studentNum);        
            pthread_mutex_unlock(&chair2);
        } 

        //check chair TA
        if (chair == 1 && pthread_mutex_trylock(&chairTA) == 0)
        {

            // chair = 0;
            pthread_mutex_unlock(&chair1);

            sem_getvalue(&TASleep,&isTASleeping);
            if (isTASleeping == 0)
            {
                sem_post(&TASleep);
            }
            

            //student getting help
            printf("Student %d is getting help\n", *studentNum);        

            sleep(rand() % 5 + 1); //sleep for 1 to 4 seconds. 

            chair = 4;
            pthread_mutex_unlock(&chairTA);


        } 

        // if (chair == 0)
        // {
        //     //student getting help
        //     printf("Student %d is getting help", *studentNum);        

        //     sleep(rand() % 5 + 1); //sleep for 1 to 4 seconds. 

        //     chair = 4;
        //     pthread_mutex_unlock(&chairTA);
        // }
        
    }

}

// TA thread
void *thread_TA() {
    while (1) {
        int canSleep = 1;

        if (pthread_mutex_trylock(&chair1) != 0)
        {
           canSleep = 0;
        } else {
           pthread_mutex_unlock(&chair1);
        }

        if (pthread_mutex_trylock(&chair2) != 0)
        {
            canSleep = 0;
        } else {
           pthread_mutex_unlock(&chair2);
        }

        if (pthread_mutex_trylock(&chair3) != 0)
        {
            canSleep = 0;
        } else{
           pthread_mutex_unlock(&chair3);
        }
        
        if (pthread_mutex_trylock(&chairTA) != 0)
        {
            canSleep = 0;
        } else{
           pthread_mutex_unlock(&chairTA);
        }

        if (canSleep == 1)
        {
            sem_wait(&TASleep);
            printf("TA is Sleeping\n");
        } else{
            printf("TA is Awake\n");
        }



    }
}



int main(int argc, char *argv[]) {

    //We need input on n students. We have one TA.
    //We have 3 chairs in the hallway, TA can help one student at a time.
    //When no students need help, TA naps at desk 
    //If student arrives, finds TA sleeping, awake TA (semaphore)
    //If student arrives, TA is helping other student, student goes to sit at chair
    //If no chair available (mutex lock), student comes back at different time. 
    //TA checks if any student is waiting in hallway and helps them. 

    // 

    //student 1 -> is chair 1 free? -> yes -> lock chair 1
    //student 1 -> is chair 1 free? -> no -> is chair 2 free? - >
    //student 1 -> is chair 1 free? -> yes -> lock chair 1

    //3 lock/unlock -> mutex
    //2
    //1
    
    //TA asks -> Can I sleep? -> are all chairs free? -> yes > sleep


    //Semaphores for the sleeping TA, and mutex locks for the chairs.

    sem_init(&TASleep,0,1);

    int numOfStudents = 4;
    if (argc > 1)
    {
        numOfStudents = atoi(argv[1]);
    }
    
    pthread_t studentThreads[numOfStudents]; 
    pthread_t TAThread;

    pthread_create(&TAThread, NULL, thread_TA, NULL);

    for (int i = 0; i < numOfStudents; i++){
        pthread_create(&studentThreads[i], NULL, thread_Student, &i);
    }

    return 0;
}