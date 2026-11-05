@echo off
if not exist resultados mkdir resultados
echo === Compilando y ejecutando C (Ejercicio 3) ===
gcc -O2 c\ejercicio3.c -o c\ejercicio3.exe
c\ejercicio3.exe c\ejemplo_entrada.c
echo.
echo === Ejecutando Python (Ejercicio 3) ===
python python\ejercicio3.py python\ejemplo_entrada.c