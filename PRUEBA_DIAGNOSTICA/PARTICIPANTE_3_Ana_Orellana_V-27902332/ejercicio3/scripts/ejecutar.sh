#!/bin/bash
echo "Compilando Ejercicio 3 (C)..."
cd ../c
gcc ejercicio3.c -o ejercicio3
if [ $? -eq 0 ]; then
    echo "Ejecutando analizador léxico..."
    ./ejercicio3
    echo "Ejecucion completada. Revisa resultado_ejercicio3.txt"
else
    echo "Error de compilacion."
fi