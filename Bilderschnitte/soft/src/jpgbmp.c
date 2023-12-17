#ifndef JPGBMP_C
#define JPGBMP_C

#include <stdio.h>
#include <stdlib.h>
#include <FreeImage.h>

#include "RGBPixelStruct.h"

// Function to load a JPEG image and convert it to an RGB bitmap
RGBPixel **loadJPEGImage(const char *filename, int *width, int *height)
{
    FreeImage_Initialise(0);

    // Load the image
    FIBITMAP *image = FreeImage_Load(FIF_JPEG, filename, JPEG_DEFAULT);
    if (!image)
    {
        fprintf(stderr, "Error loading image: %s\n", filename);
        FreeImage_DeInitialise();
        return NULL;
    }

    // Get image dimensions
    *width = FreeImage_GetWidth(image);
    *height = FreeImage_GetHeight(image);

    // Allocate memory for the bitmap
    RGBPixel **bitmap = (RGBPixel **)malloc(*height * sizeof(RGBPixel *));
    for (int i = 0; i < *height; i++)
    {
        bitmap[i] = (RGBPixel *)malloc(*width * sizeof(RGBPixel));
    }

    // Convert image to RGB bitmap
    for (int y = 0; y < *height; y++)
    {
        for (int x = 0; x < *width; x++)
        {
            RGBQUAD color;
            FreeImage_GetPixelColor(image, x, y, &color);

            bitmap[x][y].red = color.rgbRed;
            bitmap[x][y].green = color.rgbGreen;
            bitmap[x][y].blue = color.rgbBlue;
        }
    }

    // Unload the image
    FreeImage_Unload(image);

    FreeImage_DeInitialise();

    return bitmap;
}

#endif // JPGBMP_C