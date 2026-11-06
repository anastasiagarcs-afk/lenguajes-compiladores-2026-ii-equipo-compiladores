#!/usr/bin/env bash
# Ejercicio 3 - Palabras reservadas de C: compila y genera reportes (solo C).
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

echo "== Ejercicio 3: Palabras reservadas de C traducidas al espanol (solo C) =="
echo "gcc: $($GCC --version | head -n1)"

mkdir -p resultados

echo "[1/4] Compilando analizador en C..."
"$GCC" -O2 -Wall -Wextra -o c/analizador.exe c/analizador.c
echo "      OK: c/analizador.exe"

echo "[2/4] Verificando sintaxis de c/programa.c (gcc -fsyntax-only)..."
"$GCC" -fsyntax-only -Wall c/programa.c 2>/dev/null || \
  echo "      AVISO: programa.c tiene avisos de sintaxis (no bloqueante)."

echo "[3/4] Ejecutando analizador C sobre c/programa.c..."
./c/analizador.exe c/programa.c resultados/_c_reporte.txt resultados/_c_traducido.c

echo "[4/4] Escribiendo resultados finales..."
{
  echo "# Reporte del analizador de palabras reservadas (Ejercicio 3)"
  echo "# Entrada: c/programa.c - generado por analizador.c"
  echo "# Formato: palabra_original -> Traduccion, linea N, columna M"
  cat resultados/_c_reporte.txt
} > resultados/palabras_encontradas.txt

cp resultados/_c_traducido.c resultados/programa_traducido.c
rm -f resultados/_c_reporte.txt resultados/_c_traducido.c

echo "      OK: resultados/palabras_encontradas.txt"
echo "      OK: resultados/programa_traducido.c (extra)"
echo ""
echo "--- Fragmento del reporte ---"
head -n 8 resultados/palabras_encontradas.txt
echo "----------------------------"
echo "OK: Ejercicio 3 terminado."
