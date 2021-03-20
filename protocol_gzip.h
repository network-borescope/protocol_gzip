#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define BUFSIZE 64000

int compress_protocol_data(const char *input, int inputSize, char *output, int outputSize);
int uncompress_protocol_data(const char *input, int inputSize, char *output, int outputSize);
void uncompress_gzip_file(char *filename, char *buffer);