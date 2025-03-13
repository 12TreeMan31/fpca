#include "bitmap.h"
#include "label.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// clang-format off
uint8_t tetris[] = {1, 1, 1, 1,
                    0, 0, 0, 0,
                    0, 0, 0, 1,
                    0, 1, 1, 1};
uint8_t twobar[] = {1, 1, 1, 1,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    1, 1, 1, 1};
uint8_t square[] = {0, 0, 0, 0,
                    0, 1, 1, 0,
                    0, 1, 1, 0,
                    0, 0, 0, 0};
uint8_t bigbox[] = {1, 1, 1, 1,
                    1, 0, 0, 1,
                    1, 0, 0, 1,
                    1, 1, 1, 1};
uint8_t hugbox[] = {1, 1, 1, 1,
                    1, 1, 1, 1,
                    1, 1, 1, 1,
                    1, 1, 1, 1};
uint8_t four_c[] = {1, 0, 0, 1,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    1, 0, 0, 1};
uint8_t diagn1[] = {1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1};
uint8_t diagn2[] = {1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    1, 0, 0, 1};
uint8_t crosss[] = {1, 0, 0, 1,
                    0, 1, 1, 0,
                    0, 1, 1, 0,
                    1, 0, 0, 1};        
uint8_t smilie[] = {1, 0, 0, 1,
                    0, 0, 0, 0,
                    1, 0, 0, 1,
                    0, 1, 1, 0};
uint8_t dounut[] = {0, 1, 1, 0,
                    1, 0, 0, 1,
                    1, 0, 0, 1,
                    0, 1, 1, 0};
uint8_t diskss[] = {0, 1, 1, 0,
                    1, 1, 1, 1,
                    1, 1, 1, 1,
                    0, 1, 1, 0};
// clang-format on


int main() {
    struct bmp_full_header *header = malloc(sizeof *header);
    uint8_t *px_data = bmp_read("../result_nc2.bmp", &header);

    if (px_data == NULL) {
        perror("Failed to read image");
        return 1;
    }
    /*int rc = bmp_write("./hello_world.bmp", header, px_data);
    if (rc != 0) {
        perror("Could not write file");
        return -1;
    }*/
    // ccl_uf(px_data, header->image_info.width, header->image_info.height);

    ccl_uf(tetris, 4, 4);

    /*int fd = open("../result.bmp", O_RDONLY);
    struct bmp_full_header header;
    read(fd, &header, sizeof(header));
    int data_length = header.file_info.file_size - header.file_info.offset;
    void *pixel_data = calloc(data_length, sizeof(uint8_t));
    read(fd, pixel_data, data_length);*/
}