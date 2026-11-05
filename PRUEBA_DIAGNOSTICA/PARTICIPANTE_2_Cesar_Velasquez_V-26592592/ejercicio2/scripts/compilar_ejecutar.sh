#!/usr/bin/env bash
# Compila y ejecuta el ejercicio 2 en Python y C, compara resultados y mide tiempos.
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
"$CC" -O2 -std=c11 -Wall -Wextra -pedantic -o c/polinomio.exe c/polinomio.c

echo "== Demostracion n=5, x=2 =="
"$PYTHON" python/polinomio.py demo > resultados/polinomio_n5_python.txt
./c/polinomio.exe demo > resultados/polinomio_n5_c.txt

if diff -u resultados/polinomio_n5_python.txt resultados/polinomio_n5_c.txt; then
    echo "OK: Python y C coinciden en la demostracion n=5"
else
    echo "ERROR: las salidas de Python y C no coinciden (n=5)" >&2
    exit 1
fi

{
    echo "== Python =="
    cat resultados/polinomio_n5_python.txt
    echo
    echo "== C =="
    cat resultados/polinomio_n5_c.txt
} > resultados/polinomio_n5.txt
rm -f resultados/polinomio_n5_python.txt resultados/polinomio_n5_c.txt

echo "== Polinomio n=100 =="
"$PYTHON" python/polinomio.py poly 100 > resultados/polinomio_n100_python.txt
./c/polinomio.exe poly 100 > resultados/polinomio_n100_c.txt

if diff -u resultados/polinomio_n100_python.txt resultados/polinomio_n100_c.txt; then
    echo "OK: Python y C coinciden en el polinomio n=100"
else
    echo "ERROR: las salidas de Python y C no coinciden (n=100)" >&2
    exit 1
fi

{
    echo "== Python =="
    cat resultados/polinomio_n100_python.txt
    echo
    echo "== C =="
    cat resultados/polinomio_n100_c.txt
} > resultados/polinomio_n100.txt
rm -f resultados/polinomio_n100_python.txt resultados/polinomio_n100_c.txt

echo "== Tiempos de generar n=100 (10 repeticiones) =="
echo "lenguaje,tiempo_segundos,n,repeticiones" > resultados/tiempos_ejercicio2.txt
"$PYTHON" python/polinomio.py tiempo 100 10 >> resultados/tiempos_ejercicio2.txt
./c/polinomio.exe tiempo 100 10 >> resultados/tiempos_ejercicio2.txt
cat resultados/tiempos_ejercicio2.txt

echo "OK: ejercicio 2 completado"
