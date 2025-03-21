// Helper functions for serializing and deserializing bitmap files
#include "bitmap.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Write until it cannot
int write_all(int fd, void *buffer, size_t n) {
    int bytes_written = 0;
    int rc = -1;
    do {
        rc = write(fd, buffer + bytes_written, n - bytes_written);
        bytes_written += rc;
    } while (rc < 0 || bytes_written != n);
    return rc;
}

struct bmp_full_header *bitmap_read(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return NULL;
    }

    int rc = 0;
    struct bmp_full_header *header = malloc(sizeof(*header));

    // Reads the first 2 headers
    rc = read(fd, &header->file_info, sizeof(BITMAPFILEHEADER));
    if (rc < 0)
        goto FAIL;
    rc = read(fd, &header->image_info, sizeof(BITMAPINFOHEADER));
    if (rc < 0)
        goto FAIL;


    // Gets the color table
    int colorOffset = sizeof(BITMAPFILEHEADER) + header->image_info.biSize;
    if (header->image_info.biClrUsed == 0) {
        // Means we are using all of the colors
    }

    int colorSize = header->image_info.biClrUsed * sizeof(RGBQUAD);
    header->colors = malloc(colorSize);
    rc = lseek(fd, colorOffset, SEEK_SET);
    if (rc < 0)
        goto FAIL;
    rc = read(fd, header->colors, colorSize);
    if (rc < 0)
        goto FAIL;

    // Get bit data
    int imageSize = header->file_info.bfSize - header->file_info.bfOffBits;
    header->imageBits = malloc(imageSize);
    rc = read(fd, header->imageBits, imageSize);
    if (rc < 0)
        goto FAIL;

    // Now that we have all the data update the offsets to our format
    header->file_info.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                    (sizeof(RGBQUAD) * header->image_info.biClrUsed);
    header->image_info.biSize = sizeof(BITMAPINFOHEADER);

    return header;

FAIL:
    perror("Could not read bitmap file");
    close(fd);
    return NULL;
}

int bitmap_write(char *path, struct bmp_full_header *header) {
    // Open the file for creation, but fail if it already exists
    int fd = open(path, O_EXCL | O_CREAT | O_WRONLY, 0644);
    if (fd < 0)
        goto FAIL;

    int rc = 0;
    // Writes headers
    int infosSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    rc = write_all(fd, header, infosSize);
    if (rc < 0 || rc != infosSize)
        goto FAIL;

    int colorSize = header->image_info.biClrUsed * sizeof(RGBQUAD);
    rc = write_all(fd, header->colors, colorSize);
    if (rc < 0 || rc != colorSize)
        goto FAIL;

    int imageSize = header->file_info.bfSize - header->file_info.bfOffBits;
    rc = write_all(fd, header->imageBits, imageSize);
    if (rc < 0 || rc != imageSize)
        goto FAIL;

    return 0;

FAIL:
    perror("Could not write bitmap");
    close(fd);
    return -1;
}