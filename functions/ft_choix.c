#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "../library.h"

void choix()
{
    int choixUtilisateur;
    choixUtilisateur = 0;
    printf("\nQue souhaitez vous faire ?\n\n");

    printf("1. Miroir de l'image\n");
    printf("2. Rotation de l'image\n");
    printf("3. Translation de l'image\n");
    printf("4. Redimensionnement (Scale) de l'image\n\n");
    printf("5. Contraste de l'image\n");
    printf("6. Luminosité de l'image\n");
    printf("7. Seuillage de l'image\n\n");
    printf("8. Flou de l'image\n");
    printf("9. Pixelisation de l'image\n");

    printf("\n\n0. Quitter\n\n");
    printf("Entrez votre choix : ");
    scanf(" %d", &choixUtilisateur);

    GrayImage originalImage; // Déclaration
    GrayImage adjustedImage;

    switch (choixUtilisateur)
    {
    case 1:
    {
        char filename[256] = "";

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        originalImage = loadPGM(filename);
        adjustedImage = mirrorImage(&originalImage);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_MIROIR.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traité.\n\n\n");

        break;
    }
    case 2:
    {
        char filename[256] = "";
        int angle = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez l'angle de rotation : ");
        scanf("%d", &angle);

        originalImage = loadPGM(filename);
        adjustedImage = rotateImage(&originalImage, angle);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_ROTATE.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traité.\n\n\n");

        break;
    }
    case 3:
    {
        char filename[256] = "";
        int x = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez la valeur de translation (vers la droite, en pixel(s)): ");
        scanf("%d", &x);

        originalImage = loadPGM(filename);
        adjustedImage = translateImage(&originalImage, x);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_TRANSLATE.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traité.\n\n\n");

        break;
    }
    case 4:
    {
        char filename[256] = "";
        double scale_factor = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez le facteur de redimensionnement (entre 0 et 1 pour réduire, supérieur à 1 pour agrandir) : ");
        scanf("%lf", &scale_factor);

        originalImage = loadPGM(filename);
        adjustedImage = scaleImage(&originalImage, scale_factor);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_SCALE.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traité.\n\n\n");

        break;
    }
    case 5:
    {
        char filename[256] = "";
        double contrast_factor = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez le facteur de contraste (entre 0 et 1 pour diminuer, supérieur à 1 pour augmenter) : ");
        scanf("%lf", &contrast_factor);

        originalImage = loadPGM(filename);
        adjustedImage = adjustContrast(&originalImage, contrast_factor);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_CONTRAST.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traité.\n\n\n");

        break;
    }
    case 6:
    {
        char filename[256] = "";
        double brightness_factor = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez le facteur de luminosité (entre 0 et 1 pour diminuer, supérieur à 1 pour augmenter) : ");
        scanf("%lf", &brightness_factor);

        originalImage = loadPGM(filename);
        adjustedImage = adjustBrightness(&originalImage, brightness_factor);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_BRIGHTNESS.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traitée.\n\n\n");

        break;
    }
    case 7:
    {
        char filename[256] = "";
        unsigned char threshold = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez la valeur de seuillage (entre 0 et 255) : ");
        scanf("%hhu", &threshold);

        originalImage = loadPGM(filename);
        adjustedImage = thresholdImage(&originalImage, threshold);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_THRESHOLD.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traitée.\n\n\n");

        break;
    }
    case 8:
    {
        char filename[256] = "";
        int blurAmount = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez le rayon du flou (plus le rayon est grand, plus le flou est important) : ");
        scanf("%d", &blurAmount);

        originalImage = loadPGM(filename);
        adjustedImage = applyBlur(&originalImage, blurAmount);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_BLUR.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été traitée.\n\n\n");

        break;
    }
    case 9:
    {
        char filename[256] = "";
        int pixelWidth = 0;
        int pixelHeight = 0;

        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        printf("Entrez la largeur de la zone de pixelisation (1: peu pixelisé - 1000: très pixelisé): ");
        scanf("%d", &pixelWidth);

        printf("Entrez la hauteur de la zone de pixelisation (1: peu pixelisé - 1000: très pixelisé): ");
        scanf("%d", &pixelHeight);

        originalImage = loadPGM(filename);
        adjustedImage = pixelizeImage(&originalImage, pixelWidth, pixelHeight);

        size_t newLength = strlen(filename) - 4;
        filename[newLength] = '\0';

        strcat(filename, "_PIXELIZED.pgm");

        char outputPath[256] = "output/";
        strcat(outputPath, filename);
        savePGM(outputPath, &adjustedImage);

        free(originalImage.pixels);
        free(adjustedImage.pixels);

        printf("\n\n\nL'image a bien été pixelisée.\n\n\n");

        break;
    }
    case 0:
    {
        printf("\n\n\nAurevoir.\n\n\n");
        break;
    }
    default:
    {
        printf("\n\n\nErreur, veuillez entrer un choix valide.\n\n\n");
        choix();
        break;
    }
    }
}