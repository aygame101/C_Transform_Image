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

// Fonction pour effectuer la translation de l'image vers la droite
GrayImage translateImage(const GrayImage* image, int x) {
    int width = image->width;
    int height = image->height;

    GrayImage translatedImage;
    translatedImage.width = width;
    translatedImage.height = height;
    translatedImage.pixels = (unsigned char*)malloc(width * height);

    if (translatedImage.pixels == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Effectuer la translation en copiant les pixels de l'image originale
    for (int y = 0; y < height; y++) {
        for (int x_dest = 0; x_dest < width; x_dest++) {
            int x_src = (x_dest - x) % width; // Calculer la nouvelle position x_src

            if (x_src < 0) {
                x_src += width; // Si x_src est négatif, le ramener à la position valide
            }

            // Copier le pixel de l'image originale vers l'image traduite
            translatedImage.pixels[y * width + x_dest] = image->pixels[y * width + x_src];
        }
    }

    return translatedImage;
}

// Fonction pour effectuer un redimensionnement (zoom) de l'image
GrayImage scaleImage(const GrayImage* image, double scale_factor) {
    int width = image->width;
    int height = image->height;

    int new_width = (int)(width * scale_factor);
    int new_height = (int)(height * scale_factor);

    GrayImage scaledImage;
    scaledImage.width = new_width;
    scaledImage.height = new_height;
    scaledImage.pixels = (unsigned char*)malloc(new_width * new_height);

    if (scaledImage.pixels == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Parcourir les pixels de l'image résultante
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            // Coordonnées du pixel d'origine correspondant au pixel courant après redimensionnement
            double src_x = x / scale_factor;
            double src_y = y / scale_factor;

            // Interpolation bilinéaire pour obtenir la valeur du pixel après redimensionnement
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

                scaledImage.pixels[y * new_width + x] = (unsigned char)interpolated_value;
            } else {
                // Si les coordonnées sont en dehors de l'image d'origine, remplir en blanc (valeur 255)
                scaledImage.pixels[y * new_width + x] = 255;
            }
        }
    }

    return scaledImage;
}

// Fonction pour ajuster le contraste de l'image
GrayImage adjustContrast(const GrayImage* image, double contrast_factor) {
    int width = image->width;
    int height = image->height;

    GrayImage adjustedImage;
    adjustedImage.width = width;
    adjustedImage.height = height;
    adjustedImage.pixels = (unsigned char*)malloc(width * height);

    if (adjustedImage.pixels == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Parcourir les pixels de l'image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
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
GrayImage adjustBrightness(const GrayImage* image, double brightness_factor) {
    int width = image->width;
    int height = image->height;

    GrayImage adjustedImage;
    adjustedImage.width = width;
    adjustedImage.height = height;
    adjustedImage.pixels = (unsigned char*)malloc(width * height);

    if (adjustedImage.pixels == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Parcourir les pixels de l'image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double pixel_value = (double)image->pixels[y * width + x];
            double adjusted_value = pixel_value * brightness_factor;

            // Assurer que la valeur ajustée reste dans la plage [0, 255]
            adjusted_value = fmin(255.0, fmax(0.0, adjusted_value));

            adjustedImage.pixels[y * width + x] = (unsigned char)adjusted_value;
        }
    }

    return adjustedImage;
}



void choix() {
    int choixUtilisateur;
    choixUtilisateur = 0;
    
    printf("Que souhaitez vous faire ?\n\n");
    printf("1. Miroir de l'image\n");
    printf("2. Rotation de l'image\n");
    printf("3. Translation de l'image\n");
    printf("4. Redimensionnement (Scale) de l'image\n\n");
    printf("5. Contraste de l'image\n");
    printf("6. Luminosité de l'image\n");

    printf("\n\n0. Quitter\n\n");
    printf("Entrez votre choix : ");
    scanf(" %d", &choixUtilisateur);

    GrayImage originalImage; // Déclaration
    GrayImage mirroredImage;
    GrayImage rotatedImage;
    GrayImage translatedImage;
    GrayImage scaledImage;
    GrayImage adjustedImage;
    GrayImage brightImage;

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

            printf("\n\n\nL'image a bien été traité.\n\n\n");

            break;
        }
        case 2: {
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

            printf("\n\n\nL'image a bien été traité.\n\n\n");

            break;
        }
        case 3: {
            char filename[256] = "";
            int x = 0;

            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);

            printf("Entrez la valeur de translation (vers la droite, en pixel(s)): ");
            scanf("%d", &x);

            originalImage = loadPGM(filename);
            translatedImage = translateImage(&originalImage, x);

            strcat(filename, "_TRANSLATE.pgm");
            savePGM(filename, &translatedImage);

            free(originalImage.pixels);
            free(translatedImage.pixels);

            printf("\n\n\nL'image a bien été traité.\n\n\n");

            break;
        }
        case 4: {
            char filename[256] = "";
            double scale_factor = 0;

            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);

            printf("Entrez le facteur de redimensionnement (entre 0 et 1 pour réduire, supérieur à 1 pour agrandir) : ");
            scanf("%lf", &scale_factor);

            originalImage = loadPGM(filename);
            scaledImage = scaleImage(&originalImage, scale_factor);

            strcat(filename, "_SCALE.pgm");
            savePGM(filename, &scaledImage);

            free(originalImage.pixels);
            free(scaledImage.pixels);

            printf("\n\n\nL'image a bien été traité.\n\n\n");

            break;
        }
        case 5: {
            char filename[256] = "";
            double contrast_factor = 0;

            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);

            printf("Entrez le facteur de contraste (entre 0 et 1 pour diminuer, supérieur à 1 pour augmenter) : ");
            scanf("%lf", &contrast_factor);

            originalImage = loadPGM(filename);
            adjustedImage = adjustContrast(&originalImage, contrast_factor);

            strcat(filename, "_CONTRAST.pgm");
            savePGM(filename, &adjustedImage);

            free(originalImage.pixels);
            free(adjustedImage.pixels);

            printf("\n\n\nL'image a bien été traité.\n\n\n");

            break;
        }
        case 6: {
            char filename[256] = "";
            double brightness_factor = 0;

            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);

            printf("Entrez le facteur de luminosité (entre 0 et 1 pour diminuer, supérieur à 1 pour augmenter) : ");
            scanf("%lf", &brightness_factor);

            originalImage = loadPGM(filename);
            adjustedImage = adjustBrightness(&originalImage, brightness_factor);

            strcat(filename, "_BRIGHTNESS.pgm");
            savePGM(filename, &adjustedImage);

            free(originalImage.pixels);
            free(adjustedImage.pixels);

            printf("\n\n\nL'image a bien été traitée.\n\n\n");

            break;
        }
        case 0: {
            printf("\n\n\nAurevoir.\n\n\n");
            break;
        }
        default: {
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