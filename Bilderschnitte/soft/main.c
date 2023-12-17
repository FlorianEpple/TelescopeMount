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
        printf("width: %d, height: %d\n", width, height);
        printf("x:%d y:%d\n", uin_x, uin_y);

        ////////////////////////////////////////////////////////////////

        RGBPixel hori[width];
        RGBPixel verti[height];

        ////////////////////////////////////////////////////////////////

        for (int i = height; i > 0; i--)
        {
            if (i == uin_y)
            {
                for (int j = 0; j < width; j++)
                {
                    printf("hori:  x:%3d, y:%d: (%3d %3d %3d)\n", j, i, bitmap[i][j].red, bitmap[i][j].green, bitmap[i][j].blue);
                    hori[j] = bitmap[i][j];
                }
            }
        }

        printf("\n\n");

        ////////////////////////////////////////////////////////////////

        for (int i = 0; i < width; i++)
        {
            if (i == uin_x)
            {
                for (int j = 0; j < height; j++)
                {
                    printf("verti: x:%d, y:%3d: (%3d %3d %3d)\n", i, j, bitmap[j][i].red, bitmap[j][i].green, bitmap[j][i].blue);
                    verti[j] = bitmap[j][i];
                }
            }
        }

        printf("\n\n");

        ////////////////////////////////////////////////////////////////

        for (int i = height; i > 0; i--)
        {
            for (int j = 0; j < width; j++)
            {
                if (i == uin_y || j == uin_x)
                {
                    bitmap[j][i].red = 255;
                    bitmap[j][i].green = bitmap[j][i].blue = 0;
                }
            }
        }

        createBMP("./outp/source.bmp", height, width, bitmap);

        ////////////////////////////////////////////////////////////////

        int hori_d[height];

        for (int i = 0; i < height; i++)
        {
            hori_d[i] = hori[i].red + hori[i].green + hori[i].blue;

            if (i == uin_y)
            {
                hori_d[i] = 255 * 3 + 1; // mark the cursor position
            }
        }

        int h_dw, h_dh, hdn = sizeof(hori_d) / sizeof(hori_d[0]);

        printf("max in hori: %d\n", maxnum(hori_d, hdn));

        RGBPixel **graph_hori = bitgraph(hori_d, hdn, &h_dw, &h_dh, GRAPH_MAX);

        createBMP(OUTPUT_HORI, h_dh, h_dw, graph_hori);

        ////////////////////////////////////////////////////////////////

        int verti_d[width];

        for (int i = 0; i < width; i++)
        {
            verti_d[i] = verti[i].red + verti[i].green + verti[i].blue;

            if (i == uin_x)
            {
                verti_d[i] = 255 * 3 + 1; // mark the cursor position
            }
        }

        int v_dw, v_dh, vdn = sizeof(verti_d) / sizeof(verti_d[0]);

        printf("max in verti: %d\n", maxnum(verti_d, vdn));

        RGBPixel **graph_verti = bitgraph(verti_d, vdn, &v_dw, &v_dh, GRAPH_MAX);

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