#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>

typedef struct {
    uint64_t size;
    uint8_t *buffer;
} FileContent;

static uint64_t get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

static FileContent read_entire_file(char *file_path) {
    FileContent file_content;
    FILE *file = fopen(file_path, "rb");
    if (file != NULL) {
        file_content.size = get_file_size(file);
        file_content.buffer = malloc(file_content.size);
        // The entire content of the file is loaded into RAM
        fread(file_content.buffer, file_content.size, 1, file);
        fclose(file);
    } else {
        fprintf(stderr, "Could not read file at path: %s\n", file_path);
        file_content.size = 0;
        file_content.buffer = NULL;
    }
    return file_content;
}

static uint64_t get_num_lines(char *in_file_path) {
    FileContent in_file_content = read_entire_file(in_file_path);
    uint64_t num_lines = 0;
    uint64_t count = in_file_content.size / 64;
    uint8_t *buffer = in_file_content.buffer;
    __m512i new_line_vec = _mm512_set1_epi8('\n');
    while (count--) {
        __m512i buffer_vec = _mm512_loadu_epi8(buffer);
        __mmask64 new_line_mask = _mm512_cmpeq_epi8_mask(buffer_vec, new_line_vec);
        num_lines += _mm_popcnt_u64(new_line_mask);
        buffer += 64;
    }
    uint64_t remaining = in_file_content.size % 64;
    while (remaining--) {
        num_lines += *buffer == '\n';
        buffer++;
    }
    return num_lines;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: linecount_avx512 [in_file_path]\n");
        return 1;
    }

    char *in_file_path = argv[1];

    uint64_t num_lines  = get_num_lines(in_file_path);
    printf("Number of lines: %llu\n", num_lines);
}
