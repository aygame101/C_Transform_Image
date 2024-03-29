#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// Fonction pour ajuster le contraste de l'image
GrayImage adjustContrast(const GrayImage *image, double contrast_factor)
{
    int width = image->width;
    int height = image->height;

    GrayImage adjustedImage;
    adjustedImage.width = width;
    adjustedImage.height = height;
    adjustedImage.pixels = (unsigned char *)malloc(width * height);

    if (adjustedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Parcourir les pixels de l'image
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            double pixel_value = (double)image->pixels[y * width + x];
            double adjusted_value = contrast_factor * (pixel_value - 127.0) + 127.0;

            // s'assurer que la valeur ajustée reste dans la plage [0, 255]
            adjusted_value = fmin(255.0, fmax(0.0, adjusted_value));

            adjustedImage.pixels[y * width + x] = (unsigned char)adjusted_value;
        }
    }

    return adjustedImage;
}