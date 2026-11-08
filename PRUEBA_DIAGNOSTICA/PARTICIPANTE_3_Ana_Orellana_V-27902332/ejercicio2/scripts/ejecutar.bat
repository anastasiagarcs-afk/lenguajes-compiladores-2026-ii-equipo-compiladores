@echo off
echo Compilando Ejercicio 2 (C)...
cd ..\c
gcc ejercicio2.c -o ejercicio2.exe
if %errorlevel% neq 0 (
    echo Error de compilacion.
    pause
    exit /b %errorlevel%
)
echo Ejecutando... Pasando n=7 y x=2 como parametros
ejercicio2.exe 7 2
echo Ejecucion completada. Revisa resultado_ejercicio2.txt
pause