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

typedef struct Punto {
    int x;
    int y;
} Punto;

enum Color { ROJO, VERDE, AZUL };

static _Thread_local int contador_hilo = 0;
static _Atomic int atomico = 0;

_Alignas(16) static char buffer[16];

inline static int sumar(int a, int b)
{
    return a + b;
}

_Noreturn static void terminar(void)
{
    abort();
}

int main(void)
{
    register int i;
    const int limite = N;
    unsigned int u = 0u;
    long int largo = 0L;
    short int corto = 0;
    signed int con_signo = -1;
    double doble = 1.0;
    float flotante = 1.0f;
    char caracter = 'A';
    void *generico = NULL;
    Punto p = { .x = 1, .y = 2 };
    enum Color color = VERDE;
    struct Punto *pp = &p;
    union {
        int entero;
        float real;
    } dato;
    typedef int Entero;
    Entero e = 0;
    _Bool booleano = 1;
    double _Complex complejo = 1.0 + 2.0 * I;

    /* Este comentario contiene if while for int y no debe traducirse. */
    const char *mensaje = "cadena con if while for int que no se traduce";

    _Static_assert(sizeof(int) >= 2, "int demasiado pequeno");

    if (booleano) {
        while (u < 2u) {
            u++;
        }
    } else {
        u = 0u;
    }

    for (i = 0; i < limite; i++) {
        switch (i) {
            case 0:
                continue;
            case 1:
                break;
            default:
                goto salida;
        }
    }

salida:
    do {
        atomico++;
    } while (atomico < 1);

    int alineacion = (int)_Alignof(double);
    int elegido = _Generic(e, int: 1, default: 0);

    (void)caracter;
    (void)color;
    (void)pp;
    (void)dato;
    (void)largo;
    (void)corto;
    (void)con_signo;
    (void)doble;
    (void)flotante;
    (void)generico;
    (void)complejo;
    (void)alineacion;
    (void)elegido;
    (void)contador_hilo;
    (void)buffer;
    (void)sumar;
    (void)terminar;
    (void)mensaje;

    return 0;
}

#if 0
extern int externo;
volatile int volatil;
auto int automatico;
static int estatico;
const int constante;

void palabras_adicionales(void)
{
    int arreglo[2] = {0, 0};
    int *restrict ptr = arreglo;
    _Imaginary float imaginario;

    sizeof(arreglo);
    (void)ptr;
    (void)imaginario;
}
#endif
