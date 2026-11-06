#!/usr/bin/env bash
# Ejercicio 1 - Caballo de ajedrez (solo C): compila, prueba y mide tiempos.
# Uso (desde Git Bash): bash scripts/compilar_ejecutar.sh
set -euo pipefail

BASE="$(cd "$(dirname "$0")/.." && pwd)"
cd "$BASE"

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
GCC_DIR="$(dirname "$GCC")"
case ":$PATH:" in
  *":$GCC_DIR:"*) ;;
  *) export PATH="$GCC_DIR:$PATH" ;;
esac

echo "== Ejercicio 1: Caballo de ajedrez (solo C) =="
echo "gcc: $($GCC --version | head -n1)"

mkdir -p resultados

echo "[1/5] Compilando codigo C..."
"$GCC" -O2 -Wall -Wextra -o c/caballo.exe c/caballo.c
"$GCC" -O2 -Wall -Wextra -o c/test_caballo.exe c/test_caballo.c
echo "      OK: c/caballo.exe y c/test_caballo.exe"

echo "[2/5] Pruebas unitarias en C (11 tests)..."
./c/test_caballo.exe

echo "[3/5] Demostracion de deteccion de errores (casos cortos del PDF)..."
echo "   --- Caso: salto ilegal (falla en movimiento 3) ---"
./c/caballo.exe validar a1 "Kc2 Kd4 Kb4 Kc5" || true
echo "   --- Caso: letra no pertenece a caballo ---"
./c/caballo.exe validar a1 "Qc2" || true

echo "[4/5] Casos completos del enunciado (ejemplos.txt)..."
./c/caballo.exe pruebas scripts/ejemplos.txt

echo "[5/5] Medicion de tiempos (10 verificaciones de la lista de 500 movimientos)..."
LINEA_C=$(./c/caballo.exe tiempo scripts/movimientos.txt 10)
echo "      C: $LINEA_C"

echo "Escribiendo resultados/tiempos_ejercicio1.txt..."
printf 'lenguaje,tiempo_segundos,n_movimientos,repeticiones\n%s\n' \
  "$LINEA_C" > resultados/tiempos_ejercicio1.txt
cat resultados/tiempos_ejercicio1.txt

echo "OK: Ejercicio 1 terminado."
