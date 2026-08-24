# Información del sistema Ubuntu

## lscpu

- **Architecture:** arquitectura del procesador; `x86_64` significa que soporta instrucciones de 64 bits.
- **CPU op-mode(s):** modos de operación que soporta el procesador: 32 y 64 bits.
- **Address sizes:** cantidad de bits utilizados para direccionar memoria: 48 bits físicos y 48 bits virtuales.
- **Byte Order:** orden en que se almacenan los bytes en memoria: Little Endian.
- **CPU(s):** cantidad de CPUs virtuales disponibles: 5.
- **On-line CPU(s) list:** CPUs virtuales activas: 0–4.
- **Vendor ID:** fabricante del procesador: AMD.
- **Model name:** modelo del procesador: AMD Ryzen 7 6800H with Radeon Graphics.
- **CPU family:** familia a la que pertenece el procesador: 25.
- **Model:** identificador del modelo del procesador: 68.
- **Thread(s) per core:** hilos por cada núcleo: 1.
- **Core(s) per socket:** núcleos asignados al procesador virtual: 5.
- **Socket(s):** cantidad de sockets/procesadores: 1.
- **Stepping:** revisión o versión del diseño del procesador: 1.
- **BogoMIPS:** valor de referencia utilizado por Linux para medir ciertas operaciones del procesador: 6388.00.
- **Flags:** conjunto de instrucciones y características que soporta el procesador, como SSE, AVX, AVX2, AES, FMA, etc.

## lspci

- **00:00.0 Host bridge:** conexión principal entre el procesador y otros componentes del sistema.
- **00:01.0 ISA bridge:** controlador para dispositivos y funciones de compatibilidad del sistema.
- **00:01.1 IDE interface:** controlador para dispositivos de almacenamiento que utilizan IDE.
- **00:02.0 VGA compatible controller:** adaptador gráfico virtual; en tu caso es VMware SVGA II Adapter.
- **00:03.0 Ethernet controller:** controlador de red Ethernet; permite la conexión de red de la máquina virtual.
- **00:04.0 System peripheral:** servicio adicional de VirtualBox para la máquina virtual.
- **00:05.0 Multimedia audio controller:** controlador de audio virtual.
- **00:06.0 USB controller:** controlador encargado de los dispositivos USB.
- **00:07.0 Bridge:** controlador que conecta diferentes partes del sistema.
- **00:0b.0 USB controller:** otro controlador USB, utilizado por la máquina virtual.
- **00:0d.0 SATA controller:** controlador para dispositivos de almacenamiento SATA, funcionando en modo AHCI.

## lsblk

- **loop0 a loop15:** dispositivos virtuales tipo loop utilizados por Snap para montar paquetes de aplicaciones aisladas de forma independiente, como Firefox, Mesa, Snap Store, etc., en puntos de montaje dentro de `/snap/`.
- **sda:** disco duro principal (virtual) del sistema con una capacidad de 25 GB.
  - **sda1:** primera partición del disco sda de 1 MB, reservada generalmente para el sector de arranque (BIOS boot / GRUB).
  - **sda2:** segunda partición del disco sda de 25 GB, montada en la raíz (`/`) donde se encuentra instalado el sistema operativo y todos sus archivos.
- **sr0:** unidad de CD/DVD ROM virtual con un tamaño de 1024 MB (1 GB).

**En resumen:** `lsblk` lista los dispositivos de almacenamiento en bloque. En tu máquina virtual, el sistema utiliza un disco virtual principal (`sda`) para Ubuntu y múltiples unidades virtuales de bucle (`loop`) creadas automáticamente por la paquetería Snap.

## lsmem

- **0x0000000000000000-0x00000000dfffffff:** primer rango de direcciones de memoria RAM con un tamaño de 3.5 GB, activo (online) y asignado a los bloques del 0 al 27.
- **0x0000000100000000-0x000000011fffffff:** segundo rango de direcciones de memoria RAM con un tamaño de 512 MB, activo (online) y asignado a los bloques del 32 al 35.
- **Memory block size (128M):** tamaño de cada bloque indivisible de memoria gestionado por el sistema (128 MB).
- **Total online memory (4G):** cantidad total de memoria RAM actualmente activa y disponible para el sistema operativo.
- **Total offline memory (0B):** memoria RAM detectada pero deshabilitada o fuera de línea (en este caso, 0 bytes).

**En resumen:** `lsmem` muestra el estado y la disposición de la memoria RAM física asignada a la máquina virtual, dividida en bloques. En tu caso, tienes un total de 4 GB de RAM disponibles y activos para Ubuntu.
