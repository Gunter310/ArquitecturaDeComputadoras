/* exp2_cola_teclado.c
 * Experimento 2: Cola circular FIFO de 8 bytes almacenada en la RAM.
 * Compilar: gcc -std=c99 -Wall -Wextra -pedantic exp2_cola_teclado.c -o exp2
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Mapa de memoria ---------- */
#define RAM_SIZE        256
#define COLA_BASE_DIR   0x70   /* 0x70 - 0x77: datos de la cola */
#define COLA_CAPACIDAD  8
#define DIR_KB_HEAD     0x78   /* Indice de lectura   */
#define DIR_KB_TAIL     0x79   /* Indice de escritura */
#define DIR_KB_COUNT    0x7A   /* Cantidad de teclas  */

static uint8_t g_ram[RAM_SIZE];

/* ---------- Prototipos ---------- */
void cola_teclado_inicializar(void);
bool cola_teclado_encolar(uint8_t ascii);
bool cola_teclado_desencolar(uint8_t *salida_ascii);
void cola_mostrar_estado(const char *titulo);

/* ---------- Inicializa punteros de la cola en RAM ---------- */
void cola_teclado_inicializar(void)
{
    int i;
    for (i = 0; i < COLA_CAPACIDAD; i++)
        g_ram[COLA_BASE_DIR + i] = 0x00;
    g_ram[DIR_KB_HEAD]  = 0;
    g_ram[DIR_KB_TAIL]  = 0;
    g_ram[DIR_KB_COUNT] = 0;
}

/* ---------- Encolar (escritura en TAIL) ---------- */
bool cola_teclado_encolar(uint8_t ascii)
{
    uint8_t tail  = g_ram[DIR_KB_TAIL];
    uint8_t count = g_ram[DIR_KB_COUNT];

    if (count >= COLA_CAPACIDAD)
        return false;                                   /* BUFFER OVERFLOW */

    g_ram[COLA_BASE_DIR + tail] = ascii;
    g_ram[DIR_KB_TAIL]  = (uint8_t)((tail + 1) % COLA_CAPACIDAD);
    g_ram[DIR_KB_COUNT] = (uint8_t)(count + 1);
    return true;
}

/* ---------- Desencolar (lectura en HEAD) ---------- */
bool cola_teclado_desencolar(uint8_t *salida_ascii)
{
    uint8_t head  = g_ram[DIR_KB_HEAD];
    uint8_t count = g_ram[DIR_KB_COUNT];

    if (count == 0)
        return false;                                   /* COLA VACIA */

    *salida_ascii = g_ram[COLA_BASE_DIR + head];
    g_ram[DIR_KB_HEAD]  = (uint8_t)((head + 1) % COLA_CAPACIDAD);
    g_ram[DIR_KB_COUNT] = (uint8_t)(count - 1);
    return true;
}

/* ---------- Muestra el contenido de 0x70..0x7A ---------- */
void cola_mostrar_estado(const char *titulo)
{
    int i;
    printf("--- %s ---\n", titulo);
    printf("  Dir : ");
    for (i = 0; i < COLA_CAPACIDAD; i++)
        printf("0x%02X ", (unsigned)(COLA_BASE_DIR + i));
    printf("\n  Dato: ");
    for (i = 0; i < COLA_CAPACIDAD; i++) {
        uint8_t v = g_ram[COLA_BASE_DIR + i];
        printf(" %c   ", (v >= 32 && v <= 126) ? (char)v : '.');
    }
    printf("\n  HEAD[0x78]=%u  TAIL[0x79]=%u  COUNT[0x7A]=%u\n\n",
           (unsigned)g_ram[DIR_KB_HEAD],
           (unsigned)g_ram[DIR_KB_TAIL],
           (unsigned)g_ram[DIR_KB_COUNT]);
}

/* ---------- Programa de prueba ---------- */
int main(void)
{
    const char *teclas = "ABCDEFGH";
    uint8_t c;
    int i;

    printf("=== Experimento 2: Cola circular FIFO en RAM ===\n\n");

    cola_teclado_inicializar();
    cola_mostrar_estado("Cola inicial (vacia)");

    /* Prueba 1: llenar la cola con 8 teclas */
    printf("Encolando \"%s\":\n", teclas);
    for (i = 0; teclas[i] != '\0'; i++) {
        bool ok = cola_teclado_encolar((uint8_t)teclas[i]);
        printf("  encolar('%c') -> %s\n", teclas[i], ok ? "OK" : "OVERFLOW");
    }
    printf("\n");
    cola_mostrar_estado("Cola llena");

    /* Prueba 2: intentar una novena tecla -> overflow */
    printf("Intentando encolar 'I' con la cola llena:\n");
    printf("  encolar('I') -> %s\n\n",
           cola_teclado_encolar('I') ? "OK" : "BUFFER OVERFLOW (rechazada)");

    /* Prueba 3: sacar 3 teclas (orden FIFO) */
    printf("Desencolando 3 teclas:\n");
    for (i = 0; i < 3; i++) {
        if (cola_teclado_desencolar(&c))
            printf("  desencolar() -> '%c'\n", (char)c);
    }
    printf("\n");
    cola_mostrar_estado("Despues de sacar 3");

    /* Prueba 4: encolar 3 mas -> TAIL da la vuelta (circular) */
    printf("Encolando \"XYZ\" (TAIL vuelve al inicio):\n");
    teclas = "XYZ";
    for (i = 0; teclas[i] != '\0'; i++) {
        bool ok = cola_teclado_encolar((uint8_t)teclas[i]);
        printf("  encolar('%c') -> %s\n", teclas[i], ok ? "OK" : "OVERFLOW");
    }
    printf("\n");
    cola_mostrar_estado("Despues de la vuelta circular");

    /* Prueba 5: vaciar toda la cola */
    printf("Vaciando la cola (orden de salida): ");
    while (cola_teclado_desencolar(&c))
        printf("%c ", (char)c);
    printf("\n\n");
    cola_mostrar_estado("Cola vaciada");

    /* Prueba 6: desencolar con la cola vacia */
    printf("Intentando desencolar con la cola vacia:\n");
    printf("  desencolar() -> %s\n",
           cola_teclado_desencolar(&c) ? "OK" : "COLA VACIA (nada que leer)");

    return 0;
}
