#!/usr/bin/env bash
# Ejecuta el ejercicio 1 (solo Python): pruebas, ejemplos, 500 movimientos y tiempos.
set -euo pipefail

cd "$(dirname "$0")/.." || exit 1
mkdir -p resultados

PYTHON="${PYTHON:-python}"

if ! command -v "$PYTHON" >/dev/null 2>&1; then
    echo "ERROR: no se encontro Python. Instale Python 3 y agreguelo al PATH." >&2
    exit 1
fi

echo "== Pruebas unitarias de Python =="
"$PYTHON" python/test_caballo.py

echo "== Casos de ejemplo =="
"$PYTHON" python/caballo.py pruebas scripts/ejemplos.txt > resultados/salida_ejemplos.txt
cat resultados/salida_ejemplos.txt

echo "== Lista real de 500 movimientos =="
"$PYTHON" python/caballo.py validar-archivo scripts/movimientos.txt > resultados/salida_movimientos_500.txt
cat resultados/salida_movimientos_500.txt

if ! grep -q "Movimientos: 500" resultados/salida_movimientos_500.txt; then
    echo "ERROR: la lista no contiene exactamente 500 movimientos" >&2
    exit 1
fi
if ! grep -q "movimiento CORRECTO" resultados/salida_movimientos_500.txt; then
    echo "ERROR: la lista de 500 movimientos no es valida" >&2
    exit 1
fi

echo "== Tiempos (10 repeticiones de 500 movimientos) =="
echo "lenguaje,tiempo_segundos,n_movimientos,repeticiones" > resultados/tiempos_ejercicio1.txt
"$PYTHON" python/caballo.py tiempo scripts/movimientos.txt 10 >> resultados/tiempos_ejercicio1.txt
cat resultados/tiempos_ejercicio1.txt

echo "OK: ejercicio 1 completado"
