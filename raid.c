#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<time.h>
#include<math.h>

#define num_copies 4
#define FILE_PATH "random_file.txt"
#define CHUNK_SIZE 1024

int isNumberPresent(long arr[], int size, long target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Number is present
        }
    }
    return -1;  // Number is not present
}

long readChunk(FILE* file, long start, long end) {
    fseek(file, start, SEEK_SET);
    long chunkSize = end - start;
    char* buffer = (char*)malloc(chunkSize);
    size_t bytesRead = fread(buffer, 1, chunkSize, file);
    free(buffer);
    return bytesRead;
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


int main(int argc, char *argv[]) {
    srand((unsigned int)10);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int NUM_THREADS = atoi(argv[1]);
    // Open the original file for reading
    FILE* originalFile = fopen(FILE_PATH, "rb");
    if (originalFile == NULL) {
        perror("Error opening original file");
        return 1;
    }

    // Get the file size
    fseek(originalFile, 0, SEEK_END);
    long fileSize = ftell(originalFile);
    fseek(originalFile, 0, SEEK_SET);
    long num_chunks = ceil(fileSize/CHUNK_SIZE);

    srand((unsigned int)10);
    
    long rand_chunks[10];
    double rand_chunks_time[10];
    
    for(int x=0;x<10;x++){
        rand_chunks[x] =(long) rand() % num_chunks + 1;
    }

    double startTime = omp_get_wtime();
    FILE* fileCopy[NUM_THREADS];
    for(int i=0;i<NUM_THREADS;i++){
        char copyFileName[20];
        sprintf(copyFileName, "copy%d.txt", i%num_copies);
        fileCopy[i] = fopen(copyFileName, "rb");
        if (fileCopy[i] == NULL) {
                perror("Error opening file copy");
                // Handle error and clean up resources
                fclose(originalFile);
                exit(1);
            }
    }
            
    #pragma omp parallel for num_threads(NUM_THREADS)
    for(long i=0;i<num_chunks;i++){
        int threadID = omp_get_thread_num();
        
        // Open existing file copies for reading
        
        // Calculate the start and end positions for this thread
        long start = i * CHUNK_SIZE;
        long end = (i == num_chunks - 1) ? fileSize : (i + 1) * CHUNK_SIZE;        
        long local_count = readChunk(fileCopy[threadID], start, end);
        int idx = isNumberPresent(rand_chunks, sizeof(rand_chunks) / sizeof(rand_chunks[0]), i);
        if(idx!=-1){
            rand_chunks_time[idx] = omp_get_wtime() - startTime;
        }
    }
    double endTime = omp_get_wtime();
    double elapsedTime = endTime - startTime;
    int size = sizeof(rand_chunks_time) / sizeof(rand_chunks_time[0]);
    printf("random chunks selected\n");
    for (int i = 0; i < size; i++) {
        printf("%ld ", rand_chunks[i]);
    }
    printf("\nTime taken to read the chunk\n");
    for(int i=0;i<size;i++){
        printf("%f ", rand_chunks_time[i]);
    }
    printf("\n");
    printf("STATS: Average Response Time:%f\nMax Response Time:%f\nMin Response Time:%f\n", findAverage(rand_chunks_time, size),
                                                                                            findMax(rand_chunks_time, size),
                                                                                            findMin(rand_chunks_time, size));
    // Close the original file
    fclose(originalFile);
    printf("Total time taken: %f seconds\n", elapsedTime);
    return 0;
}