#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// Fonction pour pixeliser l'image en fonction des dimensions spécifiées
GrayImage pixelizeImage(const GrayImage *image, int pixelWidth, int pixelHeight)
{
    int width = image->width;
    int height = image->height;

    GrayImage pixelizedImage;
    pixelizedImage.width = width;
    pixelizedImage.height = height;
    pixelizedImage.pixels = (unsigned char *)malloc(width * height);

    if (pixelizedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    for (int y = 0; y < height; y += pixelHeight)
    {
        for (int x = 0; x < width; x += pixelWidth)
        {
            double sum = 0.0;
            int count = 0;

            for (int j = 0; j < pixelHeight; j++)
            {
                for (int i = 0; i < pixelWidth; i++)
                {
                    int newX = x + i;
                    int newY = y + j;

                    // s'asurer que nous sommes à l'intérieur de l'image
                    if (newX < width && newY < height)
                    {
                        sum += image->pixels[newY * width + newX];
                        count++;
                    }
                }
            }

            unsigned char average = (unsigned char)(sum / count);

            for (int j = 0; j < pixelHeight; j++)
            {
                for (int i = 0; i < pixelWidth; i++)
                {
                    int newX = x + i;
                    int newY = y + j;

                    // s'assurer que nous sommes à l'intérieur de l'image
                    if (newX < width && newY < height)
                    {
                        pixelizedImage.pixels[newY * width + newX] = average;
                    }
                }
            }
        }
    }

    return pixelizedImage;
}