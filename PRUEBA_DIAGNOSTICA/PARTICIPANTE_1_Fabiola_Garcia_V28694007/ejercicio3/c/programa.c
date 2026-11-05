/*
 * programa.c - Programa de ejemplo para el Ejercicio 3
 * (Prueba Diagnostica, Lenguajes y Compiladores UNEG 2026-II)
 *
 * Este archivo es la ENTRADA del analizador de palabras reservadas.
 * Contiene las 16 palabras reservadas de la tabla de traduccion,
 * ademas de comentarios y cadenas de texto que el analizador DEBE
 * ignorar (no se traducen palabras dentro de ellos).
 *
 * Comentario en bloque con palabras clave: int while for if else
 * que NO deben traducirse dentro de este comentario.
 */

#include <stdio.h>

struct Punto {
    int x;
    int y;
};

// Comentario de linea: while y return aqui tampoco se traducen

double promedio(const char *texto) {
    int i = 0;
    double suma = 0.0;
    while (texto[i] != '\0') {
        suma = suma + (double) texto[i];
        i = i + 1;
    }
    return suma;
}

void mostrar(char *mensaje, float valor) {
    printf("%s = %f\n", mensaje, valor);
}

int main(void) {
    struct Punto p;
    char mensaje[] = "cadena con if while for int que no se traduce";
    float valor = 3.14f;
    double resultado = promedio(mensaje);
    int contador = 0;

    p.x = 1;
    p.y = 2;

    if (resultado > 0.0) {
        for (contador = 0; contador < 5; contador = contador + 1) {
            valor = valor + 1.0f;
        }
    } else {
        valor = 0.0f;
    }

    switch (contador) {
        case 1:
            mostrar(mensaje, valor);
            break;
        case 2:
            break;
        default:
            break;
    }

    while (contador > 0) {
        if (contador == 3) {
            contador = contador - 1;
            continue;
        }
        contador = contador - 1;
    }

    mostrar(mensaje, valor);
    return 0;
}
