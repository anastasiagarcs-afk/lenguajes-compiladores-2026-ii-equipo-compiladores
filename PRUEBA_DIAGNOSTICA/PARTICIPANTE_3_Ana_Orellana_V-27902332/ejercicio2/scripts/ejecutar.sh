#!/bin/bash
echo "Compilando Ejercicio 2 (C)..."
cd ../c
gcc ejercicio2.c -o ejercicio2 -lm
if [ $? -eq 0 ]; then
    echo "Ejecutando... Pasando n=7 y x=2 como parametros"
    ./ejercicio2 7 2
    echo "Ejecucion completada. Revisa resultado_ejercicio2.txt"
else
    echo "Error de compilacion."
fi