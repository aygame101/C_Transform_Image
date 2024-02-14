#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// Fonction pour effectuer la translation de l'image vers la droite
GrayImage translateImage(const GrayImage *image, int x)
{
    int width = image->width;
    int height = image->height;

    GrayImage translatedImage;
    translatedImage.width = width;
    translatedImage.height = height;
    translatedImage.pixels = (unsigned char *)malloc(width * height);

    if (translatedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    for (int y = 0; y < height; y++)
    {
        for (int x_dest = 0; x_dest < width; x_dest++)
        {
            int x_src = (x_dest - x) % width;

            if (x_src < 0)
            {
                x_src += width;
            }

            translatedImage.pixels[y * width + x_dest] = image->pixels[y * width + x_src];
        }
    }

    return translatedImage;
}