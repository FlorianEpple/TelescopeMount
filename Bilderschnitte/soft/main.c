#include <stdio.h>
#include <stdlib.h>

#include "./src/defines.h"
#include "./src/graph.c"
#include "./src/jpgbmp.c"

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        printf("usage %s <file> <x> <y>\n", argv[0]);
        return 1;
    }

    const char *jpegFilename = argv[1];
    int width, height;

    const unsigned int uin_x = atoi(argv[2]);
    const unsigned int uin_y = atoi(argv[3]);

    // Load JPEG image and convert to RGB bitmap
    RGBPixel **bitmap = loadJPEGImage(jpegFilename, &width, &height);

    if (bitmap)
    {
        // Now you can work with the RGB bitmap as needed

        RGBPixel hori[width];
        RGBPixel verti[height];
        int hori_i = 0, verti_i = 0;

        printf("x:%d y:%d\n", uin_x, uin_y);

        for (int _y = 1; _y < height + 1; _y++)
        {
            for (int _x = 1; _x < width + 1; _x++)
            {
                if (_x == uin_x && hori_i < width)
                {
                    hori[hori_i] = bitmap[_x - 1][_y - 1];
                    hori_i++;
                }

                if (_y == uin_y && verti_i < height)
                {
                    verti[verti_i] = bitmap[_x - 1][_y - 1];
                    verti_i++;
                }
            }
        }

        ////////////////////////////////////////////////////////////////

        int hori_d[width];

        for (int i = 0; i < width; i++)
        {
            hori_d[i] = hori[i].red + hori[i].green + hori[i].blue;
        }

        int h_dw, h_dh, hdn = sizeof(hori_d) / sizeof(hori_d[0]);

        RGBPixel **graph_hori = bitgraph(hori_d, hdn, &h_dw, &h_dh);

        createBMP(OUTPUT_HORI, h_dh, h_dw, graph_hori);

        ////////////////////////////////////////////////////////////////

        int verti_d[height];

        for (int i = 0; i < height; i++)
        {
            verti_d[i] = verti[i].red + verti[i].green + verti[i].blue;
        }

        int v_dw, v_dh, vdn = sizeof(verti_d) / sizeof(verti_d[0]);

        RGBPixel **graph_verti = bitgraph(verti_d, vdn, &v_dw, &v_dh);

        createBMP(OUTPUT_VERTI, v_dh, v_dw, graph_verti);

        ////////////////////////////////////////////////////////////////

        // Free allocated memory
        for (int i = 0; i < width; i++)
        {
            free(bitmap[i]);
        }
        free(bitmap);
    }

    return 0;
}