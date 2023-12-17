#ifndef GRAPH_C
#define GRAPH_C

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "RGBPixelStruct.h"

// Function to create a BMP file with RGB pixel data
void createBMP(const char *filename, int height, int width, RGBPixel **pixels)
{
    // BMP file header
    uint16_t bfType = 0x4D42;                  // "BM"
    uint32_t bfSize = 54 + 3 * width * height; // Total file size
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54; // Offset to pixel data

    // BMP info header
    uint32_t biSize = 40;        // Size of info header
    int32_t biWidth = width;     // Width of the image
    int32_t biHeight = height;   // Height of the image
    uint16_t biPlanes = 1;       // Number of color planes
    uint16_t biBitCount = 24;    // Bits per pixel
    uint32_t biCompression = 0;  // Compression method (none)
    uint32_t biSizeImage = 0;    // Image size (can be 0 for uncompressed images)
    int32_t biXPelsPerMeter = 0; // Pixels per meter on X-axis (can be 0)
    int32_t biYPelsPerMeter = 0; // Pixels per meter on Y-axis (can be 0)
    uint32_t biClrUsed = 0;      // Number of used colors (for indexed images, can be 0)
    uint32_t biClrImportant = 0; // Number of important colors (can be 0)

    // Open file for writing
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write BMP file header
    fwrite(&bfType, sizeof(uint16_t), 1, file);
    fwrite(&bfSize, sizeof(uint32_t), 1, file);
    fwrite(&bfReserved1, sizeof(uint16_t), 1, file);
    fwrite(&bfReserved2, sizeof(uint16_t), 1, file);
    fwrite(&bfOffBits, sizeof(uint32_t), 1, file);

    // Write BMP info header
    fwrite(&biSize, sizeof(uint32_t), 1, file);
    fwrite(&biWidth, sizeof(int32_t), 1, file);
    fwrite(&biHeight, sizeof(int32_t), 1, file);
    fwrite(&biPlanes, sizeof(uint16_t), 1, file);
    fwrite(&biBitCount, sizeof(uint16_t), 1, file);
    fwrite(&biCompression, sizeof(uint32_t), 1, file);
    fwrite(&biSizeImage, sizeof(uint32_t), 1, file);
    fwrite(&biXPelsPerMeter, sizeof(int32_t), 1, file);
    fwrite(&biYPelsPerMeter, sizeof(int32_t), 1, file);
    fwrite(&biClrUsed, sizeof(uint32_t), 1, file);
    fwrite(&biClrImportant, sizeof(uint32_t), 1, file);

    // Write RGB pixel data
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Write pixel data
            fwrite(&pixels[x][y].blue, sizeof(uint8_t), 1, file);  // Blue
            fwrite(&pixels[x][y].green, sizeof(uint8_t), 1, file); // Green
            fwrite(&pixels[x][y].red, sizeof(uint8_t), 1, file);   // Red
        }

        // Pad row to multiple of 4 bytes (required by BMP format)
        for (int p = 0; p < (4 - (width * 3) % 4) % 4; p++)
        {
            fputc(0, file);
        }
    }

    // Close the file
    fclose(file);
}

int maxnum(int arr[], int n)
{
    int max = 0;
    for (int i = 0; i < n; i++)
        max = max < arr[i] ? arr[i] : max;
    return max;
}

RGBPixel **bitgraph(int data[], int size, int *width, int *height, int max)
{
    *width = size;
    *height = max == -1 ? maxnum(data, size) : max;

    printf("height = %d; size = %d\n", (*height), size);

    // Allocate memory for the bitmap array
    RGBPixel **bitmap = (RGBPixel **)malloc(sizeof(RGBPixel *) * (*width));
    for (int i = 0; i < *width; i++)
    {
        bitmap[i] = (RGBPixel *)malloc(sizeof(RGBPixel) * (*height));
    }

    // Initialize bitmap
    for (int i = 0; i < *width; i++)
    {
        for (int j = 0; j < *height; j++)
        {
            bitmap[i][j].red = 0;   // Red component
            bitmap[i][j].green = 0; // Green component
            bitmap[i][j].blue = 0;  // Blue component
        }
    }

    // Store data
    for (int i = 0; i < size; i++)
    {
        for (int j = *height; j > 0; j--)
        {
            if (j < data[i])
            {
                unsigned char cindex = 0xFF;

                bitmap[i][j - 1].red = cindex;
                bitmap[i][j - 1].green = cindex;
                bitmap[i][j - 1].blue = cindex;

                if (data[i] > max)
                {
                    bitmap[i][j - 1].green = bitmap[i][j - 1].blue = 0;
                }
            }
        }
    }

    return bitmap;
}

#endif // GRAPH_C