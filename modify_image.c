#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

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

// Fonction pour effectuer une rotation de l'image
GrayImage rotateImage(const GrayImage* image, int angle) {
    // Convertir l'angle en radians
    double radian_angle = angle * (M_PI / 180.0);

    int width = image->width;
    int height = image->height;

    // Calculer les nouvelles dimensions de l'image après la rotation
    int new_width = (int)(fabs(width * cos(radian_angle)) + fabs(height * sin(radian_angle)));
    int new_height = (int)(fabs(width * sin(radian_angle)) + fabs(height * cos(radian_angle)));

    GrayImage rotatedImage;
    rotatedImage.width = new_width;
    rotatedImage.height = new_height;
    rotatedImage.pixels = (unsigned char*)malloc(new_width * new_height);

    if (rotatedImage.pixels == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Calculer le centre de l'image d'origine
    double cx = width / 2.0;
    double cy = height / 2.0;

    // Parcourir les pixels de l'image résultante
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            // Coordonnées du pixel d'origine correspondant au pixel courant après rotation
            double src_x = (x - new_width / 2.0) * cos(radian_angle) - (y - new_height / 2.0) * sin(radian_angle) + cx;
            double src_y = (x - new_width / 2.0) * sin(radian_angle) + (y - new_height / 2.0) * cos(radian_angle) + cy;

            // Interpolation bilinéaire pour obtenir la valeur du pixel après rotation
            int x0 = (int)floor(src_x);
            int y0 = (int)floor(src_y);
            int x1 = x0 + 1;
            int y1 = y0 + 1;

            double dx = src_x - x0;
            double dy = src_y - y0;

            if (x0 >= 0 && x1 < width && y0 >= 0 && y1 < height) {
                double pixel00 = image->pixels[y0 * width + x0];
                double pixel01 = image->pixels[y0 * width + x1];
                double pixel10 = image->pixels[y1 * width + x0];
                double pixel11 = image->pixels[y1 * width + x1];

                double interpolated_value = (1.0 - dx) * (1.0 - dy) * pixel00 +
                                            dx * (1.0 - dy) * pixel01 +
                                            (1.0 - dx) * dy * pixel10 +
                                            dx * dy * pixel11;

                rotatedImage.pixels[y * new_width + x] = (unsigned char)interpolated_value;
            } else {
                // Si les coordonnées sont en dehors de l'image d'origine, remplir en blanc (valeur 255)
                rotatedImage.pixels[y * new_width + x] = 255;
            }
        }
    }

    return rotatedImage;
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

    GrayImage originalImage; // Déclaration
    GrayImage mirroredImage;
    GrayImage rotatedImage;

    switch (choixUtilisateur) {
        case 1: {
            char filename[256] = "";

            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);

            originalImage = loadPGM(filename);
            mirroredImage = mirrorImage(&originalImage);

            strcat(filename, "_MIROIR.pgm");
            savePGM(filename, &mirroredImage);

            free(originalImage.pixels);
            free(mirroredImage.pixels);

            break;
        } case 2: {
            char filename[256] = "";
            int angle = 0;

            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);

            printf("Entrez l'angle de rotation : ");
            scanf("%d", &angle);

            originalImage = loadPGM(filename);
            rotatedImage = rotateImage(&originalImage, angle);

            strcat(filename, "_ROTATE.pgm");
            savePGM(filename, &rotatedImage);

            free(originalImage.pixels);
            free(rotatedImage.pixels);
            
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