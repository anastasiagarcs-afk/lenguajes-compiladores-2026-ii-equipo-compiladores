#!/bin/bash
mkdir -p resultados
gcc -O2 c/ejercicio2.c -o c/ejercicio2
./c/ejercicio2
python3 python/ejercicio2.py