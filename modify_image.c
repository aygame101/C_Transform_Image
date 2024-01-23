#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 

typedef struct {
    int width;
    int height;
    unsigned char *data_r; 
    unsigned char *data_g; 
    unsigned char *data_b; 
} ColorImage;

// PGM Structure pour stocker l'image en niveaux de gris
typedef struct {
    int width;
    int height;
    unsigned char* pixels;
} GrayImage;

void welcome() {
    printf("\n               .__\n");
    printf("__  _  __ ____ |  |   ____  ____   _____   ____  \n");
    printf("\\ \\/ \\/ // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\ \n");
    printf(" \\     /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/ \n");
    printf("  \\/\\_/  \\___  >____/\\___  >____/|__|_|  /\\___  >\n");
    printf("             \\/          \\/            \\/     \\/ \n\n");
}

// Detecter si le fichier est en ppm ou pgm et executer une fonction en conséquence (fonction en double, une pour pgm et une pour ppm).

// 2. Fonction pour charger l'image au format .pgm

GrayImage loadPGM(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier .pgm");
        exit(1);
    }

    char magic[3];
    fscanf(file, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '5') {
        fprintf(stderr, "Le fichier n'est pas au format PGM P5.\n");
        exit(1);
    }

    int width, height, max_val;
    fscanf(file, "%d %d", &width, &height);
    fscanf(file, "%d", &max_val);

    if (max_val > 255) {
        fprintf(stderr, "La valeur maximale de pixel dépasse 255, ce programme ne supporte que les images 8 bits.\n");
        exit(1);
    }

    GrayImage image;
    image.width = width;
    image.height = height;
    image.pixels = (unsigned char*)malloc(width * height);

    if (image.pixels == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    fread(image.pixels, 1, width * height, file);

    fclose(file);
    return image;
}

// 3. Fonction pour générer l'image au format .pgm
void savePGM(const char* filename, const GrayImage* image) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier .pgm");
        exit(1);
    }

    fprintf(file, "P5\n%d %d\n255\n", image->width, image->height);
    fwrite(image->pixels, 1, image->width * image->height, file);

    fclose(file);
}

// 4. Fonction pour créer l'effet miroir de l'image
GrayImage mirrorImage(const GrayImage* image) {
    GrayImage mirroredImage;
    mirroredImage.width = image->width;
    mirroredImage.height = image->height;
    mirroredImage.pixels = (unsigned char*)malloc(image->width * image->height);

    for (int i = 0; i < image->height; i++) { // Parcours du tableau (comme si cetait un tableau en 2 dimensions) I
        for (int j = 0; j < image->width; j++) { // J
            mirroredImage.pixels[i * image->width + j] = image->pixels[i * image->width + (image->width - 1 - j)];
        }
    }

    return mirroredImage;
}

// Bonus. Fonction pour créer le négatif de l'image (sans miroir)
GrayImage negativeImage(const GrayImage* image) {
    GrayImage negativeImage;
    negativeImage.width = image->width;
    negativeImage.height = image->height;
    negativeImage.pixels = (unsigned char*)malloc(image->width * image->height);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            negativeImage.pixels[i * image->width + j] = 255 - image->pixels[i * image->width + j];
        }
    }

    return negativeImage;
}

void choix() {
    int choixUtilisateur;
    choixUtilisateur = 0;
    
    printf("Que souhaitez vous faire ?\n\n");
    printf("1. Miroir de l'image\n");
    printf("2. Rotation de l'image\n");

    printf("0. Quitter\n\n");
    printf("Entrez votre choix : ");
    scanf(" %d", &choixUtilisateur);

    GrayImage originalImage; // Déplacer la déclaration ici
    GrayImage mirroredImage;

    switch (choixUtilisateur) {
        case 1: {
            char filename[256] = "";

            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);

            originalImage = loadPGM(filename);
            mirroredImage = mirrorImage(&originalImage);

            strcat(filename, "_MIROIR.pgm");

            savePGM(filename, &mirroredImage);


            break;
        } case 2: {

            break;
        } case 0: {
            printf("\n\n\nAurevoir.\n\n\n");
            break;
        } default: {
            printf("\n\n\nErreur, veuillez entrer un choix valide.\n\n\n");
            choix();
            break;
        }
    }
}


int main() {
    welcome();

    choix();


}


/*
int main() {
    GrayImage originalImage = loadIMA("partiel.ima"); // On charge l'image et on la met dans originalImage
    GrayImage mirroredImage = mirrorImage(&originalImage); // On créer l'image miroir et on la met dans mirroredImage
    GrayImage negative_Image = negativeImage(&originalImage); // On créer l'image négative et on la met dans negative_Image

    GrayImage originalImage = loadIMA("partiel.pgm");
    savePGM("original_img_partiel.pgm", &originalImage); // Appel à la fonction pour sauvegarder l'image sans effet miroir
    savePGM("mirror_partiel.pgm", &mirroredImage); // Appel à la fonction pour sauvegarder l'image miroir
    savePGM("negative_partiel.pgm", &negative_Image); // Appel à la fonction pour sauvegarder l'image négative

    free(originalImage.pixels); // On libère la mémoire
    free(mirroredImage.pixels);
    free(negative_Image.pixels);

    return 0;
} */