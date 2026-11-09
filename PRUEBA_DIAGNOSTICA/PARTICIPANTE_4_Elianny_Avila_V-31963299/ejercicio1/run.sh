#!/bin/bash
echo "Compilando el caballo..."
g++ caballo.cpp -o programa.exe

echo ""
echo "Ejecutando prueba de ejemplo:"
./programa.exe a1 4 Kc2 Kd4 Kb3 Kc5

echo ""
echo "Ejecutando benchmark de los 500 movimientos:"
./programa.exe --bench a2 movimientos.txt
