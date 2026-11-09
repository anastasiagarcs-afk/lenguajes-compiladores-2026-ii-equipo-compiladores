#!/bin/bash
echo "Compilando el traductor de palabras reservadas..."
gcc reservadas.c -o reservadas.exe
echo "Ejecutando..."
./reservadas.exe