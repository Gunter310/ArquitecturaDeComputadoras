#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

const int TAM_ORIGINAL = 1000;
const int TAM_SUBARREGLO = 100;
const int INDICE_INICIO = 500;
const int INDICE_FIN = 599;

void mostrarPrimerosYUltimos(int arreglo[], int tam, int n) {
    cout << "Primeros " << n << " elementos: ";
    for(int i = 0; i < n; i++) {
        cout << arreglo[i] << " ";
    }
    cout << "\nUltimos " << n << " elementos: ";
    for(int i = tam - n; i < tam; i++) {
        cout << arreglo[i] << " ";
    }
    cout << endl;
}

int main() {

    int arregloOriginal[TAM_ORIGINAL];
    srand(time(nullptr));
    for(int i = 0; i < TAM_ORIGINAL; i++) {
        arregloOriginal[i] = rand() % 1000;
    }
    
    cout << "=== Estado del arreglo original ANTES del traslado ===" << endl;
    mostrarPrimerosYUltimos(arregloOriginal, TAM_ORIGINAL, 5);
    
    int arregloDestino[TAM_SUBARREGLO];
    
    memcpy(arregloDestino, &arregloOriginal[INDICE_INICIO], TAM_SUBARREGLO * sizeof(int));
    
    cout << "\n=== Elementos transferidos al segundo arreglo (100 elementos) ===" << endl;
    for(int i = 0; i < TAM_SUBARREGLO; i++) {
        cout << arregloDestino[i] << " ";
    }
    cout << endl;
    
    int ceros[TAM_SUBARREGLO] = {0};
    memmove(&arregloOriginal[INDICE_INICIO], ceros, TAM_SUBARREGLO * sizeof(int));
    
    bool todosCero = true;
    for(int i = INDICE_INICIO; i <= INDICE_FIN; i++) {
        if(arregloOriginal[i] != 0) {
            todosCero = false;
            break;
        }
    }
    
    cout << "\n=== Validacion tras la operacion ===" << endl;
    cout << "Indices 500 al 599 del arreglo original en cero: " << (todosCero ? "SI" : "NO") << endl;
    
    cout << "\n=== Estado del arreglo original despues del translado ===" << endl;
    mostrarPrimerosYUltimos(arregloOriginal, TAM_ORIGINAL, 5);
    
    return 0;
}
