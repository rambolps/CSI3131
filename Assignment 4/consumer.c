#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>

#define PAGESIZE 4096

int main (int argc, char *argv[]) {

    int fd = shm_open("shm_catalan", O_RDONLY, 0666);

    long unsigned int* shared_memory = mmap(0, PAGESIZE, PROT_READ, MAP_SHARED, fd, 0);

    printf("Catalan Numbers: ");

    int i = 0;

    while (shared_memory[i] != 0){
        printf("%lu ", shared_memory[i]);
        i++;
    }

    printf("\n");

    shm_unlink("shm_catalan");
    return 0;
}