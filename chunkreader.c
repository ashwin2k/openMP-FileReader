#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <string.h>

#define FILE_SIZE_GB 1
#define BUFFER_SIZE 1024
#define CHUNK_SIZE 1024
#define NUM_THREADS 5
typedef struct{
    long start;
    long end;
    long chunk_size;
} Range;

void create_new_random_file(){
    FILE *file;
    char buffer[BUFFER_SIZE];
    srand(time(NULL));

    file = fopen("random_file.txt", "wb");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while (ftell(file) < FILE_SIZE_GB * 1024L* 1024L* 1024L ) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = 'A' + rand() % 26;  // Random characters (0 to 255)
        }

        fwrite(buffer, sizeof(char), BUFFER_SIZE, file);
    }

    fclose(file);

    printf("File created successfully for %dGB.\n",FILE_SIZE_GB);

}

int read_chunk_small(long chunk_number,char* main_data,FILE* file){
    // char *buffer = (char *) malloc(CHUNK_SIZE);
    size_t bytesRead;
    bytesRead = fread(main_data+(chunk_number*CHUNK_SIZE), 1, CHUNK_SIZE, file);
    // memcpy(main_data+(chunk_number*CHUNK_SIZE),buffer,CHUNK_SIZE);
    return bytesRead;
}

long get_file_size(FILE *file){
    long size;

    long currentPosition = ftell(file);

    // Move to the end of the file
    fseek(file, 0, SEEK_END);

    // Get the current position, which is the size of the file
    size = ftell(file);

    // Restore the original position in the file
    fseek(file, currentPosition, SEEK_SET);

    return size;

}
long get_num_chunks(long file_size){
    return ceil(file_size/CHUNK_SIZE);
}


int main(){
    srand((unsigned int)10);
    // create_new_random_file();
    FILE *file;
    char buffer[1024];
    size_t bytesRead;
    // Open the file
    file = fopen("random_file.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    long file_size = get_file_size(file);
    printf("File size is %ld\n",file_size);
    char *main_data = (char *) malloc(file_size);
    long num_chunks = get_num_chunks(file_size);

    long total = 0;
    
    FILE *filePointers[NUM_THREADS];
    for(int i =0;i<NUM_THREADS;i++){
        filePointers[i]=fopen("random_file.txt", "r");
    }
    #pragma omp parallel for reduction(+:total) num_threads(NUM_THREADS)
    for(long i=0;i<num_chunks;i++){
        long read_len = read_chunk_small(i,main_data,filePointers[omp_get_thread_num()]);

        total+=read_len;
        
    }

    printf("Total Read - %ld",total);
    FILE* outfile = fopen("output.txt","wb");

    fwrite(main_data, sizeof(char), (total), outfile);

    return 0;
}