#!/bin/bash
echo "Ejecutando Polinomio en Python..."
python polinomio.py

echo "Compilando y ejecutando Polinomio en C..."
gcc polinomio.c -o polinomio.exe
./polinomio.exe