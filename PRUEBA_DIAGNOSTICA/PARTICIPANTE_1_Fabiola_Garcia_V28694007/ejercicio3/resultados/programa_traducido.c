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

estructura Punto {
    entero x;
    entero y;
};

// Comentario de linea: while y return aqui tampoco se traducen

doble promedio(const caracter *texto) {
    entero i = 0;
    doble suma = 0.0;
    mientras (texto[i] != '\0') {
        suma = suma + (doble) texto[i];
        i = i + 1;
    }
    retornar suma;
}

vacio mostrar(caracter *mensaje, flotante valor) {
    printf("%s = %f\n", mensaje, valor);
}

entero main(vacio) {
    estructura Punto p;
    caracter mensaje[] = "cadena con if while for int que no se traduce";
    flotante valor = 3.14f;
    doble resultado = promedio(mensaje);
    entero contador = 0;

    p.x = 1;
    p.y = 2;

    si (resultado > 0.0) {
        para (contador = 0; contador < 5; contador = contador + 1) {
            valor = valor + 1.0f;
        }
    } sino {
        valor = 0.0f;
    }

    segun (contador) {
        caso 1:
            mostrar(mensaje, valor);
            romper;
        caso 2:
            romper;
        defecto:
            romper;
    }

    mientras (contador > 0) {
        si (contador == 3) {
            contador = contador - 1;
            continuar;
        }
        contador = contador - 1;
    }

    mostrar(mensaje, valor);
    retornar 0;
}
