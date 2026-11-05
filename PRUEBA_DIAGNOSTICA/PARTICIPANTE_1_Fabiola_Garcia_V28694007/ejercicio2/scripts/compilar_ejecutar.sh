#!/usr/bin/env bash
# Ejercicio 2 - Polinomio (x+1)^n: compila, ejecuta y mide tiempos.
# Uso (desde Git Bash): bash scripts/compilar_ejecutar.sh
set -euo pipefail

BASE="$(cd "$(dirname "$0")/.." && pwd)"
cd "$BASE"

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

echo "== Ejercicio 2: Polinomio (x+1)^n con memoria dinamica =="
echo "Python: $($PY --version 2>&1)"
echo "gcc:    $($GCC --version | head -n1)"

mkdir -p resultados

echo "[1/4] Compilando codigo C..."
"$GCC" -O2 -Wall -Wextra -o c/polinomio.exe c/polinomio.c
echo "      OK: c/polinomio.exe"

echo "[2/4] Demostracion n=5, x=2 (polinomio + calculo paso a paso)..."
{
  echo "=== Python ==="
  $PY python/polinomio.py demo
  echo ""
  echo "=== C ==="
  ./c/polinomio.exe demo
} > resultados/polinomio_n5.txt
cat resultados/polinomio_n5.txt

echo "[3/4] Polinomio (x+1)^100 (precision arbitraria)..."
{
  echo "=== Python ==="
  $PY python/polinomio.py poly 100
  echo ""
  echo "=== C ==="
  ./c/polinomio.exe poly 100
} > resultados/polinomio_n100.txt
echo "      OK: resultados/polinomio_n100.txt"

echo "[4/4] Medicion de tiempos de generacion con n=100..."
LINEA_PY=$($PY python/polinomio.py tiempo 100)
LINEA_C=$(./c/polinomio.exe tiempo 100)
echo "      Python: $LINEA_PY"
echo "      C:      $LINEA_C"
printf 'lenguaje,tiempo_segundos,n\n%s\n%s\n' "$LINEA_PY" "$LINEA_C" \
  > resultados/tiempos_ejercicio2.txt
cat resultados/tiempos_ejercicio2.txt

echo "OK: Ejercicio 2 terminado."
