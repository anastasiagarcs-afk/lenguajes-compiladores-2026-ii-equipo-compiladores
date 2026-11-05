/*
 * Ejercicio 2 - Coeficientes de (x+1)^n con memoria dinamica (malloc/free).
 * Participante 1: Fabiola Garcia (V28694007) - UNEG 2026-II.
 *
 * Los coeficientes del triangulo de Pascal para n=100 superan los 64 bits
 * (C(100,50) ~ 1.0089e29), por lo que se implementa un entero de
 * precision arbitraria en base 10 (arreglo de digitos dinamico).
 *
 * Modos:
 *   polinomio.exe demo             # n=5, x=2 (paso a paso)
 *   polinomio.exe poly <n>         # imprime el polinomio (x+1)^n
 *   polinomio.exe tiempo [n]       # mide la generacion (por defecto n=100)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>

static double ahora(void)
{
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER counter;
    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}
#else
#include <time.h>

static double ahora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}
#endif

/* Entero sin signo de precision arbitraria en base 10 (little endian). */
typedef struct {
    unsigned char *dig; /* dig[0] = unidades */
    int len;
    int cap;
} BigInt;

static void fallo_memoria(void)
{
    fprintf(stderr, "ERROR: memoria insuficiente (malloc/realloc)\n");
    exit(1);
}

static void bi_init(BigInt *b)
{
    b->cap = 8;
    b->len = 0;
    b->dig = (unsigned char *)malloc((size_t)b->cap);
    if (b->dig == NULL) {
        fallo_memoria();
    }
}

static void bi_reserve(BigInt *b, int n)
{
    if (n <= b->cap) {
        return;
    }
    while (b->cap < n) {
        b->cap *= 2;
    }
    b->dig = (unsigned char *)realloc(b->dig, (size_t)b->cap);
    if (b->dig == NULL) {
        fallo_memoria();
    }
}

static void bi_set_u64(BigInt *b, unsigned long long v)
{
    b->len = 0;
    if (v == 0) {
        bi_reserve(b, 1);
        b->dig[0] = 0;
        b->len = 1;
        return;
    }
    while (v > 0) {
        bi_reserve(b, b->len + 1);
        b->dig[b->len++] = (unsigned char)(v % 10ULL);
        v /= 10ULL;
    }
}

/* a += b (b puede ser distinto de a; se usa siempre desde Pascal). */
static void bi_add(BigInt *a, const BigInt *b)
{
    int n = (a->len > b->len) ? a->len : b->len;
    int i, carry = 0;
    bi_reserve(a, n + 1);
    for (i = 0; i < n; i++) {
        int d = carry;
        if (i < a->len) {
            d += a->dig[i];
        }
        if (i < b->len) {
            d += b->dig[i];
        }
        a->dig[i] = (unsigned char)(d % 10);
        carry = d / 10;
    }
    if (carry > 0) {
        a->dig[i++] = (unsigned char)carry;
    }
    a->len = i;
    while (a->len > 1 && a->dig[a->len - 1] == 0) {
        a->len--;
    }
}

static void bi_print(FILE *out, const BigInt *b)
{
    int i;
    for (i = b->len - 1; i >= 0; i--) {
        fputc('0' + b->dig[i], out);
    }
}

/* Conversion a 64 bits sin signo; devuelve 0 si desborda. */
static int bi_to_u64(const BigInt *b, unsigned long long *out)
{
    unsigned long long v = 0;
    int i;
    for (i = b->len - 1; i >= 0; i--) {
        if (v > (ULLONG_MAX - b->dig[i]) / 10ULL) {
            return 0;
        }
        v = v * 10ULL + b->dig[i];
    }
    *out = v;
    return 1;
}

static void bi_free(BigInt *b)
{
    free(b->dig);
    b->dig = NULL;
    b->len = 0;
    b->cap = 0;
}

/*
 * Genera la fila n del triangulo de Pascal en *fila (arreglo de n+1 BigInt
 * ya inicializados). Devuelve la longitud utilizada.
 * Dos arreglos dinamicos (prev/cur) se alternan como en el enunciado.
 */
static int fila_pascal(int n, BigInt *fila)
{
    BigInt *prev = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    BigInt *cur = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    int k, i, len = 1;

    if (prev == NULL || cur == NULL) {
        fallo_memoria();
    }
    for (i = 0; i <= n; i++) {
        bi_init(&prev[i]);
        bi_init(&cur[i]);
    }
    bi_set_u64(&prev[0], 1);

    for (k = 1; k <= n; k++) {
        BigInt *tmp;
        bi_set_u64(&cur[0], 1);
        bi_set_u64(&cur[k], 1);
        for (i = 1; i < k; i++) {
            bi_set_u64(&cur[i], 0);
            bi_add(&cur[i], &prev[i - 1]);
            bi_add(&cur[i], &prev[i]);
        }
        len = k + 1;
        tmp = prev;
        prev = cur;
        cur = tmp;
    }

    for (i = 0; i <= n; i++) {
        bi_set_u64(&fila[i], 0);
        if (i < len) {
            bi_add(&fila[i], &prev[i]);
        }
        bi_free(&prev[i]);
        bi_free(&cur[i]);
    }
    free(prev);
    free(cur);
    return len;
}

