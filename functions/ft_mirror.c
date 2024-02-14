#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// On crée une fonction pour faire un effet miroir sur une image
GrayImage mirrorImage(const GrayImage *image)
{
    GrayImage mirroredImage;
    mirroredImage.width = image->width;
    mirroredImage.height = image->height;

    mirroredImage.pixels = (unsigned char *)malloc(image->width * image->height);


    for (int i = 0; i < image->height; i++)
    {
        
        for (int j = 0; j < image->width; j++)
        {
            
            mirroredImage.pixels[i * image->width + j] = image->pixels[i * image->width + (image->width - 1 - j)];
        }
    }

    // On renvoie l'image avec l'effet miroir.
    return mirroredImage;
}