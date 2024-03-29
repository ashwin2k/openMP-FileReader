#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <omp.h>

#include "common.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filepath> <num_threads>\n", argv[0]);
        return 1;
    }

    // set random seed - fixed across scripts
    // srand((unsigned int)10);

    // get filepath and number of threads from command line arguments
    const char* filepath = argv[1];
    int t = atoi(argv[2]);

    // declare variables
    double start_time, end_time;
    // int total = 0;

    // get file size and calculate number of chunks
    long file_size = get_file_size(filepath);
    long num_chunks = get_num_chunks(file_size);
    // printf("file size: %ld\n", file_size);
    // printf("num of chunks: %ld\n\n", num_chunks);

    // start timer
    start_time = omp_get_wtime();

    // open file
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: Failed to open input file.");
        exit(EXIT_FAILURE);
    }

    // memory map file
    char* file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    // read file chunks in parallel
    #pragma omp parallel for num_threads(t)// reduction(+:total)
    for (int i = 0; i < num_chunks; i++) {
        char *chunk_start = file_data + i * CHUNK_SIZE;
        int cur_chunk_size = (i == num_chunks - 1) ? file_size - i * CHUNK_SIZE : CHUNK_SIZE;
        
        // for testing correctness
        // for (int j = 0; j < cur_chunk_size; j++) { 
        //     total++;
        // }

        
    }
    end_time = omp_get_wtime();

    // for testing correctness
    // printf("Total read: %d\n", total);

    printf("Execution time: %f\n\n", end_time - start_time);
    munmap(file_data, file_size);
    close(fd);

    return 0;
}