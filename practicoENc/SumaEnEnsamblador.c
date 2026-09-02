#include <stdio.h>
#define TAMANO 5

int main(void) {
    int arreglo[TAMANO] = {10, 20, 30, 40, 50};
    int suma = 0;

    __asm__ (
        "movq $0, %%rcx\n\t"        // %%rcx = contador del bucle (i = 0)
        "movl $0, %%eax\n\t"        // %%eax = acumulador de la suma (suma = 0)
        "mi_bucle_suma:\n\t"        // Inicio de la etiqueta del bucle
        "cmpq %1, %%rcx\n\t"        // Compara 'i' (%%rcx) con TAMANO (%1)
        "jge fin_bucle_suma\n\t"    // Si i >= TAMANO, sal del bucle

        // Sumar: suma += arreglo[i]
        "addl (%2, %%rcx, 4), %%eax\n\t" // %%eax += arreglo[i] (base + indice*tamaño_elemento)

        "incq %%rcx\n\t"            // i++ (incrementa contador)
        "jmp mi_bucle_suma\n\t"     // Salta de vuelta al inicio del bucle
        "fin_bucle_suma:\n\t"
        "movl %%eax, %0\n\t"        // suma = %%eax (guarda el resultado final)
        : "=r" (suma)               // %0 = salida: variable suma
        : "r" ((long)TAMANO), "r" (arreglo) // %1 = TAMANO, %2 = direccion base del arreglo
        : "%eax", "%rcx", "memory"  // Registros modificados + avisa modificacion de memoria
    );

    // Verificacion e impresion de los valores
    printf("Contenido del arreglo:\n");
    for (int i = 0; i < TAMANO; i++) {
        printf("arreglo[%d] = %d\n", i, arreglo[i]);
    }
    printf("Suma total = %d\n", suma);

    return 0;
}
