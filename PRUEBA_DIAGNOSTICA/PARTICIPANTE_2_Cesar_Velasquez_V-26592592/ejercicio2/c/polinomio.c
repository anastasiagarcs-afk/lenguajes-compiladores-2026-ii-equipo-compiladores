/*
 * Coeficientes de (x+1)^n con el triangulo de Pascal.
 * Usa memoria dinamica (malloc/realloc/free) y enteros de precision
 * arbitraria en base 10^9 para soportar n=100.
 *
 * Uso:
 *   polinomio demo
 *   polinomio poly <n>
 *   polinomio eval <n> <x>
 *   polinomio tiempo <n> [repeticiones]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define BASE 1000000000u

typedef struct {
    uint32_t *digitos;
    size_t cantidad;
} BigInt;

static void bi_inicializar(BigInt *a)
{
    a->digitos = NULL;
    a->cantidad = 0;
}

static void bi_liberar(BigInt *a)
{
    free(a->digitos);
    a->digitos = NULL;
    a->cantidad = 0;
}

static void bi_normalizar(BigInt *a)
{
    while (a->cantidad > 0 && a->digitos[a->cantidad - 1] == 0) {
        a->cantidad--;
    }
}

static int bi_asegurar(BigInt *a, size_t cantidad)
{
    uint32_t *nuevos;
    size_t i;

    if (a->cantidad >= cantidad) {
        return 1;
    }
    nuevos = (uint32_t *)realloc(a->digitos, cantidad * sizeof(uint32_t));
    if (nuevos == NULL) {
        return 0;
    }
    for (i = a->cantidad; i < cantidad; i++) {
        nuevos[i] = 0;
    }
    a->digitos = nuevos;
    a->cantidad = cantidad;
    return 1;
}

static int bi_poner_u64(BigInt *a, uint64_t valor)
{
    uint64_t copia = valor;
    size_t cantidad = 1;
    size_t i;

    while (copia >= BASE) {
        cantidad++;
        copia /= BASE;
    }
    if (!bi_asegurar(a, cantidad)) {
        return 0;
    }
    copia = valor;
    for (i = 0; i < cantidad; i++) {
        a->digitos[i] = (uint32_t)(copia % BASE);
        copia /= BASE;
    }
    a->cantidad = cantidad;
    bi_normalizar(a);
    return 1;
}

static int bi_copiar(BigInt *destino, const BigInt *origen)
{
    size_t i;

    if (!bi_asegurar(destino, origen->cantidad)) {
        return 0;
    }
    for (i = 0; i < origen->cantidad; i++) {
        destino->digitos[i] = origen->digitos[i];
    }
    destino->cantidad = origen->cantidad;
    return 1;
}

static int bi_es_cero(const BigInt *a)
{
    return a->cantidad == 0;
}

static int bi_es_uno(const BigInt *a)
{
    return a->cantidad == 1 && a->digitos[0] == 1;
}

static int bi_sumar(BigInt *a, const BigInt *b)
{
    size_t maximo;
    size_t i;
    uint64_t acarreo = 0;

    if (b->cantidad == 0) {
        return 1;
    }
    maximo = a->cantidad > b->cantidad ? a->cantidad : b->cantidad;
    if (!bi_asegurar(a, maximo + 1)) {
        return 0;
    }
    for (i = 0; i < maximo; i++) {
        uint64_t suma = (uint64_t)(i < a->cantidad ? a->digitos[i] : 0) +
                        (uint64_t)(i < b->cantidad ? b->digitos[i] : 0) +
                        acarreo;
        a->digitos[i] = (uint32_t)(suma % BASE);
        acarreo = suma / BASE;
    }
    if (acarreo != 0) {
        a->digitos[maximo] = (uint32_t)acarreo;
        a->cantidad = maximo + 1;
    } else {
        a->cantidad = maximo;
    }
    bi_normalizar(a);
    return 1;
}

static int bi_multiplicar_big(BigInt *a, const BigInt *b)
{
    uint64_t *temporal;
    size_t total;
    size_t i;
    size_t j;
    size_t largo;

    if (a->cantidad == 0 || b->cantidad == 0) {
        a->cantidad = 0;
        return 1;
    }
    total = a->cantidad + b->cantidad;
    temporal = (uint64_t *)calloc(total + 1, sizeof(uint64_t));
    if (temporal == NULL) {
        return 0;
    }
    for (i = 0; i < a->cantidad; i++) {
        uint64_t acarreo = 0;
        for (j = 0; j < b->cantidad; j++) {
            uint64_t producto = (uint64_t)a->digitos[i] * (uint64_t)b->digitos[j];
            uint64_t suma = temporal[i + j] + producto + acarreo;
            temporal[i + j] = suma % BASE;
            acarreo = suma / BASE;
        }
        j = i + b->cantidad;
        while (acarreo != 0) {
            uint64_t suma = temporal[j] + acarreo;
            temporal[j] = suma % BASE;
            acarreo = suma / BASE;
            j++;
        }
    }
    largo = total + 1;
    while (largo > 0 && temporal[largo - 1] == 0) {
        largo--;
    }
    if (!bi_asegurar(a, largo)) {
        free(temporal);
        return 0;
    }
    for (i = 0; i < largo; i++) {
        a->digitos[i] = (uint32_t)temporal[i];
    }
    a->cantidad = largo;
    free(temporal);
    return 1;
}

static void bi_imprimir(FILE *salida, const BigInt *a)
{
    size_t i;

    if (a->cantidad == 0) {
        fputc('0', salida);
        return;
    }
    fprintf(salida, "%u", a->digitos[a->cantidad - 1]);
    for (i = a->cantidad - 1; i > 0; i--) {
        fprintf(salida, "%09u", a->digitos[i - 1]);
    }
}

static BigInt *generar_pascal(int n)
{
    BigInt *fila = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    int i;
    int j;

    if (fila == NULL) {
        return NULL;
    }
    for (i = 0; i <= n; i++) {
        bi_inicializar(&fila[i]);
    }
    if (!bi_poner_u64(&fila[0], 1)) {
        free(fila);
        return NULL;
    }
    for (i = 1; i <= n; i++) {
        for (j = i; j >= 1; j--) {
            if (!bi_sumar(&fila[j], &fila[j - 1])) {
                int k;
                for (k = 0; k <= n; k++) {
                    bi_liberar(&fila[k]);
                }
                free(fila);
                return NULL;
            }
        }
    }
    return fila;
}

static void liberar_fila(BigInt *fila, int n)
{
    int i;
    for (i = 0; i <= n; i++) {
        bi_liberar(&fila[i]);
    }
    free(fila);
}

static void imprimir_coeficientes(FILE *salida, BigInt *coeficientes, int n)
{
    int i;
    for (i = 0; i <= n; i++) {
        if (i > 0) {
            fputs(", ", salida);
        }
        bi_imprimir(salida, &coeficientes[i]);
    }
    fputc('\n', salida);
}

static void imprimir_polinomio(FILE *salida, BigInt *coeficientes, int n)
{
    int i;
    int primero = 1;

    for (i = 0; i <= n; i++) {
        int potencia = n - i;

        if (bi_es_cero(&coeficientes[i])) {
            continue;
        }
        if (!primero) {
            fputs(" + ", salida);
        }
        primero = 0;
        if (potencia == 0) {
            bi_imprimir(salida, &coeficientes[i]);
        } else {
            if (!bi_es_uno(&coeficientes[i])) {
                bi_imprimir(salida, &coeficientes[i]);
            }
            fputc('x', salida);
            if (potencia > 1) {
                fprintf(salida, "^%d", potencia);
            }
        }
    }
    if (primero) {
        fputc('0', salida);
    }
    fputc('\n', salida);
}

static void evaluar_pasos(FILE *salida, BigInt *coeficientes, int n, uint64_t x)
{
    BigInt *potencias = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    BigInt acumulado;
    BigInt factor;
    int i;

    if (potencias == NULL) {
        return;
    }
    for (i = 0; i <= n; i++) {
        bi_inicializar(&potencias[i]);
    }
    bi_poner_u64(&potencias[0], 1);
    bi_inicializar(&factor);
    bi_poner_u64(&factor, x);
    for (i = 1; i <= n; i++) {
        bi_copiar(&potencias[i], &potencias[i - 1]);
        bi_multiplicar_big(&potencias[i], &factor);
    }
    bi_liberar(&factor);

    bi_inicializar(&acumulado);
    fprintf(salida, "Evaluacion paso a paso para x = %" PRIu64 ":\n", x);
    for (i = 0; i <= n; i++) {
        int potencia = n - i;
        BigInt termino;

        bi_inicializar(&termino);
        bi_copiar(&termino, &coeficientes[i]);
        bi_multiplicar_big(&termino, &potencias[potencia]);
        bi_sumar(&acumulado, &termino);

        fprintf(salida, "Paso %d: ", i + 1);
        bi_imprimir(salida, &coeficientes[i]);
        if (potencia == 0) {
            fputs(" * 1 = ", salida);
        } else {
            fprintf(salida, " * %" PRIu64 "^%d = ", x, potencia);
        }
        bi_imprimir(salida, &termino);
        fputs("; acumulado = ", salida);
        bi_imprimir(salida, &acumulado);
        fputc('\n', salida);
        bi_liberar(&termino);
    }
    fputs("Suma acumulada: ", salida);
    bi_imprimir(salida, &acumulado);
    fputc('\n', salida);
    fprintf(salida, "f(%" PRIu64 ") = ", x);
    bi_imprimir(salida, &acumulado);
    fputc('\n', salida);

    bi_liberar(&acumulado);
    for (i = 0; i <= n; i++) {
        bi_liberar(&potencias[i]);
    }
    free(potencias);
}

static void mostrar(FILE *salida, int n, uint64_t x)
{
    BigInt *coeficientes = generar_pascal(n);

    if (coeficientes == NULL) {
        fprintf(stderr, "ERROR: sin memoria para n=%d\n", n);
        return;
    }
    fprintf(salida, "Coeficientes de (x+1)^%d:\n", n);
    imprimir_coeficientes(salida, coeficientes, n);
    fprintf(salida, "Polinomio (x+1)^%d:\n", n);
    imprimir_polinomio(salida, coeficientes, n);
    fputc('\n', salida);
    evaluar_pasos(salida, coeficientes, n, x);
    liberar_fila(coeficientes, n);
}

static double ahora(void)
{
#ifdef _WIN32
    static LARGE_INTEGER frecuencia;
    static int inicializado = 0;
    LARGE_INTEGER contador;

    if (!inicializado) {
        QueryPerformanceFrequency(&frecuencia);
        inicializado = 1;
    }
    QueryPerformanceCounter(&contador);
    return (double)contador.QuadPart / (double)frecuencia.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#endif
}

static int modo_tiempo(int n, int repeticiones)
{
    double inicio;
    double total;
    int i;

    inicio = ahora();
    for (i = 0; i < repeticiones; i++) {
        BigInt *coeficientes = generar_pascal(n);
        if (coeficientes == NULL) {
            fprintf(stderr, "ERROR: sin memoria para n=%d\n", n);
            return 1;
        }
        liberar_fila(coeficientes, n);
    }
    total = ahora() - inicio;
    printf("c,%.6f,%d,%d\n", total, n, repeticiones);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr,
                "Uso: polinomio {demo|poly|eval|tiempo} ...\n"
                "  demo\n"
                "  poly <n>\n"
                "  eval <n> <x>\n"
                "  tiempo <n> [repeticiones]\n");
        return 2;
    }

    if (strcmp(argv[1], "demo") == 0) {
        printf("== Demostracion n=5, x=2 ==\n");
        mostrar(stdout, 5, 2);
        return 0;
    }
    if (strcmp(argv[1], "poly") == 0) {
        int n;
        BigInt *coeficientes;

        if (argc < 3) {
            fprintf(stderr, "Uso: polinomio poly <n>\n");
            return 2;
        }
        n = atoi(argv[2]);
        if (n < 0) {
            fprintf(stderr, "ERROR: n debe ser un entero no negativo\n");
            return 2;
        }
        coeficientes = generar_pascal(n);
        if (coeficientes == NULL) {
            fprintf(stderr, "ERROR: sin memoria para n=%d\n", n);
            return 1;
        }
        printf("Coeficientes de (x+1)^%d:\n", n);
        imprimir_coeficientes(stdout, coeficientes, n);
        printf("Polinomio (x+1)^%d:\n", n);
        imprimir_polinomio(stdout, coeficientes, n);
        liberar_fila(coeficientes, n);
        return 0;
    }
    if (strcmp(argv[1], "eval") == 0) {
        int n;
        uint64_t x;

        if (argc < 4) {
            fprintf(stderr, "Uso: polinomio eval <n> <x>\n");
            return 2;
        }
        n = atoi(argv[2]);
        if (n < 0) {
            fprintf(stderr, "ERROR: n debe ser un entero no negativo\n");
            return 2;
        }
        if (argv[3][0] == '-') {
            fprintf(stderr, "ERROR: x debe ser un entero no negativo\n");
            return 2;
        }
        x = (uint64_t)strtoull(argv[3], NULL, 10);
        mostrar(stdout, n, x);
        return 0;
    }
    if (strcmp(argv[1], "tiempo") == 0) {
        int n;
        int repeticiones = 10;

        if (argc < 3) {
            fprintf(stderr, "Uso: polinomio tiempo <n> [repeticiones]\n");
            return 2;
        }
        n = atoi(argv[2]);
        if (n < 0) {
            fprintf(stderr, "ERROR: n debe ser un entero no negativo\n");
            return 2;
        }
        if (argc >= 4) {
            repeticiones = atoi(argv[3]);
        }
        if (repeticiones <= 0) {
            fprintf(stderr, "ERROR: las repeticiones deben ser mayores que cero\n");
            return 2;
        }
        return modo_tiempo(n, repeticiones);
    }

    fprintf(stderr, "ERROR: modo desconocido: %s\n", argv[1]);
    return 2;
}
