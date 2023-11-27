#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#include "common.h"

#define NUM_COPIES 4

long readChunk(FILE* file, long start, long end) {
    fseek(file, start, SEEK_SET);
    long chunkSize = end - start;
    char* buffer = (char*)malloc(chunkSize);
    size_t bytesRead = fread(buffer, 1, chunkSize, file);
    free(buffer);
    return bytesRead;
}

int read_chunk_small(long chunk_number, char* main_data, FILE* file){
    // char *buffer = (char *) malloc(CHUNK_SIZE);
    size_t bytesRead;
    bytesRead = fread(main_data+(chunk_number*CHUNK_SIZE), 1, CHUNK_SIZE, file);
    // memcpy(main_data+(chunk_number*CHUNK_SIZE),buffer,CHUNK_SIZE);
    return bytesRead;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filepath> <num_threads>\n", argv[0]);
        return 1;
    }

    // set random seed - fixed across scripts
    srand((unsigned int)10);

    // get filepath and number of threads from command line arguments
    const char* filepath = argv[1];
    int t = atoi(argv[2]);

    // declare variables
    double start_time, end_time;
    long total = 0;

    // get file size and calculate number of chunks
    long file_size = get_file_size(filepath);
    long num_chunks = get_num_chunks(file_size);
    printf("file size: %ld\n", file_size);
    printf("num of chunks: %ld\n\n", num_chunks);

    // generate random chunk numbers for experiment 2
    int num_rand_chunks = 10;
    long rand_chunks[num_rand_chunks];
    double rand_chunks_time[num_rand_chunks];
    for(int x = 0; x < num_rand_chunks; x++){
        rand_chunks[x] =(long) rand() % num_chunks + 1;
    }

    // start timer
    start_time = omp_get_wtime();

    // allocate memory for file data
    char *main_data = (char *) malloc(file_size);

    // create file pointers for each thread and corresponding file copies
    FILE* file_pointers[t];
    for(int i = 0; i < t; i++){
        char copyFileName[10];
        sprintf(copyFileName, "copy%d.txt", i % NUM_COPIES);
        file_pointers[i] = fopen(copyFileName, "rb");
    }

    // read file chunks in parallel
    #pragma omp parallel for num_threads(t) reduction(+:total)
    for(long i = 0; i < num_chunks; i++){

        int threadID = omp_get_thread_num();        
        long start = i * CHUNK_SIZE;
        long end = (i == num_chunks - 1) ? file_size : (i + 1) * CHUNK_SIZE;        
        long local_count = readChunk(file_pointers[threadID], start, end);

        // long read_len = read_chunk_small(i, main_data, file_pointers[omp_get_thread_num()]);

        // experiment 2: records times of specific (randomly selected) chunks
        int idx = isNumberPresent(rand_chunks, num_rand_chunks, i);
        if(idx!=-1){
            rand_chunks_time[idx] = omp_get_wtime() - start_time;
        }

        total += local_count;
    }
    end_time = omp_get_wtime();

    printf("Total read: %ld\n", total);
    printf("Execution time: %f\n\n", end_time - start_time);

    printf("Rand Chunks selected: ");
    for (int i = 0; i < num_rand_chunks; i++) {
        printf("%ld ", rand_chunks[i]);
    }
    printf("\n");

    printf("STATS:\nAverage Response Time:%f\nMax Response Time:%f\nMin Response Time:%f\n", findAverage(rand_chunks_time, num_rand_chunks), findMax(rand_chunks_time, num_rand_chunks), findMin(rand_chunks_time, num_rand_chunks));
    
    for(int i = 0; i < t; i++){
        fclose(file_pointers[i]);
    }

    return 0;
}
