#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "common.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
        return 1;
    }

    // srand((unsigned int)10);

    // get filepath and number of threads from command line arguments
    const char* filepath = argv[1];
    
    // declare variables
    // long total = 0;
    struct timeval start_time, end_time;
    
    // get file size and calculate number of chunks
    long file_size = get_file_size(filepath);
    long num_chunks = get_num_chunks(file_size);
    printf("file size: %ld\n", file_size);
    printf("num of chunks: %ld\n\n", num_chunks);

    // generate random chunk numbers for experiment 2
    // int num_rand_chunks = 10;
    // long rand_chunks[num_rand_chunks];
    // double rand_chunks_time[num_rand_chunks];
    // for(int x = 0; x < num_rand_chunks; x++){
    //     rand_chunks[x] =(long) rand() % num_chunks + 1;
    // }
    
    // start timer (include malloc, fopen, fread)
    gettimeofday(&start_time, NULL);

    // allocate memory for file data
    char *main_data = (char *) malloc(file_size);

    // create file pointers for each thread
    FILE *file_pointer = fopen(filepath, "r");

    // read file chunks in sequential
    for(long i = 0; i < num_chunks; i++){
        
        long start = i * CHUNK_SIZE;
        int cur_chunk_size = (i == num_chunks - 1) ? file_size - i * CHUNK_SIZE : CHUNK_SIZE;
        // long local_count = 
        read_chunk(main_data, file_pointer, start, cur_chunk_size);
        
        // int idx = isNumberPresent(rand_chunks, num_rand_chunks, i);
        // if(idx!=-1){
        //     rand_chunks_time[idx] = omp_get_wtime() - start_time;
        // }

        // for checking correctness
        // total += local_count;
        
    }
    gettimeofday(&end_time, NULL);

    // for checking correctness
    // printf("Total read: %ld\n", total);

    printf("Execution time: %f\n\n", (double)(double)(end_time.tv_usec - start_time.tv_usec) / 1000000 + (double)(end_time.tv_sec - start_time.tv_sec));

    // printf("Rand Chunks selected: ");
    // for (int i = 0; i < num_rand_chunks; i++) {
    //     printf("%ld ", rand_chunks[i]);
    // }
    // printf("\n");

    // printf("STATS:\nAverage Response Time:%f\nMax Response Time:%f\nMin Response Time:%f\n", findAverage(rand_chunks_time, num_rand_chunks), findMax(rand_chunks_time, num_rand_chunks), findMin(rand_chunks_time, num_rand_chunks));

    fclose(file_pointer);
    free(main_data);

    // for checking correctness
    // FILE* outfile = fopen("output.txt","wb");
    // fwrite(main_data, sizeof(char), (total), outfile);

    return 0;

}
