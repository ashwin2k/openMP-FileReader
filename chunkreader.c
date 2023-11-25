#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#define FILE_SIZE_GB 1
#define BUFFER_SIZE 1024

#define CHUNK_SIZE

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

    while (ftell(file) < FILE_SIZE_GB * 1024L * 1024L * 1024L) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = 'A' + rand() % 26;  // Random characters (0 to 255)
        }

        fwrite(buffer, sizeof(char), BUFFER_SIZE, file);
    }

    fclose(file);

    printf("File created successfully for %dGB.\n",FILE_SIZE_GB);

}
int read_chunk(int process_no,Range range){
    FILE *file;
    char *buffer = (char *) malloc(range.chunk_size*sizeof(char));
    size_t bytesRead;

    // Open the file
    file = fopen("random_file.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    //Seek to start of chunk
    fseek(file,range.start,SEEK_SET);


    bytesRead = fread(buffer, 1, range.chunk_size, file);
    // fwrite(buffer, 1, bytesRead, stdout);
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

Range divide_chunk(long file_size,int thread_num){
    long chunk_size = (long)floor(file_size/omp_get_num_threads());
    Range result;
    result.start = chunk_size*(thread_num);
    if(thread_num==omp_get_num_threads()-1){
        result.end = file_size;
    }
    else{
        result.end = chunk_size*(thread_num+1) -1;
    }
    result.chunk_size = result.end - result.start + 1;

    return result;
}
int main(){
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
    long total = 0;
    #pragma omp parallel num_threads(5)
    {
        Range rnge = divide_chunk(file_size,omp_get_thread_num());
        long read_len = read_chunk(omp_get_thread_num(),rnge);
        printf("%ld --read\n",read_len);
        total+=read_len;
    }
    printf("Total Read - %ld",total);
    return 0;
}