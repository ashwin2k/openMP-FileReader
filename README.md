# Multithreaded File Reader

Repository for CSCI-GA.3033-034 Multicore Processors: Architecture and Programming - Fall 2023  - NYU.

## Files
| FileName | Description | Compilation | Usage |
|----------|----------|----------|----------|
|sequential.c | Source code for sequential reading of a file | `gcc sequential.c common.c -o sequential` |`./sequential <input file>`|
|chunkreader.c | Source code for multi threaded chunked reading of a file | `gcc -fopenmp chunkreader.c common.c -o chunkreader` |`./chunkreader <input file> <num threads>`|
|filemirroring.c | Source code for multi threaded - file mirrored (RAID 1 inspired) reading of a file | `gcc -fopenmp filemirroring.c common.c -o filemirroring` |`./filemirroring <input file> <num threads>`|
|mmap.c | Source code for memory mapped reading of a file | `gcc -fopenmp mmap.c common.c -o mmap` |`./mmap <input file> <num threads>`|
|priority.c | Source code for priority chunk loading of file | `gcc -fopenmp priority.c -o priority` |`./priority`|
|gen_input.c | Helper file for generating random txt file | `gcc gen_input.c common.c -o gen_input` |`./gen_input <file_size_GB> <num_copies>`|
|diffchecker.py | File for comparing file differences | --- |`python diffchecker.py`|
|random_csv_generator.c | Helper file for generating random CSV file | `gcc random_csv_generator.c -o random_csv_generator` |`./random_csv_generator`|


## Other files

| FileName | Description |
|----------|----------|
| common.h | Header file containing utility functions |
| common.c | Header file containing utility functions definitions |

## How to run?
 - **If you are NYU CIMS crunchy1, make sure you run `module avail gcc-9.2`**
 - Before running `filemirroring` or `chunkreader` or `mmap` or `sequential`, make sure you run the `gen_input` script to generate the input file.
 - Before running the `priority` script, make sure you run `random_csv_generator` script to generate the CSV file.

