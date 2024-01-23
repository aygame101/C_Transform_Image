#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 

typedef struct {
    int width;
    int height;
    unsigned char* pixels;
} GrayImage;

typedef struct {
    int width;
    int height;
    unsigned char *data_r; 
    unsigned char *data_g; 
    unsigned char *data_b; 
} ColorImage;

void welcome() {
    printf("\n               .__\n");
    printf("__  _  __ ____ |  |   ____  ____   _____   ____  \n");
    printf("\\ \\/ \\/ // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\ \n");
    printf(" \\     /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/ \n");
    printf("  \\/\\_/  \\___  >____/\\___  >____/|__|_|  /\\___  >\n");
    printf("             \\/          \\/            \\/     \\/ \n\n");
}

// Detecter si le fichier est en ppm ou pgm et executer une fonction en conséquence (fonction en double, une pour pgm et une pour ppm).



void mirrorImage(const GrayImage* image) {
    GrayImage mirroredImage;
    mirroredImage.width = image->width;
    mirroredImage.height = image->height;
    mirroredImage.pixels = (unsigned char*)malloc(image->width * image->height);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) { 
            mirroredImage.pixels[i * image->width + j] = image->pixels[i * image->width + (image->width - 1 - j)];
        }
    }

    return mirroredImage;
}

void choix() {
    int choixUtilisateur;
    choixUtilisateur = -1;
    
    printf("Que souhaitez vous faire ?\n\n");
    printf("1. Translation d'image\n");
    printf("2. Rotation d'image\n");
    printf("3. Aggrandissement d'image\n");
    printf("4. Contraste d'image\n");
    printf("5. Luminosité d'image\n");
    printf("6. Seuillage d'image\n");
    printf("7. Flouter l'image\n");
    printf("8. Pixeliser l'image\n");
    printf("9. Passer en négatif\n");

    printf("0. Quitter\n\n");
    printf("Entrez votre choix : ");
    scanf(" %d", &choixUtilisateur);

    switch (choixUtilisateur) {
        case 1:
            // Appel fonction translation
            break;
        case 2:
            // Appel fonction rotation
            break;
        case 0:
            printf("\n\n\nAurevoir.\n\n\n");
            break;
        default:
            printf("\n\n\nChoix invalide. Veuillez entrer 0, 1 ou 2.\n\n\n");
            break;
    }
}

int main() {
    welcome();

    choix();


}