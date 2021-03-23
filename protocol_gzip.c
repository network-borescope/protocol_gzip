#include "protocol_gzip.h"

int compress_protocol_data(const char *input, int inputSize, char *output, int outputSize) {
	// ignoring protocol HEADER
	char *data = input;
	int line_break_count = 0;
	while(line_break_count < 4) {
		if (*data == '\n') {
			line_break_count++;
		}
		data++;
	}

    z_stream zs;
    zs.zalloc = Z_NULL; // used to allocate the internal state
    zs.zfree = Z_NULL; // used to free the internal state
    zs.opaque = Z_NULL; // private data object passed to zalloc and zfree
    zs.avail_in = (uInt)inputSize; // remaining free space at next_in
    zs.next_in = (Bytef *)data; // next input byte
    zs.avail_out = (uInt)outputSize; // remaining free space at next_out
    zs.next_out = (Bytef *)output; // next output byte will go here

    int r = deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY);
    if(r < 0) {
    	printf("Deflate Init ERROR(%d)\n", r);
    	return 1;
    }

    r = deflate(&zs, Z_FINISH);
    if(r < 0) {
    	printf("Deflate ERROR(%d)\n", r);
    	return 1;
    }

    r = deflateEnd(&zs);
    if(r < 0) {
    	printf("Deflate End ERROR(%d)\n", r);
    	return 1;
    }

    return zs.total_out;
}

int uncompress_protocol_data(const char *input, int inputSize, char *output, int outputSize) {
    z_stream zs;
    zs.zalloc = Z_NULL; // used to allocate the internal state
    zs.zfree = Z_NULL; // used to free the internal state
    zs.opaque = Z_NULL; // private data object passed to zalloc and zfree
    zs.avail_in = (uInt)inputSize; // remaining free space at next_in
    zs.next_in = (Bytef *)input; // next input byte
    zs.avail_out = (uInt)outputSize; // remaining free space at next_out
    zs.next_out = (Bytef *)output; // next output byte will go here

    int r = inflateInit2(&zs, 15 | 16);
    if(r < 0) {
    	printf("InflateInit Init ERROR(%d)\n", r);
    	return 1;
    }

    r = inflate(&zs, Z_FINISH);
    if(r < 0) {
    	printf("Inflate ERROR(%d)\n", r);
    	return 1;
    }

    r = inflateEnd(&zs);
    if(r < 0) {
    	printf("Inflate End ERROR(%d)\n", r);
    	return 1;
    }

    return zs.total_out;
}

void uncompress_gzip_file(char *filename, char *buffer) {
	gzFile fin = gzopen(filename, "rb");
	int n;

    while((n = gzread(fin, buffer, BUFSIZE)) > 0)

    gzclose(fin);
}

int main() {
	char buffer_in[BUFSIZE];
#if 1 // compress and uncompress test
	char buffer_out[BUFSIZE];
	char buffer_out2[BUFSIZE];

	char aux[BUFSIZE];
	char *buffer_end = buffer_in;
	while(fgets(aux, BUFSIZE, stdin)) {
        while(*buffer_end != '\0') buffer_end++;
		
        strcat(buffer_end, aux);
	}

	compress_protocol_data(buffer_in, BUFSIZE, buffer_out, BUFSIZE);

	uncompress_protocol_data(buffer_out, BUFSIZE, buffer_out2, BUFSIZE);
	printf("%s\n", buffer_out2);
#else // testing python generated file
	uncompress_gzip_file("python.gz", buffer_in);
	printf("%s\n", buffer_in);
#endif
	return 0;
}