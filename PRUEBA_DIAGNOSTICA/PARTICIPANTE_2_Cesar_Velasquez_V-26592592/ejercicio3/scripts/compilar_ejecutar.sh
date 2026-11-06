#!/usr/bin/env bash
# Ejecuta el ejercicio 3 (solo C) y verifica el reporte y la traduccion.
set -euo pipefail

cd "$(dirname "$0")/.." || exit 1
mkdir -p resultados

CC="${CC:-gcc}"

if ! command -v "$CC" >/dev/null 2>&1; then
    echo "ERROR: no se encontro gcc. En Windows instale MSYS2 y el paquete mingw-w64-ucrt-x86_64-gcc." >&2
    exit 1
fi

echo "== Verificando que programa.c sea C valido =="
"$CC" -std=c11 -fsyntax-only -Wall -Wextra -pedantic c/programa.c

echo "== Compilando analizador en C =="
"$CC" -O2 -std=c11 -Wall -Wextra -pedantic -o c/analizador.exe c/analizador.c

echo "== Ejecutando analizador en C =="
./c/analizador.exe c/programa.c \
    resultados/palabras_encontradas.txt \
    resultados/programa_traducido.c

echo "== Verificando resultados =="
test -s resultados/palabras_encontradas.txt
test -s resultados/programa_traducido.c

total=$(wc -l < resultados/palabras_encontradas.txt)
if [ "$total" -ne 118 ]; then
    echo "ERROR: se esperaban 118 ocurrencias y se obtuvieron $total" >&2
    exit 1
fi
if ! grep -q "while -> Mientras" resultados/palabras_encontradas.txt; then
    echo "ERROR: no se encontro la traduccion de while" >&2
    exit 1
fi
if ! grep -q "typedef -> Tipo definido" resultados/palabras_encontradas.txt; then
    echo "ERROR: no se encontro la traduccion de typedef" >&2
    exit 1
fi

echo "OK: ejercicio 3 completado (118 ocurrencias)"
