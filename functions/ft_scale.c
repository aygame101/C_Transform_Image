#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "library.h"

// fonction pour agrandir ou retrécir une image
GrayImage scaleImage(const GrayImage *image, double scale_factor)
{
    int width = image->width;
    int height = image->height;

    int new_width = (int)(width * scale_factor);
    int new_height = (int)(height * scale_factor);

    GrayImage scaledImage;
    scaledImage.width = new_width;
    scaledImage.height = new_height;

    scaledImage.pixels = (unsigned char *)malloc(new_width * new_height);

    if (scaledImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            double src_x = x / scale_factor;
            double src_y = y / scale_factor;

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

                scaledImage.pixels[y * new_width + x] = (unsigned char)interpolated_value;
            }
            else
            {
                scaledImage.pixels[y * new_width + x] = 255;
            }
        }
    }

    // On renvoie l'image redimensionnée.
    return scaledImage;
}