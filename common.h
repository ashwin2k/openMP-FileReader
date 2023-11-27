#ifndef COMMON_H
#define COMMON_H

// Function declarations that are shared between scripts
#define FILE_SIZE_GB 1
#define BUFFER_SIZE 1024
#define CHUNK_SIZE 1024

void create_new_random_file();

int isNumberPresent(long arr[], int size, long target);

double findMax(double arr[], int size);

double findMin(double arr[], int size);

double findAverage(double arr[], int size);

#endif