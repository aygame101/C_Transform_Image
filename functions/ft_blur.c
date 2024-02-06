#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// Fonction pour appliquer un flou à l'image en fonction de la valeur de flou
GrayImage applyBlur(const GrayImage *image, int blurAmount)
{
    int width = image->width;
    int height = image->height;

    GrayImage blurredImage;
    blurredImage.width = width;
    blurredImage.height = height;
    blurredImage.pixels = (unsigned char *)malloc(width * height);

    if (blurredImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Créer un noyau de flou (ici, un noyau de moyenne)
    double kernel[3][3] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
                           {1.0 / 9, 1.0 / 9, 1.0 / 9},
                           {1.0 / 9, 1.0 / 9, 1.0 / 9}};

    // Appliquer le flou en fonction de la valeur de flou
    for (int amount = 0; amount < blurAmount; amount++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                double sum = 0.0;

                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        int newX = x + j;
                        int newY = y + i;

                        // Assurer que nous sommes à l'intérieur de l'image
                        if (newX >= 0 && newX < width && newY >= 0 && newY < height)
                        {
                            sum += kernel[i + 1][j + 1] * image->pixels[newY * width + newX];
                        }
                    }
                }

                blurredImage.pixels[y * width + x] = (unsigned char)sum;
            }
        }
    }

    return blurredImage;
}