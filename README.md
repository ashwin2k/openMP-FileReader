# Multithreaded File Reader

Repository for CSCI-GA.3033-034 Multicore Processors: Architecture and Programming - Fall 2023  - NYU.

## Files
| FileName | Description | Compilation | Usage |
|----------|----------|----------|----------|
|sequential.c | Source code for sequential reading of a file | `gcc sequential.c common.c -o sequential` |`./sequential <input file>`|
|chunkreader.c | Source code for multi threaded chunked reading of a file | `gcc -fopenmp chunkreader.c common.c -o chunkreader` |`./chunkreader <input file> <num threads>`|
|raid.c | Source code for multi threaded - file mirrored (RAID 1 inspired) reading of a file | `gcc -fopenmp raid.c common.c -o raid` |`./raid <input file> <num threads>`|
|mmap.c | Source code for memory mapped reading of a file | `gcc -fopenmp mmap.c common.c -o mmap` |`./mmap <input file> <num threads>`|
|gen_input.c | Source code for memory mapped reading of a file | `gcc gen_input.c common.c -o gen_input` |`./gen_input <file_size_GB> <num_copies>`|
|diffchecker.py | File for comparing file differences | --- |`python diffchecker.py`|
|random_csv_generator.c | Input file for Loading based on priority | `gcc -o random_csv_generator random_csv_generator.c` |`./random_csv_generator`|
|priority.c | Source code for Loading based on priority, the parameters #define NUM_CHUNKS <value>, NUM_PRIORITIES <value>, NUM_THREADS <value> can be changed according to requirements | `gcc −Wall -std=c99 −fopenmp −o priority priority.c` |`./priority.c`|
## Other files

| FileName | Description |
|----------|----------|
| common.h | Header file containing utility functions |
| common.c | Header file containing utility functions definitions |

