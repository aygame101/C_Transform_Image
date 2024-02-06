#include <stdio.h>
#include <stdlib.h>

#include "../library.h"

// Fonction pour charger une image au format .pgm
GrayImage loadPGM(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier .pgm");
        exit(1);
    }

    char magic[3];
    fscanf(file, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '5')
    {
        fprintf(stderr, "Le fichier n'est pas au format PGM P5.\n");
        exit(1);
    }

    int width, height, max_val;
    fscanf(file, "%d %d", &width, &height);
    fscanf(file, "%d", &max_val);

    if (max_val > 255)
    {
        fprintf(stderr, "La valeur maximale de pixel dépasse 255, ce programme ne supporte que les images 8 bits.\n");
        exit(1);
    }

    GrayImage image;
    image.width = width;
    image.height = height;
    image.pixels = (unsigned char *)malloc(width * height);

    if (image.pixels == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    fread(image.pixels, 1, width * height, file);

    fclose(file);

    return image;
}

// Fonction pour sauvegarder une image au format .pgm
void savePGM(const char *filename, const GrayImage *image)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier .pgm");
        exit(1);
    }

    fprintf(file, "P5\n%d %d\n255\n", image->width, image->height);

    fwrite(image->pixels, 1, image->width * image->height, file);

    fclose(file);
}