#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "common.h"

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
