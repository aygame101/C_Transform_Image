#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 

typedef struct {
    int width;
    int height;
    unsigned char *data;
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

int choix() {
    int choixUtilisateur;
    
    printf("Que souhaitez vous faire ?\n\n");
    printf("1. Translation d'image\n");
    printf("2. Rotation d'image\n");
    printf("0. Quitter\n\n");
    printf("Entrez votre choix : ");
    scanf("%d", &choixUtilisateur);

    switch (choixUtilisateur) {
        case 1:
            // Appel fonction translation
            break;
        case 2:
            // Appel fonction rotation
            break;
        case 0:
            printf("Aurevoir.\n");
            break;
        default:
            printf("Choix invalide. Veuillez entrer 0, 1 ou 2.\n");
            break;
    }

    return choixUtilisateur;
}

int main() {
    welcome();

    choix();


}