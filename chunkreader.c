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

int isNumberPresent(long arr[], int size, long target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Number is present
        }
    }
    return -1;  // Number is not present
}

double findMax(double arr[], int size) {
    if (size <= 0) {
        // Handle empty array or invalid size
        printf("Invalid array size.\n");
        return -1;  // Returning a special value to indicate an error
    }

    double max = arr[0];  // Assume the first element is the maximum

    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];  // Update the maximum if a larger element is found
        }
    }

    return max;
}

double findMin(double arr[], int size) {
    if (size <= 0) {
        // Handle empty array or invalid size
        printf("Invalid array size.\n");
        return -1;  // Returning a special value to indicate an error
    }

    double min = arr[0];  // Assume the first element is the minimum

    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];  // Update the minimum if a smaller element is found
        }
    }

    return min;
}

double findAverage(double arr[], int size) {
    if (size <= 0) {
        // Handle empty array or invalid size
        printf("Invalid array size.\n");
        return -1.0;  // Returning a special value to indicate an error
    }

    double sum = 0;

    for (int i = 0; i < size; i++) {
        sum += arr[i];  // Sum up all the elements in the array
    }

    return (double)sum / size;  // Calculate the average
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
    long num_chunks = get_num_chunks(file_size);
    
    long rand_chunks[10];
    double rand_chunks_time[10];


    for(int x=0;x<10;x++){
        rand_chunks[x] =(long) rand() % num_chunks + 1;
    }
    
    printf("File size is %ld\n",file_size);

    char *main_data = (char *) malloc(file_size);

    long total = 0;
    
    FILE *filePointers[NUM_THREADS];
    for(int i =0;i<NUM_THREADS;i++){
        filePointers[i]=fopen("random_file.txt", "r");
    }

    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+:total) num_threads(NUM_THREADS)
    for(long i=0;i<num_chunks;i++){

        long read_len = read_chunk_small(i,main_data,filePointers[omp_get_thread_num()]);
        int idx = isNumberPresent(rand_chunks, sizeof(rand_chunks) / sizeof(rand_chunks[0]), i);
        if(idx!=-1){
            rand_chunks_time[idx] = omp_get_wtime() - start;
        }
        total+=read_len;
        
    }

    int size = sizeof(rand_chunks_time) / sizeof(rand_chunks_time[0]);
    printf("Total Read - %ld\n",total);

    printf("Rand Chunks selected: ");
    for (int i = 0; i < size; i++) {
        printf("%ld ", rand_chunks[i]);
    }
    printf("\n");

    printf("STATS: Average Response Time:%f\nMax Response Time:%f\nMin Response Time:%f\n", findAverage(rand_chunks_time, size),
                                                                                            findMax(rand_chunks_time, size),
                                                                                            findMin(rand_chunks_time, size));

    FILE* outfile = fopen("output.txt","wb");

    fwrite(main_data, sizeof(char), (total), outfile);

    return 0;
}