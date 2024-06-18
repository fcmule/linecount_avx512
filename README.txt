A simple command line utility to count the number of lines in a file.
Requires a 64-bit processor with AVX-512 extensions support.

The utility has been benchmarked and compared with the equivalent "wc -l" command using the "enwik9" data,
obtaining the following results:

Command                   | Time (10 runs avg)
------------------------------------------------
"wc -l enwik9"            | 1.531 s +/- 0.037 s
------------------------------------------------
"linecount_avx512 enwik9" | 0.684 s +/- 0.016 s
------------------------------------------------

Usage:

./build.sh
./build/linecount_avx512 resources/tinyshakespeare.txt
