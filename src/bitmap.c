// Helper functions for serializing and deserializing bitmap files
#include "bitmap.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Header used to first in order to get color table info
struct pre_header {
    BMPFILEHEADER file_info;
    BMPINFOHEADER image_info;
} __attribute__((packed));

// Write until it cannot
int write_all(int fd, void *buf, size_t n) {
    int bytes_written = 0;
    int rc = -1;
    do {
        rc = write(fd, buf, n);
        bytes_written += rc;
    } while (rc < 0 || bytes_written != n);
    return rc;
}

uint8_t *bmp_read(char *path, struct bmp_full_header **header) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return NULL;
    }

    struct pre_header pre;
    int rc = read(fd, &pre, sizeof(pre));
    if (rc < 0) {
        return NULL;
    }
    // Check for the magic bytes
    if (pre.file_info.magic != BMP_MAGIC) {
        errno = EILSEQ;
        return NULL;
    }

    // Creates color table then loads colors into it
    BMPCOLORTABLE *color_pallet = calloc(pre.image_info.color_count, sizeof(*color_pallet));
    rc = read(fd, color_pallet, sizeof(*color_pallet) * pre.image_info.color_count);
    if (rc < 0) {
        free(color_pallet);
        return NULL;
    }

    struct bmp_full_header *tmp = malloc(sizeof(*tmp));
    memcpy(&tmp->file_info, &pre.file_info, sizeof(pre.file_info));
    memcpy(&tmp->image_info, &pre.image_info, sizeof(pre.image_info));
    tmp->colors = color_pallet;

    *header = tmp;

    // Gets the start of the image data in the file
    int px_data_length = pre.file_info.file_size - pre.file_info.offset;
    uint8_t *data = calloc(px_data_length, sizeof(uint8_t));
    rc = read(fd, data, px_data_length);
    if (rc < 0) {
        return NULL;
    }

    close(fd);

    return data;
}

int bmp_write(char *path, struct bmp_full_header *header, void *data) {
    // Open the file for creation, but fail if it already exists
    int fd = open(path, O_EXCL | O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        return -fd;
    }

    // Check for the magic bytes
    if (header->file_info.magic != BMP_MAGIC) {
        errno = EILSEQ;
        close(fd);
        return -1;
    }

    struct pre_header pre;
    memcpy(&pre.file_info, &header->file_info, sizeof(pre.file_info));
    memcpy(&pre.image_info, &header->image_info, sizeof(pre.image_info));

    // Write the pre-header
    int rc = write_all(fd, &pre, sizeof(pre));
    if (rc < 0 || rc != sizeof(pre)) {
        close(fd);
        return -1;
    }

    // Write the color table
    rc = write_all(fd, header->colors, header->image_info.color_count);
    if (rc < 0 || rc != header->image_info.color_count) {
        close(fd);
        return -1;
    }

    // Write the pixel data
    int px_data_length = header->file_info.file_size - header->file_info.offset;
    rc = write_all(fd, data, px_data_length);
    if (rc < 0 || rc != px_data_length) {
        close(fd);
        return -1;
    }

    // Close the file
    if (close(fd) < 0) {
        return -1;
    }

    return 0;
}