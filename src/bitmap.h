#ifndef BITMAP_H
#define BITMAP_H

// TAKEN FROM MY NEPNEP PROJECT
// https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
#include <stdint.h>

// Magic bytes for BMP header. Others exist but this is the most common
#define BMP_MAGIC 0x4d42

// COMPRESSION METHOD: NONE
#define BI_RGB 0

typedef struct {
    uint16_t magic;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t offset;
} __attribute__((packed)) BMPFILEHEADER;

typedef struct {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    // Bits per pixel
    uint16_t bpp;
    uint32_t compress_type;
    uint32_t image_size;
    int32_t horz_res;
    int32_t vert_res;
    uint32_t color_count;
    uint32_t important_colors;
} __attribute__((packed)) BMPINFOHEADER;

// Put into array of length color_count found in BMPINFOHEADER
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t reserved;
} __attribute__((packed)) BMPCOLORTABLE;

struct bmp_full_header {
    BMPFILEHEADER file_info;
    BMPINFOHEADER image_info;
    BMPCOLORTABLE *colors;
} __attribute__((packed));

uint8_t *bmp_read(char *path, struct bmp_full_header **header);
int bmp_write(char *path, struct bmp_full_header *header, void *data);

#endif