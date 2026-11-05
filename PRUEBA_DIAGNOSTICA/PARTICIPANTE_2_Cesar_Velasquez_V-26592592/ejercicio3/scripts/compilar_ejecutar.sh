#!/usr/bin/env bash
# Compila y ejecuta el ejercicio 3 en Python y C, y compara los reportes.
set -euo pipefail

cd "$(dirname "$0")/.." || exit 1
mkdir -p resultados

PYTHON="${PYTHON:-python}"
CC="${CC:-gcc}"

if ! command -v "$PYTHON" >/dev/null 2>&1; then
    echo "ERROR: no se encontro Python. Instale Python 3 y agreguelo al PATH." >&2
    exit 1
fi
if ! command -v "$CC" >/dev/null 2>&1; then
    echo "ERROR: no se encontro gcc. En Windows instale MSYS2 y el paquete mingw-w64-ucrt-x86_64-gcc." >&2
    exit 1
fi

echo "== Verificando que programa.c sea C valido =="
"$CC" -std=c11 -fsyntax-only -Wall -Wextra -pedantic c/programa.c

echo "== Compilando analizador en C =="
"$CC" -O2 -std=c11 -Wall -Wextra -pedantic -o c/analizador.exe c/analizador.c

echo "== Ejecutando Python =="
"$PYTHON" python/analizador.py c/programa.c \
    resultados/reporte_python.txt \
    resultados/programa_traducido_python.c

echo "== Ejecutando C =="
./c/analizador.exe c/programa.c \
    resultados/reporte_c.txt \
    resultados/programa_traducido_c.c

if diff -u resultados/reporte_python.txt resultados/reporte_c.txt; then
    echo "OK: ambos lenguajes producen el mismo reporte"
else
    echo "ERROR: los reportes de Python y C no coinciden" >&2
    exit 1
fi

if diff -u resultados/programa_traducido_python.c resultados/programa_traducido_c.c; then
    echo "OK: ambos lenguajes producen la misma traduccion"
else
    echo "ERROR: las traducciones de Python y C no coinciden" >&2
    exit 1
fi

cp resultados/reporte_python.txt resultados/palabras_encontradas.txt
cp resultados/programa_traducido_python.c resultados/programa_traducido.c
rm -f resultados/reporte_python.txt resultados/reporte_c.txt \
      resultados/programa_traducido_python.c resultados/programa_traducido_c.c

echo "OK: ejercicio 3 completado"
