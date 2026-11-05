#!/usr/bin/env bash
# Ejercicio 1 - Caballo de ajedrez: compila, prueba y mide tiempos.
# Uso (desde Git Bash): bash scripts/compilar_ejecutar.sh
set -euo pipefail

BASE="$(cd "$(dirname "$0")/.." && pwd)"
cd "$BASE"

# --- Deteccion de herramientas ---
PY=""
for cand in python3 python "py -3"; do
  if $cand --version >/dev/null 2>&1; then
    PY="$cand"
    break
  fi
done
if [ -z "$PY" ]; then
  echo "ERROR: Python 3 no encontrado. Instalalo desde https://www.python.org/downloads/" >&2
  exit 1
fi

GCC=""
if command -v gcc >/dev/null 2>&1; then
  GCC="$(command -v gcc)"
elif [ -x "/c/msys64/mingw64/bin/gcc.exe" ]; then
  GCC="/c/msys64/mingw64/bin/gcc.exe"
elif [ -x "C:/msys64/mingw64/bin/gcc.exe" ]; then
  GCC="C:/msys64/mingw64/bin/gcc.exe"
else
  echo "ERROR: gcc no encontrado. Instala MSYS2 + mingw-w64-x86_64-gcc." >&2
  exit 1
fi
# gcc necesita sus utilidades (as, ld) en el PATH
GCC_DIR="$(dirname "$GCC")"
case ":$PATH:" in
  *":$GCC_DIR:"*) ;;
  *) export PATH="$GCC_DIR:$PATH" ;;
esac

echo "== Ejercicio 1: Caballo de ajedrez =="
echo "Python: $($PY --version 2>&1)"
echo "gcc:    $($GCC --version | head -n1)"

mkdir -p resultados

echo "[1/5] Compilando codigo C..."
"$GCC" -O2 -Wall -Wextra -o c/caballo.exe c/caballo.c
echo "      OK: c/caballo.exe"

echo "[2/5] Pruebas unitarias de Python (unittest)..."
$PY python/test_caballo.py

echo "[3/5] Casos del enunciado en Python y en C..."
$PY python/caballo.py pruebas scripts/ejemplos.txt
./c/caballo.exe pruebas scripts/ejemplos.txt

echo "[4/5] Medicion de tiempos (10 verificaciones de 500 movimientos)..."
LINEA_PY=$($PY python/caballo.py tiempo scripts/movimientos.txt 10)
LINEA_C=$(./c/caballo.exe tiempo scripts/movimientos.txt 10)
echo "      Python: $LINEA_PY"
echo "      C:      $LINEA_C"

echo "[5/5] Escribiendo resultados/tiempos_ejercicio1.txt..."
printf 'lenguaje,tiempo_segundos,n_movimientos,repeticiones\n%s\n%s\n' \
  "$LINEA_PY" "$LINEA_C" > resultados/tiempos_ejercicio1.txt
cat resultados/tiempos_ejercicio1.txt

echo "OK: Ejercicio 1 terminado."
