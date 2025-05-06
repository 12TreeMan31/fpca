#ifndef BITMAP_H
#define BITMAP_H

// http://justsolve.archiveteam.org/wiki/BMP
#include <stdint.h>

// Magic bytes for BMP header. Others exist but this is the most common
#define BF_MAGIC 0x4d42

// biCompressionTypes
#define BI_RGB 0
#define BI_BITFIELDS 1

typedef struct BITMAPFILEHEADER {
    uint16_t bfType;      // Magic bytes; must be 0x4d42
    uint32_t bfSize;      // Size of file
    uint16_t bfReserved1; // Must be 0
    uint16_t bfReserved2; // Must be 0
    uint32_t bfOffBits;   // Byte offset from start of BITMAPFILEHEADER to image data
} __attribute__((packed)) BITMAPFILEHEADER;

// Special Handling for 1 and 0 on bpp
typedef struct BITMAPINFOHEADER {
    uint32_t biSize;         // Size of this header
    int32_t biWidth;         // Width in pixels
    int32_t biHeight;        // Height in pixels - If negative origin is upper-left, otherwise bottom left
    uint16_t biPlanes;       // Number of planes MUST BE 1
    uint16_t biBitCount;     // Bits per pixel
    uint32_t biCompression;  // Compression type used should be BI_RGB
    uint32_t biSizeImage;    // Size of bit data
    int32_t biXPelsPerMeter; // ??
    int32_t biYPelsPerMeter; // ??
    uint32_t biClrUsed;      // Amount of colors used; if 0 then maxed used
    uint32_t biClrImportant; // ??
} __attribute__((packed)) BITMAPINFOHEADER;

// Put into array of length color_count found in BMPINFOHEADER. Note: This ordering changes depending on bpp which we do not support
typedef struct RGBQUAD {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} __attribute__((packed)) RGBQUAD;

struct bmp_full_header {
    BITMAPFILEHEADER file_info;
    BITMAPINFOHEADER image_info;
    RGBQUAD *colors;
    void *image_bits;
} __attribute__((packed));

struct bmp_full_header *bitmap_read(const char *path);
int bitmap_write(const char *path, struct bmp_full_header * restrict header);
void bitmap_free(struct bmp_full_header * restrict header);

#endif