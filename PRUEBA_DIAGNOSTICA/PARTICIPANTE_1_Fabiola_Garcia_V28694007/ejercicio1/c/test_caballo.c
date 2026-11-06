/*
 * Ejercicio 1 - Caballo de ajedrez: pruebas unitarias en C.
 * Participante 1: Fabiola Garcia (V28694007) - UNEG 2026-II.
 *
 * Port de test_caballo.py a C. Usa las funciones internas de caballo.c
 * mediante #include "caballo.c" con renombrado del main.
 * 11 pruebas: parsear_casilla (2) + validar (9).
 * Sale con codigo 0 si todas pasan, 1 si alguna falla.
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Renombrar main de caballo.c para evitar conflicto con el nuestro */
#define main caballo_main
#include "caballo.c"
#undef main

static int total = 0, fallas = 0;

static void check(int cond, const char *nombre)
{
    total++;
    if (cond) {
        printf("  ok   %s\n", nombre);
    } else {
        fallas++;
        printf("  FAIL %s\n", nombre);
    }
}

static int contiene(const char *msg, const char *sub)
{
    return strstr(msg, sub) != NULL;
}

int main(void)
{
    char msg[512];
    int ok;

    printf("TestParsearCasilla:\n");

    /* test_valida */
    {
        int c, f;
        check(parsear_casilla("a1", &c, &f) == 1 && c == 0 && f == 0 &&
              parsear_casilla("h8", &c, &f) == 1 && c == 7 && f == 7 &&
              parsear_casilla("E4", &c, &f) == 1 && c == 4 && f == 3,
              "test_valida");
    }

    /* test_invalida */
    {
        int c, f;
        check(parsear_casilla("i1", &c, &f) == 0 &&
              parsear_casilla("a9", &c, &f) == 0 &&
              parsear_casilla("a0", &c, &f) == 0 &&
              parsear_casilla("a", &c, &f) == 0 &&
              parsear_casilla("zz", &c, &f) == 0,
              "test_invalida");
    }

    printf("TestValidar:\n");

    /* test_secuencia_correcta_ejemplo_pdf */
    ok = validar("a1", "Kc2 Kd4 Kb3 Kc5", msg, sizeof msg);
    check(ok == 1 && strcmp(msg, "movimiento CORRECTO") == 0,
          "test_secuencia_correcta_ejemplo_pdf");

    /* test_salto_sencillo */
    ok = validar("a1", "Kb3", msg, sizeof msg);
    check(ok == 1 && strcmp(msg, "movimiento CORRECTO") == 0,
          "test_salto_sencillo");

    /* test_secuencia_desde_h8 */
    ok = validar("h8", "Kf7 Kd6", msg, sizeof msg);
    check(ok == 1 && strcmp(msg, "movimiento CORRECTO") == 0,
          "test_secuencia_desde_h8");

    /* test_letra_distinta_de_k */
    ok = validar("a1", "Be2", msg, sizeof msg);
    check(ok == 0 && contiene(msg, "no pertenece a un caballo") &&
          contiene(msg, "movimiento 1"),
          "test_letra_distinta_de_k");

    /* test_letra_distinta_de_k_posterior */
    ok = validar("a1", "Kc2 Rd4", msg, sizeof msg);
    check(ok == 0 && contiene(msg, "no pertenece a un caballo") &&
          contiene(msg, "movimiento 2"),
          "test_letra_distinta_de_k_posterior");

    /* test_casilla_invalida */
    ok = validar("a1", "Kh9", msg, sizeof msg);
    check(ok == 0 && contiene(msg, "casilla invalida"),
          "test_casilla_invalida");

    /* test_primer_fallo_falla_en_movimiento_1 */
    ok = validar("a1", "Kc3", msg, sizeof msg);
    check(ok == 0 && strcmp(msg, "INCORRECTO falla en el movimiento 1") == 0,
          "test_primer_fallo_falla_en_movimiento_1");

    /* test_primer_fallo_falla_en_movimiento_2 */
    ok = validar("a1", "Kc2 Kc4", msg, sizeof msg);
    check(ok == 0 && strcmp(msg, "INCORRECTO falla en el movimiento 2") == 0,
          "test_primer_fallo_falla_en_movimiento_2");

    /* test_posicion_inicial_invalida */
    ok = validar("z9", "Kc2", msg, sizeof msg);
    check(ok == 0 && contiene(msg, "posicion inicial invalida"),
          "test_posicion_inicial_invalida");

    printf("\n%d pruebas ejecutadas, %d fallas\n", total, fallas);
    return fallas > 0 ? 1 : 0;
}
