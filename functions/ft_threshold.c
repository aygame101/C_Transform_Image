#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// Fonction pour appliquer le seuillage à l'image
GrayImage thresholdImage(const GrayImage *image, unsigned char threshold)
{
    int width = image->width;
    int height = image->height;

    GrayImage thresholdedImage;
    thresholdedImage.width = width;
    thresholdedImage.height = height;
    thresholdedImage.pixels = (unsigned char *)malloc(width * height);

    if (thresholdedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char pixel_value = image->pixels[y * width + x];
            if (pixel_value < threshold)
            {
                thresholdedImage.pixels[y * width + x] = 0;
            }
            else
            {
                thresholdedImage.pixels[y * width + x] = 255;
            }
        }
    }

    return thresholdedImage;
}