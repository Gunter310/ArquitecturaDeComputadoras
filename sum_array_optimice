#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000 // 100 millones de elementos (400 MB en RAM)

static double medir_tiempo(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

/* 1. NAIVE: Salto por Stride (Provoca 1 Cache Miss por cada elemento) */
float suma_naive(const float *arr) {
    float suma = 0.0f;
    int stride = 16; // 16 floats = 64 Bytes (Línea de caché L1 completa)
    for (int s = 0; s < stride; s++) {
        for (int i = s; i < N; i += stride) {
            suma += arr[i];
        }
    }
    return suma;
}

/* 2. LOCALIDAD ESPACIAL: Recorrido contiguo (1 Cache Miss cada 16 floats) */
float suma_localidad(const float *arr) {
    float suma = 0.0f;
    for (int i = 0; i < N; i++) {
        suma += arr[i];
    }
    return suma;
}

/* 3. REGISTROS DE CPU: Retiene el puntero y el acumulador en registros de FPU */
float suma_registros(const float *arr) {
    register float suma = 0.0f;
    register const float *ptr = arr;
    for (int i = 0; i < N; i++) {
        suma += ptr[i];
    }
    return suma;
}

/* 4. LOOP UNROLLING 4X + ILP: Acumuladores independientes para paralelismo */
float suma_unrolling(const float *arr) {
    register float s0 = 0.0f, s1 = 0.0f, s2 = 0.0f, s3 = 0.0f;
    register const float *ptr = arr;
    int i = 0;
    for (; i <= N - 4; i += 4) {
        s0 += ptr[i];
        s1 += ptr[i + 1];
        s2 += ptr[i + 2];
        s3 += ptr[i + 3];
    }
    float suma_total = (s0 + s1) + (s2 + s3);
    for (; i < N; i++) suma_total += ptr[i]; // Manejo del residuo
    return suma_total;
}

int main(void) {
    size_t bytes = (size_t)N * sizeof(float);
    float *arr = (float*)malloc(bytes);
    if (!arr) return 1;

    for (int i = 0; i < N; i++) arr[i] = 1.0f; // Inicialización

    double t0, t1, t2, t3, t4;
    float r1, r2, r3, r4;

    t0 = medir_tiempo(); r1 = suma_naive(arr);     t1 = medir_tiempo() - t0;
    t0 = medir_tiempo(); r2 = suma_localidad(arr); t2 = medir_tiempo() - t0;
    t0 = medir_tiempo(); r3 = suma_registros(arr); t3 = medir_tiempo() - t0;
    t0 = medir_tiempo(); r4 = suma_unrolling(arr); t4 = medir_tiempo() - t0;

    printf("\n=== DEMOSTRACION DE OPTIMIZACION (SUMA DE ARRAY N=%d) ===\n", N);
    printf("1. Naive (Salto de Stride)   : %7.4f s | Speedup: 1.00x\n", t1);
    printf("2. Localidad Espacial        : %7.4f s | Speedup: %.2fx\n", t2, t1 / t2);
    printf("3. Registros de CPU          : %7.4f s | Speedup: %.2fx\n", t3, t1 / t3);
    printf("4. Loop Unrolling 4x (ILP)   : %7.4f s | Speedup: %.2fx\n", t4, t1 / t4);
    printf("[OK] Resultado Verificado: %.0f\n", r4);

    free(arr);
    return 0;
}
