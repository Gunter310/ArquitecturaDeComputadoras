# Cuestionario de Análisis Crítico - Laboratorio de Arquitectura

## Pregunta 1: Líneas de Caché y Hit Rate
**Enunciado:** Si una línea de caché mide 64 bytes y cada float ocupa 4 bytes, ¿cuántos accesos a memoria consecutivos aprovechan una sola carga a la caché L1 en la Fase 2? Demuestre la fórmula teórica del Hit Rate.

**Respuesta:**
Una transferencia de línea de caché L1 lee un bloque físico de 64 bytes desde la memoria principal. El número de elementos `float` cargados simultáneamente es:

$$\text{Elementos por línea} = \frac{64\text{ bytes}}{4\text{ bytes}} = 16\text{ elementos contiguos}$$

En la Fase 2 (acceso secuencial), el primer acceso a un bloque genera 1 fallo de caché (*Cache Miss*) y transfiere los 16 datos. Los **15 accesos consecutivos siguientes** aprovechan esa misma carga directamente desde la L1 (*Cache Hits*).

### Demostración del Hit Rate Teórico:
$$\text{Hit Rate} = \frac{\text{Aciertos (Hits)}}{\text{Accesos Totales}} = \frac{15}{16} = 0.9375 \quad (93.75\%)$$

---

## Pregunta 2: Localidad Temporal y Registros
**Enunciado:** En la Fase 3, ¿qué ventaja física representa almacenar `reg_a` en un registro de la FPU/ALU en lugar de releerlo desde la memoria en cada paso del bucle interior?

**Respuesta:**
Almacenar `reg_a` en un registro físico de la CPU (FPU/ALU) ofrece una ventaja de latencia y ancho de banda:

* **Latencia de Acceso:** Un registro responde en **< 1 ciclo de reloj** (< 0.5 ns), mientras que leer desde la caché L1 toma entre 4 y 5 ciclos, y la memoria RAM entre 200 y 300 ciclos.
* **Reducción de Tráfico de Bus:** Se elimina la contención en el bus de datos interno de la CPU, evitando lecturas redundantes a la jerarquía de memoria y saturación del canal de datos durante la ejecución del bucle interno.

---

## Pregunta 3: Loop Unrolling e Instrucción Level Parallelism (ILP)
**Enunciado:** ¿Por qué el desenrollado de bucles en la Fase 4 reduce el tiempo de ejecución incluso cuando el número total de sumas y multiplicaciones matemáticas es exactamente el mismo?

**Respuesta:**
El desenrollado de bucles no altera la cantidad de operaciones aritméticas, pero acelera la ejecución mediante dos principios de arquitectura:

1. **Reducción de Overhead Control:** Disminuye en un 75% las instrucciones de control del bucle (comparaciones `i < N`, incrementos `i++` y saltos condicionales `jmp`), liberando ciclos de reloj.
2. **Explotación de ILP (Paralelismo a Nivel de Instrucción):** Al emplear 4 acumuladores independientes (`s0`, `s1`, `s2`, `s3`), se rompe la cadena de dependencia estricta (*RAW Hazard*). Esto permite que la CPU superescalar ejecute múltiples sumas en paralelo aprovechando sus múltiples pipelines/unidades de cálculo en el mismo ciclo.

---

## Pregunta 4: Validación de Determinismo y Checksum
**Enunciado:** ¿Por qué el valor del Checksum matemático debe ser idéntico en las cuatro fases? ¿Qué indicaría si el Checksum de la Fase 4 difiere del de la Fase 1?

**Respuesta:**
El Checksum debe ser idéntico en todas las fases porque el algoritmo ejecuta la misma suma sobre la misma estructura de datos; las optimizaciones modifican **cómo** la CPU procesa los datos en hardware, no **qué** calcula matemáticamente.

Si el Checksum de la Fase 4 difiere de la Fase 1, indicaría:
* **Fallo Lógico en el Manejo del Residuo:** Error en el bucle secundario que procesa los elementos finales sobrantes cuando el tamaño $N$ no es múltiplo de 4.
* **Corrupción o Desbordamiento de Punteros:** Acceso fuera de los límites del arreglo (*out-of-bounds*) producido por una indexación errónea en el paso desenrollado (`ptr[i + 3]`).