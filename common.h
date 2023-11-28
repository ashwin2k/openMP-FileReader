#ifndef COMMON_H
#define COMMON_H

// Function declarations that are shared between scripts
#define FILE_SIZE_GB 1
#define BUFFER_SIZE 1024
#define CHUNK_SIZE 1024

int isNumberPresent(long arr[], int size, long target);

double findMax(double arr[], int size);

double findMin(double arr[], int size);

double findAverage(double arr[], int size);

long get_num_chunks(long file_size);

long get_file_size(const char* filepath);

int read_chunk(char* main_data, FILE* file, long start, int cur_chunk_size);

#endif