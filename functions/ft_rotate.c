#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// On crée une fonction pour faire tourner une image
GrayImage rotateImage(const GrayImage *image, int angle)
{
    // On convertit l'angle en radians, c'est une mesure spéciale pour les angles.
    double radian_angle = angle * (M_PI / 180.0);

    // On obtient la largeur et la hauteur de l'image d'origine.
    int width = image->width;
    int height = image->height;

    // On calcule les nouvelles dimensions de l'image après la rotation.
    int new_width = (int)(fabs(width * cos(radian_angle)) + fabs(height * sin(radian_angle)));
    int new_height = (int)(fabs(width * sin(radian_angle)) + fabs(height * cos(radian_angle)));

    // On crée une nouvelle image pour stocker l'image tournée.
    GrayImage rotatedImage;
    rotatedImage.width = new_width;
    rotatedImage.height = new_height;
    // On alloue de la mémoire pour les pixels de l'image tournée.
    rotatedImage.pixels = (unsigned char *)malloc(new_width * new_height);

    // On vérifie si l'allocation de mémoire a réussi.
    if (rotatedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // On calcule le centre de l'image d'origine.
    double cx = width / 2.0;
    double cy = height / 2.0;

    // On parcourt les pixels de la nouvelle image.
    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            // On calcule les coordonnées du pixel d'origine correspondant au pixel actuel dans la nouvelle image après rotation.
            double src_x = (x - new_width / 2.0) * cos(radian_angle) - (y - new_height / 2.0) * sin(radian_angle) + cx;
            double src_y = (x - new_width / 2.0) * sin(radian_angle) + (y - new_height / 2.0) * cos(radian_angle) + cy;

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
                rotatedImage.pixels[y * new_width + x] = (unsigned char)interpolated_value;
            }
            else
            {
                // Si les coordonnées sont en dehors de l'image d'origine, on remplit avec la couleur blanche (valeur 255).
                rotatedImage.pixels[y * new_width + x] = 255;
            }
        }
    }

    // On renvoie l'image avec la rotation appliquée.
    return rotatedImage;
}