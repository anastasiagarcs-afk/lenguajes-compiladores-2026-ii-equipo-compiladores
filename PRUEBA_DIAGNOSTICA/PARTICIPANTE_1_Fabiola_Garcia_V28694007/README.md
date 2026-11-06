# Fabiola García — Participante 1 (Líder)

**Carpeta de trabajo individual del participante. Ejercicios 1 y 2 en Python y C; Ejercicio 3 solo en C (actualización del profesor).**

| Campo | Dato |
| --- | --- |
| Nombre | Fabiola García |
| Cédula | V28694007 |
| Sección | 1 |
| Rol | Líder / Participante 1 |
| Usuario de GitHub | [`anastasiagarcs-afk`](https://github.com/anastasiagarcs-afk) |

## Enlace a Drive

Ver [`enlace_drive.txt`](enlace_drive.txt) de esta carpeta.

<!-- PENDIENTE: pegar aquí el enlace al video de defensa en Google Drive (máximo 5 minutos, horizontal, con rostro y audio) -->

---

## Cómo ejecutar las soluciones

Requisitos: **Python 3** (`python` o `py -3`) y **gcc** (MSYS2 + `mingw-w64-x86_64-gcc`).
En Windows se usa **Git Bash** para los scripts:

```bash
# Desde la raíz del repositorio:
bash PRUEBA_DIAGNOSTICA/PARTICIPANTE_1_Fabiola_Garcia_V28694007/ejercicio1/scripts/compilar_ejecutar.sh
bash PRUEBA_DIAGNOSTICA/PARTICIPANTE_1_Fabiola_Garcia_V28694007/ejercicio2/scripts/compilar_ejecutar.sh
bash PRUEBA_DIAGNOSTICA/PARTICIPANTE_1_Fabiola_Garcia_V28694007/ejercicio3/scripts/compilar_ejecutar.sh
```

Cada script: verifica las herramientas → compila el código C → ejecuta (**Python y C en Ej1 y Ej2; solo C en Ej3**) → escribe los archivos en `resultados/` → imprime `OK:` o `ERROR:`.

Ejecución manual (opcional), dentro de la carpeta de cada ejercicio:

```bash
# Ejercicio 1
python python/caballo.py validar a1 "Kc2 Kd4 Kb3 Kc5"
python python/caballo.py pruebas scripts/ejemplos.txt
python python/test_caballo.py                    # pruebas unitarias
gcc -O2 -Wall -o c/caballo.exe c/caballo.c && ./c/caballo.exe pruebas scripts/ejemplos.txt

# Ejercicio 2
python python/polinomio.py demo
python python/polinomio.py poly 100
python python/polinomio.py tiempo 100
gcc -O2 -Wall -o c/polinomio.exe c/polinomio.c && ./c/polinomio.exe demo

# Ejercicio 3 (solo C)
gcc -O2 -Wall -o c/analizador.exe c/analizador.c && ./c/analizador.exe c/programa.c resultados/palabras_encontradas.txt resultados/programa_traducido.c
```

---

## Ejercicio 1 — Caballo de ajedrez

**Enunciado:** dado un tablero 8×8 (columnas a–h, filas 1–8), una posición inicial `Ak`
y una cadena `C` con `n` movimientos separados por espacio, verificar cada movimiento:

1. Comienza con `K` (caballo); si no, error indicando que la letra no pertenece a un caballo.
2. La casilla destino es válida (columna a–h, fila 1–8).
3. El desplazamiento es un salto de caballo (2 en una dirección y 1 en la otra)
   respecto a la posición anterior.

Se detiene en el **primer** movimiento incorrecto e indica su número (1-indexado).
Salida: `movimiento CORRECTO` o `INCORRECTO falla en el movimiento X` o el mensaje
específico si la letra no es `K`.

**Archivos:**

| Ruta | Contenido |
| --- | --- |
| `python/caballo.py` | Implementación en Python (modos `validar`, `pruebas`, `tiempo`) |
| `python/test_caballo.py` | 11 pruebas unitarias (unittest) con los casos del enunciado |
| `c/caballo.c` | Implementación en C (mismos modos, temporizador de alta resolución) |
| `scripts/ejemplos.txt` | 15 casos de prueba (formato `inicial;movimientos;esperado`) |
| `scripts/movimientos.txt` | **Lista real del PDF actualizado**: 500 movimientos, inicio `a2` |
| `scripts/compilar_ejecutar.sh` | Compila, prueba y mide tiempos |
| `resultados/tiempos_ejercicio1.txt` | Tiempos medidos (CSV) |

**Notas y supuestos:**

- **`movimientos.txt` contiene los 500 movimientos válidos del PDF actualizado**
  del profesor, con posición inicial **`a2`**. La lista fue auditada y es 100 %
  legal desde `a2`. Los casos cortos de `ejemplos.txt` demuestran la detección de
  errores (salto ilegal en movimiento 3, letra `Q`, casillas inválidas).
  Formato del archivo: línea `inicial <casilla>`, movimientos separados por espacios.
- El primer movimiento se evalúa desde la posición inicial; cada movimiento posterior
  desde la casilla anterior.
- Casillas inválidas, letras distintas de `K` y saltos no permitidos producen mensajes
  específicos; el reporte usa índice de movimiento basado en 1.

**Tiempos medidos (10 verificaciones de la lista de 500 movimientos):** ver
`resultados/tiempos_ejercicio1.txt` (formato `lenguaje,tiempo_segundos,n_movimientos,repeticiones`).

---

## Ejercicio 2 — Polinomio (x+1)ⁿ con memoria dinámica

**Enunciado:** dado un entero no negativo `n`:

a) Generar los coeficientes de `(x+1)^n` con el **triángulo de Pascal** y mostrar el polinomio.
b) Para un `x` dado, mostrar **paso a paso** el cálculo de `f(x) = (x+1)^n` con los
   coeficientes generados.

