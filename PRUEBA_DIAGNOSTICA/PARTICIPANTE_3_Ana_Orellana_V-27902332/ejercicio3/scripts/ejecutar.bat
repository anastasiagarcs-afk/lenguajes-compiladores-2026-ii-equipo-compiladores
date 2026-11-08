@echo off
echo Compilando Ejercicio 3 (C)...
cd ..\c
gcc ejercicio3.c -o ejercicio3.exe
if %errorlevel% neq 0 (
    echo Error de compilacion.
    pause
    exit /b %errorlevel%
)
echo Ejecutando analizador léxico...
ejercicio3.exe
echo Ejecucion completada. Revisa resultado_ejercicio3.txt
pause