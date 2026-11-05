#!/bin/bash
mkdir -p resultados
gcc -O2 c/ejercicio3.c -o c/ejercicio3
./c/ejercicio3 c/ejemplo_entrada.c
python3 python/ejercicio3.py python/ejemplo_entrada.c