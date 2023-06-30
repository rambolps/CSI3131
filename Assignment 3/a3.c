#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 3
// to compile use: gcc -g -pthread a3.c -o main
// To run cd to output then do ./a3 [number of students]

int numStudents = 5;

pthread_t ta;
pthread_mutex_t mutex;

sem_t sem_Students; 
sem_t sem_TA;

int taIsSleeping = 0; // 0 meaning not sleeping, 1 meaning sleeping; 

int chairs[3]; //Number of chairs
int studentsWaiting = 0;
int nextChairPos = 0;
int nextTeachPos = 0;
int studentWaitingID = 0;

int seated(int studentId) {
    for (int i = 0; i < 3; i++) {
        if (chairs[i] == studentId) {
            return 1;
        }
    }

    return 0;
}

int isEmpty() {
    for (int i = 0; i < 3; i++) {
        if (chairs[i] > 0) {
            return 0;
        }
    }
    return 1;
}


void *studentThread(void* arg) {
    int id = *(int*) arg;

    while (1) {

        if (seated(id) == 1) {
            continue; 
        } 

        int pTime = rand() % 5 + 1;

        printf("Student %d is programming for %d seconds.\n", id, pTime);
        sleep(pTime);
        /*
            Ask TA for help
            - If TA not helping other student: Ask for help, ELSE: sit in a chair
            - If no chairs are available, start programming. 
            - If TA is sleeping, wake TA up using semaphore. 
        
            -- Check queue for free seats, if a seat is free, grab the lock so that no other threads can sit at the same chair.
        */
       
        // Check if the seats are empty, if they are empty, go direcctly to ask the TA for help, otherwise, try to get a seat. 
        pthread_mutex_lock(&mutex);

        // If no students are in the seats, try to see if TA is free.
        if (studentsWaiting == 0){
            // Check if TA is busy/sleeping.
            if (taIsSleeping == 1) {
                //wake up TA and get help
                studentsWaiting++;
                studentWaitingID = id;
                pthread_mutex_unlock(&mutex);
                sem_post(&sem_Students);
                sem_wait(&sem_TA);
            }
            // Sit in a seat.
            else {
                chairs[0] == id;
                studentsWaiting++;

                printf("The TA is busy, Student %d is sitting at Chair 0.\n", id); //continue here.
                
                nextChairPos = (nextChairPos + 1) % NUM_CHAIRS; 
                pthread_mutex_unlock(&mutex);

                //Wake up the TA if they are sleeping.
                sem_post(&sem_Students);
                sem_wait(&sem_TA);
            }
        }

        else {
            
            if (studentsWaiting < NUM_CHAIRS) {

                chairs[nextChairPos] = id;
                studentsWaiting++;

                printf("Student %d is sitting in seat %d\n", id, nextChairPos);
                nextChairPos = (nextChairPos + 1) % NUM_CHAIRS;

                pthread_mutex_unlock(&mutex);

                //Wake up the TA
                sem_post(&sem_Students);
                sem_wait(&sem_TA);
            } 
            // No chairs available, go back to programming.
            else {
                pthread_mutex_unlock(&mutex);
                printf("All chairs are full, student %d will try again later.\n", id);
            }

        } 
       
    }


    free(arg); //free the memmory allocated to the IDs. 
    }

void *taThread() {

    // If no students ask for help (in chairs), sleep. 
    // When finished helping student, check seats to see if any students need help. 
    // If so, help students. 

    //Sleep if semaphore of students is 0.
    while (1) {
        pthread_mutex_lock(&mutex); 

        // If there are students waiting in chairs 
        if (studentsWaiting > 0) {
            taIsSleeping = 0;

            //Help a student. 
            sem_wait(&sem_Students);

            sleep(rand() % 6 + 1);

            if (isEmpty()) {
                printf("The TA has helped student %d.\n", studentWaitingID);
            }

            else {
                int studentId = chairs[nextTeachPos];
                chairs[nextTeachPos] = 0;
                nextTeachPos = (nextTeachPos + 1) % NUM_CHAIRS;
                printf("The TA has helped student %d\n", studentId);

            }

            studentsWaiting--;
            pthread_mutex_unlock(&mutex);

            sem_post(&sem_TA);
        } 
        
        else {
            pthread_mutex_unlock(&mutex);

            if (taIsSleeping != 1) {
                taIsSleeping = 1; 

                printf("TA is sleeping.\n");
            }
        }
    }
}

void main(int argc, char* argv[]) {

    srand(time(0));

    sem_init(&sem_TA, 0, 0);
    sem_init(&sem_Students, 0 , 0);
    pthread_mutex_init(&mutex, NULL);
    
    //create TA thread.
    pthread_create(&ta, NULL, &taThread, NULL);

    if (argc > 1) {
        numStudents = atoi(argv[1]);
    }

    pthread_t student[numStudents];

    // Create 4 student threads

    for (int i = 0; i < numStudents; i++) {
        //Create student thrreads and pass in the index number as their IDs.
        int* id = malloc(sizeof(int));
        *id = i + 1; //We allocate different memory locations for i so that we avoid multithreading issues. 

        pthread_create(&student[i], NULL, &studentThread, id); //We check if this is not 0 for errors. 
    }

    pthread_join(ta, NULL);
    //join threads. aka wait for threads to finish.
    for (int a = 0; a < numStudents; a++){
        pthread_join(student[a], NULL);
    }

    sem_destroy(&sem_TA);
    sem_destroy(&sem_Students);
    pthread_mutex_destroy(&mutex);

}
