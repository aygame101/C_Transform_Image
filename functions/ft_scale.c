#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

GrayImage scaleImage(const GrayImage *image, double scale_factor)
{
    // On récupère la largeur et la hauteur de l'image d'origine.
    int width = image->width;
    int height = image->height;

    // On calcule la nouvelle largeur et la nouvelle hauteur en fonction du facteur d'échelle.
    int new_width = (int)(width * scale_factor);
    int new_height = (int)(height * scale_factor);

    // On crée une nouvelle image redimensionnée avec les nouvelles dimensions.
    GrayImage scaledImage;
    scaledImage.width = new_width;
    scaledImage.height = new_height;
    // On alloue de la mémoire pour stocker les pixels de l'image redimensionnée.
    scaledImage.pixels = (unsigned char *)malloc(new_width * new_height);

    // Si on n'a pas pu allouer de mémoire, on affiche un message d'erreur et on quitte.
    if (scaledImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // On parcourt les pixels de la nouvelle image.
    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            // On calcule les coordonnées du pixel d'origine correspondant au pixel actuel dans la nouvelle image.
            double src_x = x / scale_factor;
            double src_y = y / scale_factor;

            // On utilise l'interpolation bilinéaire pour obtenir la valeur du pixel dans la nouvelle image.
            int x0 = (int)floor(src_x);
            int y0 = (int)floor(src_y);
            int x1 = x0 + 1;
            int y1 = y0 + 1;

            double dx = src_x - x0;
            double dy = src_y - y0;

            if (x0 >= 0 && x1 < width && y0 >= 0 && y1 < height)
            {
                double pixel00 = image->pixels[y0 * width + x0];
                double pixel01 = image->pixels[y0 * width + x1];
                double pixel10 = image->pixels[y1 * width + x0];
                double pixel11 = image->pixels[y1 * width + x1];

                // On calcule la valeur interpolée du pixel.
                double interpolated_value = (1.0 - dx) * (1.0 - dy) * pixel00 +
                                            dx * (1.0 - dy) * pixel01 +
                                            (1.0 - dx) * dy * pixel10 +
                                            dx * dy * pixel11;

                // On assigne cette valeur à l'emplacement correspondant dans la nouvelle image.
                scaledImage.pixels[y * new_width + x] = (unsigned char)interpolated_value;
            }
            else
            {
                // Si les coordonnées sont en dehors de l'image d'origine, on remplit avec la couleur blanche (valeur 255).
                scaledImage.pixels[y * new_width + x] = 255;
            }
        }
    }

    // On renvoie l'image redimensionnée.
    return scaledImage;
}