static void mostrar_polinomio(int n, BigInt *coefs)
{
    int k;
    printf("(x+1)^%d = ", n);
    for (k = n; k >= 0; k--) {
        if (k < n) {
            printf(" + ");
        }
        bi_print(stdout, &coefs[k]);
        printf("*x^%d", k);
    }
    printf("\n");
}

static void evaluar_paso_a_paso(int n, BigInt *coefs, int x)
{
    int k;
    unsigned long long acumulado = 0;
    unsigned long long potencia = 1;

    printf("Calculo paso a paso de f(%d) = (%d+1)^%d con los coeficientes generados:\n",
           x, x, n);
    for (k = 0; k <= n; k++) {
        unsigned long long ck = 0, termino;
        if (!bi_to_u64(&coefs[k], &ck)) {
            printf("  paso %d: coeficiente demasiado grande para 64 bits\n", k + 1);
            return;
        }
        if (k > 0) {
            if (x != 0 && potencia > ULLONG_MAX / (unsigned long long)x) {
                printf("  paso %d: potencia %d^%d desborda 64 bits\n", k + 1, x, k);
                return;
            }
            potencia *= (unsigned long long)x;
        }
        termino = ck * potencia;
        acumulado += termino;
        printf("  paso %d: C(%d,%d)*%d^%d = %llu*%llu = %llu   acumulado = %llu\n",
               k + 1, n, k, x, k, ck, potencia, termino, acumulado);
    }
    printf("f(%d) = %llu\n", x, acumulado);
    {
        unsigned long long verif = 1;
        int j;
        for (j = 0; j < n; j++) {
            verif *= (unsigned long long)(x + 1);
        }
        printf("Verificacion: (%d+1)^%d = %llu\n", x, n, verif);
    }
}

static void liberar(BigInt *coefs, int n)
{
    int i;
    for (i = 0; i <= n; i++) {
        bi_free(&coefs[i]);
    }
    free(coefs);
}

static int modo_demo(void)
{
    int n = 5, x = 2, i;
    BigInt *coefs = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    if (coefs == NULL) {
        fallo_memoria();
    }
    for (i = 0; i <= n; i++) {
        bi_init(&coefs[i]);
    }
    fila_pascal(n, coefs);
    mostrar_polinomio(n, coefs);
    printf("\n");
    evaluar_paso_a_paso(n, coefs, x);
    liberar(coefs, n);
    return 0;
}

static int modo_poly(int n)
{
    int i;
    BigInt *coefs;
    if (n < 0 || n > 5000) {
        fprintf(stderr, "ERROR: n debe estar entre 0 y 5000\n");
        return 1;
    }
    coefs = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    if (coefs == NULL) {
        fallo_memoria();
    }
    for (i = 0; i <= n; i++) {
        bi_init(&coefs[i]);
    }
    fila_pascal(n, coefs);
    mostrar_polinomio(n, coefs);
    liberar(coefs, n);
    return 0;
}

static int modo_tiempo(int n)
{
    int i;
    BigInt *coefs;
    double t0, dt;
    if (n < 0 || n > 5000) {
        fprintf(stderr, "ERROR: n debe estar entre 0 y 5000\n");
        return 1;
    }
    coefs = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    if (coefs == NULL) {
        fallo_memoria();
    }
    for (i = 0; i <= n; i++) {
        bi_init(&coefs[i]);
    }
    t0 = ahora();
    fila_pascal(n, coefs);
    dt = ahora() - t0;
    printf("c,%.6f,%d\n", dt, n);
    liberar(coefs, n);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc >= 2 && strcmp(argv[1], "demo") == 0) {
        return modo_demo();
    }
    if (argc >= 3 && strcmp(argv[1], "poly") == 0) {
        return modo_poly(atoi(argv[2]));
    }
    if (argc >= 2 && strcmp(argv[1], "tiempo") == 0) {
        int n = (argc >= 3) ? atoi(argv[2]) : 100;
        return modo_tiempo(n);
    }
    fprintf(stderr,
            "Uso: polinomio.exe demo | poly <n> | tiempo [n]\n");
    return 2;
}
