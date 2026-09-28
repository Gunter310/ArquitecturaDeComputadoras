#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <limits>
#include <pthread.h>
#include <sched.h>

// Variable de control global para detener la carga intensiva
std::atomic<bool> keep_running(true);

// Rutina intensiva de cómputo para estresar el núcleo
void cpu_stress_task(int core_id) {
    // 1. Configurar la máscara de afinidad de la CPU
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    // 2. Asignar la afinidad al hilo actual
    pthread_t current_thread = pthread_self();
    int rc = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
    if (rc != 0) {
        std::cerr << "Error al asignar la afinidad al núcleo " << core_id << std::endl;
        return;
    }

    // 3. Bucle intensivo (uso continuo del procesador al ~100%)
    while (keep_running.load(std::memory_order_relaxed)) {
        // Cómputo constante sin llamadas a sleep para forzar uso máximo de la CPU
    }
}

int main() {
    // Requerimiento 1: Detección de núcleos lógicos
    unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "==========================================\n";
    std::cout << "   CONTROL DE AFINIDAD DE CPU EN C++\n";
    std::cout << "==========================================\n";
    std::cout << "Núcleos lógicos detectados en el sistema: " << num_cores << "\n\n";

    // Requerimiento 2: Configuración por parte del usuario
    int selected_count = 0;
    std::cout << "¿Cuántos núcleos deseas utilizar? (1 - " << num_cores << "): ";
    std::cin >> selected_count;

    if (selected_count <= 0 || selected_count > static_cast<int>(num_cores)) {
        std::cerr << "Cantidad de núcleos inválida.\n";
        return 1;
    }

    std::vector<int> selected_cores(selected_count);
    std::cout << "Ingresa los índices de los núcleos (valores entre 0 y " << num_cores - 1 << "):\n";
    
    for (int i = 0; i < selected_count; ++i) {
        int core_idx;
        std::cout << "  Núcleo #" << (i + 1) << ": ";
        std::cin >> core_idx;

        if (core_idx < 0 || core_idx >= static_cast<int>(num_cores)) {
            std::cerr << "Índice de núcleo fuera de rango [0 - " << num_cores - 1 << "].\n";
            return 1;
        }
        selected_cores[i] = core_idx;
    }

    // Requerimiento 3 y 4: Creación de hilos, afinidad y estrés
    std::vector<std::thread> threads;
    threads.reserve(selected_count);

    std::cout << "\n[+] Iniciando estresador en los núcleos seleccionados...\n";
    for (int core : selected_cores) {
        threads.emplace_back(cpu_stress_task, core);
    }

    std::cout << "[!] Estrés activo al 100% en los núcleos especificados.\n";
    std::cout << "--> Presiona ENTER para finalizar todos los hilos limpiamente <--" << std::endl;

    // Limpieza de búfer de entrada
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    // Requerimiento 5: Control de finalización limpia
    std::cout << "\n[-] Deteniendo hilos...\n";
    keep_running.store(false);

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    std::cout << "[+] Todos los hilos se han cerrado de forma segura. Programa finalizado.\n";
    return 0;
}
