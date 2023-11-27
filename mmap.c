#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <omp.h>

#include "common.h"

// mention assumptions in report, that no error checking is done. its assumed input is valid and I/O operations are successful

int main(int argc, char *argv[]) {
    srand((unsigned int)10);
    const char* filepath = argv[1];
    int t = atoi(argv[2]);
    double start, end;
    int read_counter = 0;

    // get file size and calculate number of chunks
    long file_size = get_file_size(filepath);
    long num_chunks = get_num_chunks(file_size);
    printf("file size: %ld\n", file_size);
    printf("num of chunks: %ld\n\n", num_chunks);

    long rand_chunks[10];
    double rand_chunks_time[10];
    for(int x=0;x<10;x++){
        rand_chunks[x] =(long) rand() % num_chunks + 1;
    }

    // start timer
    start = omp_get_wtime();

    // open file and memory map it
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: Failed to open input file.");
        exit(EXIT_FAILURE);
    }
    char* file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    // read file chunks in parallel
    #pragma omp parallel for num_threads(t) reduction(+:read_counter)
    for (int i = 0; i < num_chunks; i++) {
        char *chunk_start = file_data + i * CHUNK_SIZE;
        int cur_chunk_size = (i == num_chunks - 1) ? file_size - i * CHUNK_SIZE : CHUNK_SIZE;
        
        // for testing correctness
        for (int j = 0; j < cur_chunk_size; j++) {
            read_counter++;
        }

        // for reading specific chunks
        int idx = isNumberPresent(rand_chunks, sizeof(rand_chunks) / sizeof(rand_chunks[0]), i);
        if(idx!=-1){
            rand_chunks_time[idx] = omp_get_wtime() - start;
        }
        
    }
    end = omp_get_wtime();

    printf("Total read: %d\n", read_counter);
    printf("Execution time: %f\n\n", end-start);


    printf("Rand Chunks selected: ");
    int size = sizeof(rand_chunks_time) / sizeof(rand_chunks_time[0]);
    for (int i = 0; i < size; i++) {
        printf("%ld ", rand_chunks[i]);
    }
    printf("\n");

    printf("STATS:\nAverage Response Time:%f\nMax Response Time:%f\nMin Response Time:%f\n", findAverage(rand_chunks_time, size), findMax(rand_chunks_time, size), findMin(rand_chunks_time, size));

    munmap(file_data, file_size);
    close(fd);

    return 0;
}