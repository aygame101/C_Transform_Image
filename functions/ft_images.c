#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "../library.h"

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

// Fonction pour effectuer la translation de l'image vers la droite
GrayImage translateImage(const GrayImage *image, int x)
{
    int width = image->width;
    int height = image->height;

    GrayImage translatedImage;
    translatedImage.width = width;
    translatedImage.height = height;
    translatedImage.pixels = (unsigned char *)malloc(width * height);

    if (translatedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Effectuer la translation en copiant les pixels de l'image originale
    for (int y = 0; y < height; y++)
    {
        for (int x_dest = 0; x_dest < width; x_dest++)
        {
            int x_src = (x_dest - x) % width; // Calculer la nouvelle position x_src

            if (x_src < 0)
            {
                x_src += width; // Si x_src est négatif, le ramener à la position valide
            }

            // Copier le pixel de l'image originale vers l'image traduite
            translatedImage.pixels[y * width + x_dest] = image->pixels[y * width + x_src];
        }
    }

    return translatedImage;
}

// On crée une fonction pour redimensionner une image en niveaux de gris.
// Elle prend en entrée l'image d'origine et le facteur d'échelle.

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

            // Assurer que la valeur ajustée reste dans la plage [0, 255]
            adjusted_value = fmin(255.0, fmax(0.0, adjusted_value));

            adjustedImage.pixels[y * width + x] = (unsigned char)adjusted_value;
        }
    }

    return adjustedImage;
}

// Fonction pour ajuster la luminosité de l'image
GrayImage adjustBrightness(const GrayImage *image, double brightness_factor)
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
            double adjusted_value = pixel_value * brightness_factor;

            // Assurer que la valeur ajustée reste dans la plage [0, 255]
            adjusted_value = fmin(255.0, fmax(0.0, adjusted_value));

            adjustedImage.pixels[y * width + x] = (unsigned char)adjusted_value;
        }
    }

    return adjustedImage;
}

// Fonction pour appliquer le seuillage à l'image
GrayImage thresholdImage(const GrayImage *image, unsigned char threshold)
{
    int width = image->width;
    int height = image->height;

    GrayImage thresholdedImage;
    thresholdedImage.width = width;
    thresholdedImage.height = height;
    thresholdedImage.pixels = (unsigned char *)malloc(width * height);

    if (thresholdedImage.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Parcourir les pixels de l'image et appliquer le seuillage
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char pixel_value = image->pixels[y * width + x];
            if (pixel_value < threshold)
            {
                thresholdedImage.pixels[y * width + x] = 0; // Pixel noir
            }
            else
            {
                thresholdedImage.pixels[y * width + x] = 255; // Pixel blanc
            }
        }
    }

    return thresholdedImage;
}

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