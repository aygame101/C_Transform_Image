#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// On crée une fonction pour faire tourner une image
GrayImage rotateImage(const GrayImage *image, int angle)
{
    double radian_angle = angle * (M_PI / 180.0);

    int width = image->width;
    int height = image->height;

    int new_width = (int)(fabs(width * cos(radian_angle)) + fabs(height * sin(radian_angle)));
    int new_height = (int)(fabs(width * sin(radian_angle)) + fabs(height * cos(radian_angle)));

    GrayImage rotatedImage;
    rotatedImage.width = new_width;
    rotatedImage.height = new_height;
    rotatedImage.pixels = (unsigned char *)malloc(new_width * new_height);

    if (rotatedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    double cx = width / 2.0;
    double cy = height / 2.0;

    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            double src_x = (x - new_width / 2.0) * cos(radian_angle) - (y - new_height / 2.0) * sin(radian_angle) + cx;
            double src_y = (x - new_width / 2.0) * sin(radian_angle) + (y - new_height / 2.0) * cos(radian_angle) + cy;

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

                double interpolated_value = (1.0 - dx) * (1.0 - dy) * pixel00 +
                                            dx * (1.0 - dy) * pixel01 +
                                            (1.0 - dx) * dy * pixel10 +
                                            dx * dy * pixel11;

                rotatedImage.pixels[y * new_width + x] = (unsigned char)interpolated_value;
            }
            else
            {
                rotatedImage.pixels[y * new_width + x] = 255;
            }
        }
    }

    // On renvoie l'image avec la rotation appliquée.
    return rotatedImage;
}