# Protocol gzip

## Compile

gcc -o main protocol_pre_processor.c -lz

## Deflate Functions

### EXTERN int ZEXPORT deflateInit2 OF((z_streamp strm,
                                     int  level,
                                     int  method,
                                     int  windowBits,
                                     int  memLevel,
                                     int  strategy));

The level parameter specifies the compression level must be Z_DEFAULT_COMPRESSION, or
between 0 and 9:1 gives best speed, 9 gives best compression, 0 gives no compression 
at all (the input data is simply copied a block at a time).  Z_DEFAULT_COMPRESSION 
requests a default compromise between speed and compression
(currently equivalent to level 6).

The method parameter is the compression method. It must be Z_DEFLATED in this version 
of the library.

The windowBits parameter is the base two logarithm of the window size (the size of
the history buffer). It should be in the range 8..15 for this version of the 
library. Larger values of this parameter result in better compression at the expense
of memory usage. The default value is 15 if deflateInit is used instead.
WindowBits can also be greater than 15 for optional gzip encoding. Add 16 to 
windowBits to write a simple gzip header and trailer around the compressed data 
instead of a zlib wrapper. The gzip header will have no file name, no extra data, 
no comment, no modification time (set to zero), no header crc, and the operating 
system will be set to the appropriate value, if the operating system was determined 
at compile time. If a gzip stream is being written, strm->adler is a CRC-32 instead 
of an Adler-32.

The memLevel parameter specifies how much memory should be allocated
for the internal compression state.  memLevel=1 uses minimum memory but is
slow and reduces compression ratio; memLevel=9 uses maximum memory for
optimal speed.  The default value is 8.  See zconf.h for total memory usage
as a function of windowBits and memLevel.

The strategy parameter is used to tune the compression algorithm. Use the value
Z_DEFAULT_STRATEGY for normal data, Z_FILTERED for data produced by a filter 
(or predictor), Z_HUFFMAN_ONLY to force Huffman encoding only (no string match), 
or Z_RLE to limit match distances to one (run-length encoding). Filtered data 
consists mostly of small values with a somewhat random distribution. In this case, 
the compression algorithm is tuned to compress them better. The effect of Z_FILTERED
is to force more Huffman coding and less string matching; it is somewhat intermediate
between Z_DEFAULT_STRATEGY and Z_HUFFMAN_ONLY. Z_RLE is designed to be almost as fast
as Z_HUFFMAN_ONLY, but give better compression for PNG image data. The strategy 
parameter only affects the compression ratio but not the correctness of the compressed
output even if it is not set appropriately. Z_FIXED prevents the use of dynamic
Huffman codes, allowing for a simpler decoder for special applications.

### ZEXTERN int ZEXPORT deflate OF((z_streamp strm, int flush));

deflate compresses as much data as possible, and stops when the input buffer 
becomes empty or the output buffer becomes full. It may introduce some output 
latency (reading input without producing any output) except when forced to flush.
If the parameter flush is set to Z_FINISH, pending input is processed, pending 
output is flushed and deflate returns with Z_STREAM_END if there was enough output
space. If deflate returns with Z_OK or Z_BUF_ERROR, this function must be called 
again with Z_FINISH and more output space (updated avail_out) but no more input 
data, until it returns with Z_STREAM_END or an error. After deflate has returned
Z_STREAM_END, the only possible operations on the stream are deflateReset or 
deflateEnd.
Z_FINISH can be used in the first deflate call after deflateInit if all the 
compression is to be done in a single step. In order to complete in one call, 
avail_out must be at least the value returned by deflateBound (see below). Then 
deflate is guaranteed to return Z_STREAM_END. If not enough output space is 
provided, deflate will not return Z_STREAM_END, and it must be called again as 
described above. 

### ZEXTERN int ZEXPORT deflateEnd OF((z_streamp strm));

All dynamically allocated data structures for this stream are freed. This function
discards any unprocessed input and does not flush any pending output.
deflateEnd returns Z_OK if success, Z_STREAM_ERROR if the stream state was inconsistent,
Z_DATA_ERROR if the stream was freed prematurely (some input or output was discarded).
In the error case, msg may be set but then points to a static string (which must not
be deallocated).

## INFLATE Functions

### ZEXTERN int ZEXPORT inflateInit2 OF((z_streamp strm, int windowBits));

windowBits can also be greater than 15 for optional gzip decoding. Add 32 to 
windowBits to enable zlib and gzip decoding with automatic header detection,
or add 16 to decode only the gzip format (the zlib format will return a Z_DATA_ERROR).
If a gzip stream is being decoded, strm->adler is a CRC-32 instead of an Adler-32.
Unlike the gunzip utility and gzread() (see below), inflate() will not automatically
decode concatenated gzip streams. inflate() will return Z_STREAM_END at the end of the
gzip stream. The state would need to be reset to continue decoding a subsequent gzip
stream. 

### ZEXTERN int ZEXPORT inflate OF((z_streamp strm, int flush));

The use of Z_FINISH is not required to perform an inflation in one step. However
it may be used to inform inflate that a faster approach can be used for the single
inflate() call. Z_FINISH also informs inflate to not maintain a sliding window if the
stream completes, which reduces inflate's memory footprint. If the stream does not
complete, either because not all of the stream is provided or not enough output space
is provided, then a sliding window will be allocated and inflate() can be called
again to continue the operation as if Z_NO_FLUSH had been used.

### ZEXTERN int ZEXPORT inflateEnd OF((z_streamp strm));

All dynamically allocated data structures for this stream are freed. This function
discards any unprocessed input and does not flush any pending output.
inflateEnd returns Z_OK if success, or Z_STREAM_ERROR if the stream state was
inconsistent. 