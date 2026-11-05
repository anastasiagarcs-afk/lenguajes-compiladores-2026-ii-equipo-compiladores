# Cesar Velasquez — Participante 2

**Carpeta de trabajo individual del participante. Prueba diagnóstica completa en Python y C.**

| Campo | Dato |
| --- | --- |
| Nombre | Cesar Velasquez |
| Cédula | V-26592592 |
| Sección | 3 |
| Rol | Participante 2 |
| Usuario de GitHub | [`Bboyjoker15`](https://github.com/Bboyjoker15) |

## Enlace a Drive

Ver `enlace_drive.txt` de esta carpeta.

<!-- PENDIENTE: pegar el enlace al video de defensa en Google Drive (máximo 5 minutos, horizontal, con rostro y audio) -->

---

## Cómo ejecutar las soluciones

Requisitos: **Python 3** (`python`) y **gcc** (MSYS2 + `mingw-w64-ucrt-x86_64-gcc`).
En Windows se usa **Git Bash** para los scripts (el `bash` de WSL no incluye `dirname`/`mkdir`):

```bash
# Desde la raíz del repositorio:
bash PRUEBA_DIAGNOSTICA/PARTICIPANTE_2_Cesar_Velasquez_V-26592592/ejercicio1/scripts/compilar_ejecutar.sh
bash PRUEBA_DIAGNOSTICA/PARTICIPANTE_2_Cesar_Velasquez_V-26592592/ejercicio2/scripts/compilar_ejecutar.sh
bash PRUEBA_DIAGNOSTICA/PARTICIPANTE_2_Cesar_Velasquez_V-26592592/ejercicio3/scripts/compilar_ejecutar.sh
```

Cada script verifica las herramientas, compila el código C, ejecuta Python y C,
compara los resultados, escribe los archivos en `resultados/` e imprime `OK:` o `ERROR:`.

Ejecución manual (opcional), dentro de la carpeta de cada ejercicio:

```bash
# Ejercicio 1
python python/caballo.py validar a2 "Kb4 Ka6 Kc5 Ka4"
python python/caballo.py pruebas scripts/ejemplos.txt
python python/test_caballo.py
gcc -O2 -std=c11 -Wall -Wextra -pedantic -o c/caballo.exe c/caballo.c
./c/caballo.exe pruebas scripts/ejemplos.txt

# Ejercicio 2
python python/polinomio.py demo
python python/polinomio.py poly 100
python python/polinomio.py eval 5 2
python python/polinomio.py tiempo 100 10
gcc -O2 -std=c11 -Wall -Wextra -pedantic -o c/polinomio.exe c/polinomio.c
./c/polinomio.exe demo

# Ejercicio 3
python python/analizador.py c/programa.c resultados/palabras_encontradas.txt resultados/programa_traducido.c
gcc -O2 -std=c11 -Wall -Wextra -pedantic -o c/analizador.exe c/analizador.c
./c/analizador.exe c/programa.c resultados/palabras_encontradas.txt resultados/programa_traducido.c
```

---

## Ejercicio 1 — Caballo de ajedrez

**Enunciado:** dado un tablero 8×8 (columnas a–h, filas 1–8), una posición inicial
`Ak` y una cadena `C` con `n` movimientos separados por espacio, verificar cada
movimiento:

1. Debe comenzar con `K` (caballo); si no, se informa que la letra no pertenece a un caballo.
2. La casilla destino debe ser válida (columna a–h, fila 1–8).
3. El desplazamiento debe ser un salto de caballo (2 en una dirección y 1 en la otra)
   respecto a la posición anterior.

El programa se detiene en el **primer** movimiento incorrecto e indica su número
(1-indexado). Salidas: `movimiento CORRECTO`, `INCORRECTO falla en el movimiento X`
o el mensaje específico para una letra distinta de `K`.

**Archivos:**

| Ruta | Contenido |
| --- | --- |
| `python/caballo.py` | Implementación en Python (modos `validar`, `pruebas`, `tiempo`) |
| `python/test_caballo.py` | 11 pruebas unitarias (`unittest`) con los casos del enunciado |
| `c/caballo.c` | Implementación en C (mismos modos, temporizador de alta resolución) |
| `scripts/ejemplos.txt` | 11 casos de prueba en formato `inicial;movimientos;esperado` |
| `scripts/movimientos.txt` | Lista real tomada del documento, con `inicial a2` |
| `scripts/compilar_ejecutar.sh` | Compila, ejecuta, compara y mide tiempos |
| `resultados/salida_ejemplos.txt` | Salida de los casos de ejemplo en ambos lenguajes |
| `resultados/tiempos_ejercicio1.txt` | Tiempos medidos (CSV) |

**Notas y supuestos:**

- **La lista del documento tiene 224 movimientos, no 500.** El enunciado dice
  "los siguientes 500 movimientos", pero el texto suministrado contiene exactamente
  224; se usaron esos 224 sin inventar movimientos. El programa acepta cualquier cantidad.
- **Posición inicial `a2`.** El documento no indica la casilla inicial. Se verificó que
  la secuencia completa es válida desde `a2` (también desde `c2`, `a6`, `c6`, `d3` y `d5`,
  que son las vecinas del primer salto `Kb4`). Se dejó `inicial a2` en
  `scripts/movimientos.txt` para poder cambiarla fácilmente.
- El primer movimiento se evalúa desde la posición inicial; cada movimiento posterior
  desde la casilla anterior.
- Se ignoran las líneas vacías y las que empiezan con `#`.
- Medición: 10 repeticiones de la validación completa (el parseo del archivo queda fuera
  del cronómetro). Los tiempos de cada corrida se guardan en
  `resultados/tiempos_ejercicio1.txt`.

---

## Ejercicio 2 — Polinomio (x+1)^n con memoria dinámica

**Enunciado:** dado un entero no negativo `n`:

a) Generar los coeficientes de `(x+1)^n` con el **triángulo de Pascal** y mostrar el polinomio.
b) Para un `x` dado, mostrar **paso a paso** el cálculo de `f(x) = (x+1)^n` con los
   coeficientes generados, usando memoria dinámica.

