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

    // Parcourir l'image par sections
    for (int y = 0; y < height; y += pixelHeight)
    {
        for (int x = 0; x < width; x += pixelWidth)
        {
            // Calculer la somme des valeurs des pixels à l'intérieur de la section
            double sum = 0.0;
            int count = 0;

            for (int j = 0; j < pixelHeight; j++)
            {
                for (int i = 0; i < pixelWidth; i++)
                {
                    int newX = x + i;
                    int newY = y + j;

                    // Assurer que nous sommes à l'intérieur de l'image
                    if (newX < width && newY < height)
                    {
                        sum += image->pixels[newY * width + newX];
                        count++;
                    }
                }
            }

            // Calculer la valeur moyenne des pixels dans la section
            unsigned char average = (unsigned char)(sum / count);

            // Remplacer les pixels à l'intérieur de la section par la valeur moyenne
            for (int j = 0; j < pixelHeight; j++)
            {
                for (int i = 0; i < pixelWidth; i++)
                {
                    int newX = x + i;
                    int newY = y + j;

                    // Assurer que nous sommes à l'intérieur de l'image
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