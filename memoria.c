#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOQUE (100 * 1024 * 1024)  // 100 MiB
#define MAX_BLOQUES 25              // máximo 2.5 GiB

int main() {
    void *memoria[MAX_BLOQUES];
    int bloques = 0;

    printf("Iniciando prueba de memoria...\n");

    while (bloques < MAX_BLOQUES) {

        memoria[bloques] = malloc(BLOQUE);

        if (memoria[bloques] == NULL) {
            printf("No se pudo reservar más memoria.\n");
            break;
        }

        // Escribimos en el bloque para que la RAM sea realmente utilizada
        memset(memoria[bloques], 1, BLOQUE);

        bloques++;

        printf("Memoria reservada: %d MiB\n", bloques * 100);
        fflush(stdout);

        sleep(2);
    }

    printf("\nPrueba terminada.\n");
    printf("Presiona ENTER para liberar la memoria...\n");
    getchar();

    for (int i = 0; i < bloques; i++) {
        free(memoria[i]);
    }

    printf("Memoria liberada.\n");

    return 0;
}
