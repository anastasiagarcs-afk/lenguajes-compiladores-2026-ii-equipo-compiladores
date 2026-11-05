#!/usr/bin/env bash
# Ejercicio 3 - Palabras reservadas de C: compila, ejecuta y genera reportes.
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

echo "== Ejercicio 3: Palabras reservadas de C traducidas al espanol =="
echo "Python: $($PY --version 2>&1)"
echo "gcc:    $($GCC --version | head -n1)"

mkdir -p resultados

echo "[1/5] Compilando analizador en C..."
"$GCC" -O2 -Wall -Wextra -o c/analizador.exe c/analizador.c
echo "      OK: c/analizador.exe"

echo "[2/5] Verificando sintaxis de c/programa.c (gcc -fsyntax-only)..."
"$GCC" -fsyntax-only -Wall c/programa.c 2>/dev/null || \
  echo "      AVISO: programa.c tiene avisos de sintaxis (no bloqueante)."

echo "[3/5] Ejecutando analizador Python y C sobre c/programa.c..."
$PY python/analizador.py c/programa.c resultados/_py_reporte.txt resultados/_py_traducido.c
./c/analizador.exe c/programa.c resultados/_c_reporte.txt resultados/_c_traducido.c

echo "[4/5] Comparando reportes de Python y C..."
if diff -q resultados/_py_reporte.txt resultados/_c_reporte.txt >/dev/null 2>&1; then
  echo "      OK: ambos lenguajes producen el mismo reporte."
else
  echo "      ERROR: los reportes de Python y C difieren:" >&2
  diff resultados/_py_reporte.txt resultados/_c_reporte.txt >&2 || true
  exit 1
fi

echo "[5/5] Escribiendo resultados finales..."
{
  echo "# Reporte del analizador de palabras reservadas (Ejercicio 3)"
  echo "# Entrada: c/programa.c - generado por analizador.py y analizador.c"
  echo "# Formato: palabra_original -> Traduccion, linea N, columna M"
  cat resultados/_py_reporte.txt
} > resultados/palabras_encontradas.txt

cp resultados/_py_traducido.c resultados/programa_traducido.c
rm -f resultados/_py_reporte.txt resultados/_c_reporte.txt \
      resultados/_py_traducido.c resultados/_c_traducido.c

echo "      OK: resultados/palabras_encontradas.txt"
echo "      OK: resultados/programa_traducido.c (extra)"
echo ""
echo "--- Fragmento del reporte ---"
head -n 8 resultados/palabras_encontradas.txt
echo "----------------------------"
echo "OK: Ejercicio 3 terminado."
