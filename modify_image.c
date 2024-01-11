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

// Fonction pour vérifier si le fichier a l'extension .PPM ou .PGM
int hasValidExtension(const char *filename) {
    const char *dot = strrchr(filename, '.'); 
    if(!dot || dot == filename) return 0;
    return strcmp(dot, ".ppm") == 0 || strcmp(dot, ".pgm") == 0;
}

void listImages(const char *directory) {
    struct dirent *entry;
    DIR *dp = opendir(directory);

    if (dp == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return;
    }


    printf("Liste des images dans le dossier:\n\n");
    while ((entry = readdir(dp)) != NULL) {
        if (hasValidExtension(entry->d_name)) {
            printf("%s\n", entry->d_name); 
        } 
    }

    closedir(dp);
}

void selectImage(const char *directory, char *selectedFile) {
    listImages(directory); 

    printf("\nEntrez le nom de l'image que vous souhaitez modifier (inclure l'extension) : ");
    scanf("%s", selectedFile);
}

int main() {
    char selectedFileName[256];

    welcome();
    selectImage(".", selectedFileName);

    return 0;
}