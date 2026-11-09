#!/bin/bash
set -e
cd "$(dirname "$0")"

echo "Compilando caballo.cpp..."
g++ -O2 -Wall -Wextra caballo.cpp -o caballo_cpp

echo
echo "== Pruebas del enunciado =="
CASES=(
  "a1 4 Kc2 Kd4 Kb3 Kc5"
  "a1 4 Kc2 Kd4 Kb4 Kc5"
  "a2 5 Kc3 Ke4 Kf6 Ke8 Kg7"
  "a2 5 Kc8 Ke4 Kf6 Ke8 Kg7"
  "a8 2 Kb6 Kd7"
  "a8 2 Kb6 Kb7"
  "a1 10 Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"
  "a1 10 Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"
  "a1 10 Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"
)
for c in "${CASES[@]}"; do
  echo "--- ./caballo_cpp $c"
  ./caballo_cpp $c
done

echo
echo "== Benchmark =="
if [ -f movimientos.txt ]; then
  ./caballo_cpp --bench a2 movimientos.txt
else
  echo "Falta movimientos.txt"
fi