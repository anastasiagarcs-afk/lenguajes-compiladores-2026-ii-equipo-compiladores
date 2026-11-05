/*
 * Programa de ejemplo para el ejercicio 3.
 * Contiene las 44 palabras reservadas de C11 (algunas dentro de #if 0),
 * comentarios y cadenas que NO deben traducirse.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <complex.h>

#define N 3

tipo definido estructura Punto {
    entero x;
    entero y;
} Punto;

enumeración Color { ROJO, VERDE, AZUL };

estático hilo local entero contador_hilo = 0;
estático atómico entero atomico = 0;

alinear como(16) estático carácter buffer[16];

en línea estático entero sumar(entero a, entero b)
{
    retornar a + b;
}

sin retorno estático vacío terminar(vacío)
{
    abort();
}

entero main(vacío)
{
    registro entero i;
    constante entero limite = N;
    sin signo entero u = 0u;
    largo entero largo = 0L;
    corto entero corto = 0;
    con signo entero con_signo = -1;
    doble doble = 1.0;
    flotante flotante = 1.0f;
    carácter caracter = 'A';
    vacío *generico = NULL;
    Punto p = { .x = 1, .y = 2 };
    enumeración Color color = VERDE;
    estructura Punto *pp = &p;
    unión {
        entero entero;
        flotante real;
    } dato;
    tipo definido entero Entero;
    Entero e = 0;
    booleano booleano = 1;
    doble complejo complejo = 1.0 + 2.0 * I;

    /* Este comentario contiene if while for int y no debe traducirse. */
    constante carácter *mensaje = "cadena con if while for int que no se traduce";

    afirmación estática(tamaño de(entero) >= 2, "int demasiado pequeno");

    si (booleano) {
        mientras (u < 2u) {
            u++;
        }
    } sino {
        u = 0u;
    }

    para (i = 0; i < limite; i++) {
        según (i) {
            caso 0:
                continuar;
            caso 1:
                romper;
            defecto:
                ir a salida;
        }
    }

salida:
    hacer {
        atomico++;
    } mientras (atomico < 1);

    entero alineacion = (entero)alineación de(doble);
    entero elegido = genérico(e, entero: 1, defecto: 0);

    (vacío)caracter;
    (vacío)color;
    (vacío)pp;
    (vacío)dato;
    (vacío)largo;
    (vacío)corto;
    (vacío)con_signo;
    (vacío)doble;
    (vacío)flotante;
    (vacío)generico;
    (vacío)complejo;
    (vacío)alineacion;
    (vacío)elegido;
    (vacío)contador_hilo;
    (vacío)buffer;
    (vacío)sumar;
    (vacío)terminar;
    (vacío)mensaje;

    retornar 0;
}

#si 0
externo entero externo;
volátil entero volatil;
automático entero automatico;
estático entero estatico;
constante entero constante;

vacío palabras_adicionales(vacío)
{
    entero arreglo[2] = {0, 0};
    entero *restringido ptr = arreglo;
    imaginario flotante imaginario;

    tamaño de(arreglo);
    (vacío)ptr;
    (vacío)imaginario;
}
#endif
