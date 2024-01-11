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

// Fonction pour vérifier si le fichier a l'extension .PPM ou .PGM
int hasValidExtension(const char *filename) {
    const char *dot = strrchr(filename, '.'); // Trouve le dernier point dans le nom du fichier
    if(!dot || dot == filename) return 0; // Pas d'extension
    return strcmp(dot, ".ppm") == 0 || strcmp(dot, ".pgm") == 0;
}

void listImages(const char *directory) {
    struct dirent *entry;
    DIR *dp = opendir(directory); // Ouvre le répertoire

    if (dp == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return;
    }

    // Parcourt tous les fichiers du répertoire
    while ((entry = readdir(dp)) != NULL) {
        if (hasValidExtension(entry->d_name)) {
            printf("%s\n", entry->d_name); // Affiche le nom du fichier
        }
    }

    closedir(dp); // Ferme le répertoire
}

int main() {
    listImages("."); // Liste les fichiers dans le répertoire courant
    return 0;
}