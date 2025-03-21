#include "bitmap.h"
#include "label.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint8_t tests_ccl[12][16] = {
    { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1 }, // Tetris
    { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, // 2-Bars
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0 }, // Inner Square
    { 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1 }, // Outter Square
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, // Full Box
    { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 }, // 4 Corners
    { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }, // Diaginal
    { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1 }, // Diaginal with dot
    { 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 }, // Cross
    { 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0 }, // Smile
    { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 }, // Dougnut
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 }  // Full Dougnut
}; /*            |           |           |           */

int correct[12] = {
    2, 2, 1, 1, 1, 4, 1, 2, 1, 3, 1, 1,
};

void tests() {
    for (int i = 0; i < 12; i++) {
        printf("---Test %i---\n", i);
        int rc = ccl_uf(tests_ccl[i], 4, 4);
        if (rc != correct[i])
            printf("\033[0;31mFAILED: Expected: %i, got %i \033[0;37m\n", correct[i], rc);
    }
    return;
}

int main() {
    struct bmp_full_header *header = malloc(sizeof *header);
    uint8_t *px_data = bmp_read("../result_nc2.bmp", &header);

    if (px_data == NULL) {
        perror("Failed to read image");
        return 1;
    }
    tests();
    /*int rc = bmp_write("./hello_world.bmp", header, px_data);
    if (rc != 0) {
        perror("Could not write file");
        return -1;
    }*/
    // ccl_uf(px_data, header->image_info.width, header->image_info.height);

    /*int fd = open("../result.bmp", O_RDONLY);
    struct bmp_full_header header;
    read(fd, &header, sizeof(header));
    int data_length = header.file_info.file_size - header.file_info.offset;
    void *pixel_data = calloc(data_length, sizeof(uint8_t));
    read(fd, pixel_data, data_length);*/
}