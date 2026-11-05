@echo off
if not exist resultados mkdir resultados
echo === Compilando y ejecutando C (Ejercicio 2) ===
gcc -O2 c\ejercicio2.c -o c\ejercicio2.exe
c\ejercicio2.exe
echo.
echo === Ejecutando Python (Ejercicio 2) ===
python python\ejercicio2.py