# Ana Orellana — Participante 3

**Carpeta de trabajo individual del participante. Ejercicio 1 en Python; Ejercicio 2 en C y Python; Ejercicio 3 en C.**

| Campo | Dato |
| --- | --- |
| Nombre | Ana Orellana |
| Cédula | V-27902332 |
| Sección | 01 |
| Rol | Participante 3 |
| Usuario de GitHub | [`AnaO21`](https://github.com/AnaO21) |

## Enlace a Drive

Ver [`enlace_drive.txt`](enlace_drive.txt) en la raíz de esta carpeta.

<!-- PENDIENTE: pegar en el .txt el enlace al video de defensa en Google Drive (máximo 5 minutos, formato horizontal, mostrando rostro, código y ejecución) -->

---

## Cómo ejecutar las soluciones

**Requisitos:** 
* **Python 3** instalado en el PATH del sistema.
* **GCC** (vía MinGW-w64/MSYS2) configurado en las variables de entorno para compilar C.

Para facilitar la ejecución, cada ejercicio cuenta con scripts automatizados tanto para Windows (`.bat`) como para Linux/Git Bash (`.sh`).

**Ejecución automatizada (Recomendada):**
Navega a la carpeta `scripts` de cualquier ejercicio y ejecuta el archivo correspondiente a tu sistema operativo.
```bash
# Ejemplo en Windows desde la raíz del repositorio:
cd PARTICIPANTE_3_Ana_Orellana_V-27902332\ejercicio1\scripts
.\ejecutar.bat

Ejecución manual (Opcional):
# Ejercicio 1 (Python)
cd ejercicio1/python
python ejercicio1.py

# Ejercicio 2 (C y Python)
cd ejercicio2/c
gcc ejercicio2.c -o ejercicio2.exe && .\ejercicio2.exe 7 2
cd ../python
python ejercicio2.py 7 2

# Ejercicio 3 (C)
cd ejercicio3/c
gcc ejercicio3.c -o ejercicio3.exe && .\ejercicio3.exe

Nota: Todos los códigos están configurados con rutas relativas para escribir sus salidas automáticamente en sus respectivas carpetas resultados/.
Ejercicio 1 — Lógica del Caballo de Ajedrez
Enunciado: Para un tablero de ajedrez (columnas a-h, filas 1-8), dada una posición inicial y una cadena de N movimientos, el algoritmo debe verificar si los movimientos son matemáticamente correctos para un caballo. Debe detenerse al encontrar el primer error, validando sintaxis (letra 'K') y límites del tablero.
Implementación (Python):
 * La lógica se fundamenta en los deltas cartesianos: el valor absoluto de la diferencia entre la coordenada actual y la nueva debe ser (dx == 1 y dy == 2) o (dx == 2 y dy == 1).
 * Prueba de estrés: Se utiliza la librería time (perf_counter) para evaluar internamente la cadena de 500 movimientos iterada 10 veces continuas.
Archivos:
| Ruta | Contenido |
|---|---|
| python/ejercicio1.py | Código fuente con la lógica de validación y medición de rendimiento. |
| resultados/resultado_ejercicio1.txt | Salida generada automáticamente con el tiempo de ejecución en milisegundos. |
Ejercicio 2 — Polinomio (x+1)ⁿ con memoria dinámica
Enunciado: Dado un entero n, generar los coeficientes del polinomio (x+1)^n mediante el Triángulo de Pascal, evaluar la función paso a paso para un valor x dado, y medir el rendimiento computacional generando n=100 comparando C vs Python.
Implementación y Supuestos:
 * C: Utiliza punteros dobles y malloc para reservar en el Heap el espacio matricial exacto del triángulo. Imprime la pirámide numérica y realiza el cálculo paso a paso.
 * Python (Comparativa): Se ejecuta en modo append para añadir sus resultados al mismo archivo de C.
 * Fenómeno evaluado: Al calcular n=100, C nativo sufre desbordamiento de enteros (overflow) por la magnitud de las combinatorias, mientras que Python procesa los 100 niveles nativamente gracias a su manejo de enteros de precisión arbitraria.
Archivos:
| Ruta | Contenido |
|---|---|
| c/ejercicio2.c | Lógica de punteros dobles, Triángulo de Pascal y timer (clock_t). |
| python/ejercicio2.py | Generación en Python para evitar el overflow y comparar tiempos. |
| resultados/resultado_ejercicio2.txt | Archivo consolidado con la pirámide, el polinomio, el paso a paso y la métrica dual de tiempos (C vs Python). |
Ejercicio 3 — Analizador Léxico (Traductor de palabras reservadas)
Enunciado: Desarrollar un programa en lenguaje C que se cargue dinámicamente en memoria, analice un código fuente, detecte palabras reservadas nativas de C y las traduzca a su equivalente en español.
Implementación (C):
 * Manejo de archivos y memoria: Se utilizan fseek y ftell para determinar la cantidad de bytes exactos del archivo fuente (ejercicio3.c se lee a sí mismo como prueba), y se carga todo el texto en un búfer usando malloc.
 * Tokenización: Se utiliza strtok con un set de delimitadores estándar (espacios, saltos, operadores, llaves) para aislar las palabras.
 * Traducción: Se itera sobre un diccionario estático de 16 palabras reservadas. Si hay coincidencia, se exporta la traducción.
Archivos:
| Ruta | Contenido |
|---|---|
| c/ejercicio3.c | Código fuente del analizador léxico. |
| resultados/resultado_ejercicio3.txt | Reporte de salida con el tamaño en bytes de la memoria asignada y la lista de traducciones (ej. Detectado: 'int' -> Traduccion: 'entero'). |
Estructura de esta carpeta
PARTICIPANTE_3_Ana_Orellana_V-27902332/
├── README.md                  # Este archivo
├── enlace_drive.txt           # Enlace al video de defensa
├── ejercicio1/
│   ├── python/  ejercicio1.py
│   ├── scripts/ ejecutar.bat, ejecutar.sh
│   └── resultados/ resultado_ejercicio1.txt
├── ejercicio2/
│   ├── c/       ejercicio2.c
│   ├── python/  ejercicio2.py
│   ├── scripts/ ejecutar.bat, ejecutar.sh
│   └── resultados/ resultado_ejercicio2.txt
└── ejercicio3/
    ├── c/       ejercicio3.c
    ├── scripts/ ejecutar.bat, ejecutar.sh
    └── resultados/ resultado_ejercicio3.txt

Reglas recordatorias
 * La prueba diagnóstica es individual: este trabajo corresponde a la Participante 3.
 * No subir videos al repositorio: solo enlaces de Drive en enlace_drive.txt.
 * Todos los códigos manejan rutas relativas limpias. Ejecutar desde el directorio correcto (ver sección "Cómo ejecutar").
 * Informes y citas en normas APA.

