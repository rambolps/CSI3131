#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>

#define PAGESIZE 4096

/*
 To run the code properly, perform these in order:
    gcc producer.c -o producer -lrt
    gcc consumer.c -o consumer -lrt
    cd output
    ./producer [amount of catalan numbers to print]
    ./consumer
*/

unsigned long int catalan(int n) {

    //Cn = (2n)! / [(n + 1)! * n!]

    unsigned long int num = 1;
    unsigned long int den1 = 1;
    unsigned long int den2 = 1;
    unsigned long int result = 1;

    if (n == 0) {
        return result;
    }

    for (int i = (2 * n); i > 0; i--){
        num *= i;
    }

    for (int j = (n + 1); j > 0; j--) {
        den1 *= j;
    }

    for (int k = n; k > 0; k--) {
        den2 *= k;
    }
    
    result = num / (den1 * den2);

    return result;
}

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]); //Get the number of catalan numbers to process. 

    if (n < 1) {
        printf("Please enter a valid integer.");

        return 1;
    }

    int fd = shm_open("shm_catalan", O_CREAT | O_RDWR, 0600);
    if (fd == -1) {
        perror("Error with shared memory.");

        return 1;
    }

    ftruncate(fd, PAGESIZE);

    unsigned long int *shared_memory = mmap(0, PAGESIZE, PROT_WRITE, MAP_SHARED, fd, 0);

    for (int i = 0; i < n; i++){ 
        shared_memory[i] = catalan(i);
    }

    shared_memory[n] = 0; //End of the array identifier since we cannot easily determine the size of the array when passing to another function.

    munmap(shared_memory, PAGESIZE); // Unmap the shared memory and close it. 

    return 0;
}




















