#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// On crée une fonction pour faire un effet miroir sur une image
GrayImage mirrorImage(const GrayImage *image)
{
    // On crée une nouvelle image pour stocker l'effet miroir.
    GrayImage mirroredImage;
    mirroredImage.width = image->width;
    mirroredImage.height = image->height;
    // On alloue de la mémoire pour les pixels de l'image miroir.
    mirroredImage.pixels = (unsigned char *)malloc(image->width * image->height);

    // On parcourt les lignes de l'image d'origine.
    for (int i = 0; i < image->height; i++)
    {
        // On parcourt les colonnes de l'image d'origine.
        for (int j = 0; j < image->width; j++)
        {
            // On copie les pixels de l'image d'origine de gauche à droite dans l'image miroir.
            mirroredImage.pixels[i * image->width + j] = image->pixels[i * image->width + (image->width - 1 - j)];
        }
    }

    // On renvoie l'image avec l'effet miroir.
    return mirroredImage;
}