**Implementación:**

- **Python:** listas que crecen con `append`/comprensión; la fila anterior se reemplaza
  al construir la nueva.
- **C:** `malloc`/`realloc`/`free` para el arreglo de coeficientes y un **entero de
  precisión arbitraria** en base 10⁹. Es necesario porque `C(100,50) ≈ 1.0089×10²⁹`
  supera los 64 bits.

**Supuestos:**

- Los coeficientes se imprimen en orden descendente de potencias,
  `coeficiente*x^k` (omitiendo el `1` cuando el coeficiente es 1).
- La demostración paso a paso usa `n=5, x=2` porque permite mostrar cada término con
  claridad; la generación se mide con `n=100`.
- La evaluación también usa enteros grandes, así que funciona con `x` y `n` mayores.
  `n` y `x` se validan como enteros no negativos.
- Medición: 10 generaciones completas de `n=100`. Los tiempos de cada corrida se
  guardan en `resultados/tiempos_ejercicio2.txt`.

**Archivos:** `python/polinomio.py`, `c/polinomio.c`,
`scripts/compilar_ejecutar.sh`, y en `resultados/`:

| Archivo | Contenido |
| --- | --- |
| `polinomio_n5.txt` | Demostración (polinomio + paso a paso) de Python y C; ambos llegan a `f(2)=243` |
| `polinomio_n100.txt` | Polinomio `(x+1)^100` completo de Python y C (idénticos) |
| `tiempos_ejercicio2.txt` | CSV `lenguaje,tiempo_segundos,n,repeticiones` |

---

## Ejercicio 3 — Palabras reservadas de C traducidas al español

**Enunciado:** para un programa escrito en C cargado dinámicamente en memoria,
verificar si existen palabras reservadas y traducirlas a su equivalente en español.

**Requisito principal:** detección y traducción **por palabra**, con reporte en
`resultados/palabras_encontradas.txt`:

```text
palabra_original -> Traduccion, linea N, columna M
```

La traducción va con mayúscula inicial (ejemplo del profesor: `while -> Mientras`).
Las líneas y columnas son **1-based** y apuntan al inicio del identificador.

**Supuestos:**

- Se ignoran **comentarios** (`//` y `/* */`), **cadenas** (`"..."`) y **caracteres**
  (`'...'`); las palabras reservadas dentro de ellos **no** se traducen.
- Las subcadenas dentro de identificadores no cuentan (p. ej. `contador` no activa `for`).
- Se detectan las **44 palabras reservadas de C11** (C89 + C99 + C11), no solo las 16
  más comunes. La tabla completa está en `python/analizador.py` y `c/analizador.c`.
- `c/programa.c` es un programa C11 **válido** que contiene las 44 palabras (algunas
  dentro de `#if 0`), además de comentarios y cadenas para comprobar que se ignoran.
- En C el archivo se carga completo con `malloc`; en Python se lee completo en memoria.

**Resultado:** **118 ocurrencias** detectadas en `c/programa.c`, con reportes idénticos
en Python y C, y la misma traducción.

**Archivos:** `c/programa.c`, `python/analizador.py`, `c/analizador.c`,
`scripts/compilar_ejecutar.sh`, y en `resultados/`:

| Archivo | Contenido |
| --- | --- |
| `palabras_encontradas.txt` | Reporte principal con las 118 ocurrencias |
| `programa_traducido.c` | Copia del programa con las palabras reemplazadas por su traducción (extra visual) |

---

## Estructura de esta carpeta

```text
PARTICIPANTE_2_Cesar_Velasquez_V-26592592/
├── README.md
├── enlace_drive.txt
├── ejercicio1/
│   ├── python/    caballo.py, test_caballo.py
│   ├── c/         caballo.c
│   ├── scripts/   compilar_ejecutar.sh, ejemplos.txt, movimientos.txt
│   └── resultados/ salida_ejemplos.txt, tiempos_ejercicio1.txt
├── ejercicio2/
│   ├── python/    polinomio.py
│   ├── c/         polinomio.c
│   ├── scripts/   compilar_ejecutar.sh
│   └── resultados/ polinomio_n5.txt, polinomio_n100.txt, tiempos_ejercicio2.txt
└── ejercicio3/
    ├── python/    analizador.py
    ├── c/         analizador.c, programa.c
    ├── scripts/   compilar_ejecutar.sh
    └── resultados/ palabras_encontradas.txt, programa_traducido.c
```

## Reglas recordatorias

- La prueba diagnóstica es **individual**: este trabajo es del Participante 2.
- **No subir videos** al repositorio: solo el enlace de Drive en `enlace_drive.txt`.
- **IA permitida**, pero debo entender y defender cada línea de este código.
- Informes y citas en **normas APA** si el producto lo requiere.