**Implementación con memoria dinámica:**

- **Python:** listas que crecen con `append` (la fila anterior se libera al construir la nueva).
- **C:** `malloc`/`realloc`/`free` para los arreglos de coeficientes.

**Supuestos:**

- Los coeficientes de `(x+1)^100` superan los 64 bits (`C(100,50) ≈ 1.0089×10²⁹`), por lo
  que en C se implementa un **entero de precisión arbitraria** (arreglo de dígitos decimales
  dinámico). El polinomio n=100 de Python y de C son **idénticos** (verificado).
- El cálculo **paso a paso** se demuestra con `n=5, x=2` (cabe en 64 bits y permite mostrar
  cada término con claridad); la generación se mide con `n=100`.
- Salida del polinomio: grados en orden descendente, términos `coeficiente*x^k`.

**Archivos:** `python/polinomio.py`, `c/polinomio.c`,
`scripts/compilar_ejecutar.sh`, y en `resultados/`:

| Archivo | Contenido |
| --- | --- |
| `polinomio_n5.txt` | Demostración (polinomio + paso a paso) de Python y C — ambos llegan a `f(2) = 243` |
| `polinomio_n100.txt` | Polinomio `(x+1)^100` completo de Python y C (idénticos) |
| `tiempos_ejercicio2.txt` | CSV `lenguaje,tiempo_segundos,n` con `n=100` |

---

## Ejercicio 3 — Palabras reservadas de C traducidas al español

**Enunciado:** leer un programa escrito en C cargado dinámicamente en memoria y, si existen
palabras reservadas, mostrar su equivalente en español. Ejemplo del profesor: encontrar
`while` en `while (x!=1)` y mostrar `Mientras`.

**Requisito principal:** detección y traducción **por palabra**, con reporte en
`resultados/palabras_encontradas.txt`:

```text
palabra_original -> Traduccion, linea N, columna M
```

La traducción en el reporte va con **mayúscula inicial** (ej. `while -> Mientras`),
siguiendo el ejemplo del profesor.

**Supuestos:**

- Se ignoran **comentarios** (`//` y `/* */`) y **cadenas** (`"..."` y `'...'`); las
  palabras reservadas dentro de ellos **no** se traducen (verificado: la cadena
  `"cadena con if while for int que no se traduce"` y los comentarios quedan intactos).
- Las subcadenas dentro de identificadores no cuentan (p. ej. `contador` no activa `for`).
- Línea y columna son **1-based** y apuntan al inicio del identificador.
- `c/programa.c` es un archivo de ejemplo **válido** (verificado con `gcc -fsyntax-only`)
  que contiene las 16 palabras de la tabla, comentarios y cadenas.

**Tabla de traducción completa (16 palabras):**

| Palabra reservada | Traducción |
| --- | --- |
| `int` | entero |
| `float` | flotante |
| `if` | si |
| `else` | sino |
| `while` | mientras |
| `for` | para |
| `return` | retornar |
| `void` | vacio |
| `char` | caracter |
| `double` | doble |
| `struct` | estructura |
| `break` | romper |
| `continue` | continuar |
| `switch` | segun |
| `case` | caso |
| `default` | defecto |

**Archivos:** `c/programa.c` (entrada obligatoria), `c/analizador.c` (carga el archivo
completo con `malloc`), `scripts/compilar_ejecutar.sh` (solo C), y en `resultados/`:

| Archivo | Contenido |
| --- | --- |
| `palabras_encontradas.txt` | **Reporte principal**: 34 ocurrencias detectadas, formato `palabra -> Traduccion, linea N, columna M` |
| `programa_traducido.c` | **EXTRA**: copia del programa con las palabras reservadas reemplazadas por su traducción en español |

> **Nota sobre `programa_traducido.c`:** es un **extra** no obligatorio. El requisito
> principal es la **detección y traducción por palabra** (el reporte). El archivo
> traducido usa las palabras en minúscula (son identificadores) y **no es un programa C
> compilable** — su propósito es mostrar visualmente el reemplazo.

**Resultado esperado:** `OK: 34 palabras reservadas encontradas (C)` al ejecutar el script.

---

## Estructura de esta carpeta

```text
PARTICIPANTE_1_Fabiola_Garcia_V28694007/
├── README.md                  # este archivo
├── enlace_drive.txt           # enlace al video de defensa (PLACEHOLDER)
├── ejercicio1/
│   ├── python/  caballo.py, test_caballo.py
│   ├── c/       caballo.c (+ caballo.exe compilado, ignorado por git)
│   ├── scripts/ compilar_ejecutar.sh, ejemplos.txt, movimientos.txt
│   └── resultados/ tiempos_ejercicio1.txt
├── ejercicio2/
│   ├── python/  polinomio.py
│   ├── c/       polinomio.c
│   ├── scripts/ compilar_ejecutar.sh
│   └── resultados/ tiempos_ejercicio2.txt, polinomio_n5.txt, polinomio_n100.txt
└── ejercicio3/
    ├── c/       analizador.c, programa.c
    ├── scripts/ compilar_ejecutar.sh
    └── resultados/ palabras_encontradas.txt, programa_traducido.c
```

## Reglas recordatorias

- La prueba diagnóstica es **individual**: este trabajo es de la Participante 1.
- **No subir videos** al repositorio: solo enlaces de Drive en `enlace_drive.txt`.
- **IA permitida**, pero debo entender y defender cada línea de este código.
- Informes y citas en **normas APA** (`docs/plantilla_informe_APA.md`).
