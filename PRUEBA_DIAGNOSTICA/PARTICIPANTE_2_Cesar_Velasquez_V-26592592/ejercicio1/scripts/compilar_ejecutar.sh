#!/usr/bin/env bash
# Compila y ejecuta el ejercicio 1 en Python y C, compara resultados y mide tiempos.
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

echo "== Compilando C =="
"$CC" -O2 -std=c11 -Wall -Wextra -pedantic -o c/caballo.exe c/caballo.c

echo "== Pruebas unitarias de Python =="
"$PYTHON" python/test_caballo.py

echo "== Casos de ejemplo =="
"$PYTHON" python/caballo.py pruebas scripts/ejemplos.txt > resultados/salida_ejemplos_python.txt
./c/caballo.exe pruebas scripts/ejemplos.txt > resultados/salida_ejemplos_c.txt

if diff -u resultados/salida_ejemplos_python.txt resultados/salida_ejemplos_c.txt; then
    echo "OK: Python y C producen la misma salida en los ejemplos"
else
    echo "ERROR: las salidas de Python y C no coinciden" >&2
    exit 1
fi

{
    echo "== Python =="
    cat resultados/salida_ejemplos_python.txt
    echo
    echo "== C =="
    cat resultados/salida_ejemplos_c.txt
} > resultados/salida_ejemplos.txt
rm -f resultados/salida_ejemplos_python.txt resultados/salida_ejemplos_c.txt

echo "== Tiempos (10 repeticiones) =="
echo "lenguaje,tiempo_segundos,n_movimientos,repeticiones" > resultados/tiempos_ejercicio1.txt
"$PYTHON" python/caballo.py tiempo scripts/movimientos.txt 10 >> resultados/tiempos_ejercicio1.txt
./c/caballo.exe tiempo scripts/movimientos.txt 10 >> resultados/tiempos_ejercicio1.txt
cat resultados/tiempos_ejercicio1.txt

echo "OK: ejercicio 1 completado"
