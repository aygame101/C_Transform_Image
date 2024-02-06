#include <stdio.h>
#include <stdlib.h>

#include "library.h"

// une fonction pour charger une image au format .pgm
GrayImage loadPGM(const char *filename)
{
    // ouvre fichier .pgm pour le lire en mode binaire ("rb").
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        // message d'erreur et on arrête le programme.
        perror("Erreur lors de l'ouverture du fichier .pgm");
        exit(1);
    }

    // vérification si le fichier a le bon format en lisant les deux premiers caractères.
    char magic[3];
    fscanf(file, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '5')
    {
        // Si format n'est pas le bon, message d'erreur et arret du programme.
        fprintf(stderr, "Le fichier n'est pas au format PGM P5.\n");
        exit(1);
    }

    // On lit la largeur, la hauteur et la valeur maximale des pixels de l'image.
    int width, height, max_val;
    fscanf(file, "%d %d", &width, &height);
    fscanf(file, "%d", &max_val);

    // On vérifie si la valeur maximale des pixels ne dépasse pas 255 (images 8 bits).
    if (max_val > 255)
    {
        // Si valeur maximale est trop grande, on affiche un message d'erreur et on arrête le programme.
        fprintf(stderr, "La valeur maximale de pixel dépasse 255, ce programme ne supporte que les images 8 bits.\n");
        exit(1);
    }

    // On crée une structure pour stocker les informations de l'image.
    GrayImage image;
    image.width = width;
    image.height = height;
    // On alloue de la mémoire pour stocker les pixels de l'image.
    image.pixels = (unsigned char *)malloc(width * height);

    // On vérifie si l'allocation de mémoire a réussi.
    if (image.pixels == NULL)
    {
        // Si l'allocation de mémoire a échoué, on affiche un message d'erreur et on arrête le programme.
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // On lit les pixels de l'image à partir du fichier.
    fread(image.pixels, 1, width * height, file);

    // On ferme le fichier après avoir lu l'image.
    fclose(file);

    // On renvoie l'image chargée.
    return image;
}

// On crée une fonction pour sauvegarder une image au format .pgm
void savePGM(const char *filename, const GrayImage *image)
{
    // On ouvre un fichier en mode écriture binaire ("wb").
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        // Si on ne peut pas ouvrir le fichier, on affiche un message d'erreur et on arrête le programme.
        perror("Erreur lors de l'ouverture du fichier .pgm");
        exit(1);
    }

    // On écrit l'en-tête de l'image au format PGM dans le fichier.
    fprintf(file, "P5\n%d %d\n255\n", image->width, image->height);

    // On écrit les pixels de l'image dans le fichier.
    fwrite(image->pixels, 1, image->width * image->height, file);

    // On ferme le fichier après avoir sauvegardé l'image.
    fclose(file);
}