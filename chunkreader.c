#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[]){
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
    // long total = 0;
    double start_time, end_time;
    
    // get file size and calculate number of chunks
    long file_size = get_file_size(filepath);
    long num_chunks = get_num_chunks(file_size);
    // printf("file size: %ld\n", file_size);
    // printf("num of chunks: %ld\n\n", num_chunks);
    
    // start timer (include malloc, fopen, fread)
    start_time = omp_get_wtime();

    // allocate memory for file data
    char *main_data = (char *) malloc(file_size);

    // create file pointers for each thread
    FILE *file_pointers[t];
    for(int i = 0; i < t; i++){
        file_pointers[i]=fopen(filepath, "r");
        if (file_pointers[i] == NULL) {
            fprintf(stderr, "Error: Failed to open input file.");
            exit(EXIT_FAILURE);
        }
    }

    // read file chunks in parallel
    #pragma omp parallel for num_threads(t)// reduction(+:total)
    for(long i = 0; i < num_chunks; i++){
        
        long start = i * CHUNK_SIZE;
        int cur_chunk_size = (i == num_chunks - 1) ? file_size - i * CHUNK_SIZE : CHUNK_SIZE;
        //long local_count = 
        read_chunk(main_data, file_pointers[omp_get_thread_num()], start, cur_chunk_size);
        
    }
    end_time = omp_get_wtime();

    // for checking correctness
    // printf("Total read: %ld\n", total);

    printf("Execution time: %f\n\n", end_time - start_time);

    for(int i = 0; i < t; i++){
        fclose(file_pointers[i]);
    }
    free(main_data);

    // for checking correctness
    // FILE* outfile = fopen("output.txt","wb");
    // fwrite(main_data, sizeof(char), (total), outfile);

    return 0;
}