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

/*
 * ===========================================================================
 *  POR QUE MEMORIA DINAMICA (malloc/free) Y POR QUE PRECISION ARBITRARIA?
 * ===========================================================================
 *  El enunciado pide calcular los coeficientes de (x+1)^n con memoria
 *  dinamica. Para n=100 hay DOS motivos por los que no sirve un arreglo
 *  fijo de enteros normales:
 *
 *  1) CANTIDAD: la fila n tiene n+1 coeficientes. Si n cambia (10, 100,
 *     5000...) no podemos declarar siempre el mismo tamano de arreglo;
 *     necesitamos pedir la memoria exacta con malloc() y liberarla con
 *     free() al terminar. Eso es "memoria dinamica".
 *
 *  2) TAMANO de cada coeficiente: C(100,50) = 100891344545564193334812497256
 *     (29 digitos). Un unsigned long long llega como maximo a ~1.8e19
 *     (20 digitos): DESBORDARIA. Por eso implementamos un entero de
 *     precision arbitraria como arreglo de digitos decimales.
 *
 *  REPRESENTACION DEL BigInt (little endian, base 10):
 *      dig[0] = unidades, dig[1] = decenas, ..., dig[len-1] = mayor.
 *      Ejemplo: 4950 -> dig = [0, 5, 9, 4], len = 4.
 *  Base 10 (y no base 2^32) para que convertir a texto al imprimir sea
 *  trivial: cada digito ya es un digito decimal.
 * ===========================================================================
 */

/* Entero sin signo de precision arbitraria en base 10 (little endian).
 * Es una ESTRUCTURA que envuelve al arreglo dinamico: guardamos la
 * capacidad (cap) para saber cuanto podemos crecer sin volver a pedir
 * memoria, y la longitud real (len) con la que trabajamos. */
typedef struct {
    unsigned char *dig; /* digitos en base 10: dig[0] = unidades */
    int len;            /* cuantos digitos tiene el numero ahora */
    int cap;            /* cuanto espacio reservamos con malloc/realloc */
} BigInt;

/* Si malloc falla (no hay RAM), abortamos con un mensaje claro. */
static void fallo_memoria(void)
{
    fprintf(stderr, "ERROR: memoria insuficiente (malloc/realloc)\n");
    exit(1);
}

/* Reserva inicial del arreglo de digitos: 8 posiciones (cap=8, len=0).
 * Equivale a "declarar un arreglo pequeno" pero en el heap (malloc). */
static void bi_init(BigInt *b)
{
    b->cap = 8;
    b->len = 0;
    b->dig = (unsigned char *)malloc((size_t)b->cap);
    if (b->dig == NULL) {
        fallo_memoria();
    }
}

/*
 * Amplia el arreglo si no cabe lo que necesitamos: esto es REALLOC,
 * el corazon de la memoria dinamica. Solo se pide memoria nueva cuando
 * se necesita (doblemos la capacidad vieja para no pedir cada vez).
 * Si realloc devuelve NULL, el arreglo viejo sigue valido: por eso se
 * asigna el resultado directamente sobre el puntero y se verifica.
 */
static void bi_reserve(BigInt *b, int n)
{
    if (n <= b->cap) {
        return; /* aun cabe: no tocamos la memoria */
    }
    while (b->cap < n) {
        b->cap *= 2; /* crecemos en potencias de 2: menos llamadas */
    }
    b->dig = (unsigned char *)realloc(b->dig, (size_t)b->cap);
    if (b->dig == NULL) {
        fallo_memoria();
    }
}

/*
 * Asigna un valor pequeno (cabe en 64 bits) al BigInt dividiendolo
 * entre 10 sucesivamente: cada resto es un digito.
 *   4950 -> resto 0, luego 5, luego 9, luego 4  =>  dig = [0,5,9,4]
 * Es la misma idea que convertir un numero a string, pero guardamos
 * los digitos como bytes numericos, no como caracteres.
 */
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

/*
 * ===========================================================================
 *  SUMA DIGITO POR DIGITO (la operacion clave de todo el ejercicio)
 * ===========================================================================
 *  a += b  (igual que sumar a mano, de derecha a izquierda):
 *
 *      4950                    (a)
 *    +  617                    (b)
 *    -----
 *      5567
 *
 *  Recorremos posicion por posicion (unidades con unidades, decenas con
 *  decenas...). En cada una: d = acarreo + digito de a + digito de b.
 *  - El RESULTADO de esa columna es d % 10 (solo el digito de la columna).
 *  - El ACARREO para la siguiente columna es d / 10 (lo que "sobra").
 *  Ejemplo de columna: 9 + 7 = 16 -> escribimos 6 y acarreamos 1.
 *
 *  Si un numero tiene mas digitos que el otro (len distinto), las
 *  posiciones que no existen se tratan como 0 (los `if (i < len)`).
 *  Al final puede quedar acarreo pendiente: se agrega como digito extra
 *  y se recortan ceros a la izquierda (len) para no guardar basura.
 *
 *  El triangulo de Pascal solo necesita SUMAS (C(k,i)=C(k-1,i-1)+
 *  C(k-1,i)), asi que esta funcion es todo lo que hace falta para
 *  generar coeficientes gigantes sin desbordar.
 * ===========================================================================
 */
static void bi_add(BigInt *a, const BigInt *b)
{
    int n = (a->len > b->len) ? a->len : b->len; /* columnas a recorrer */
    int i, carry = 0; /* acarreo: empieza en 0, como al sumar a mano */
    bi_reserve(a, n + 1); /* por si el acarreo crea un digito nuevo */
    for (i = 0; i < n; i++) {
        int d = carry;
        if (i < a->len) {
            d += a->dig[i]; /* digito de a en esta columna */
        }
        if (i < b->len) {
            d += b->dig[i]; /* digito de b en esta columna */
        }
        a->dig[i] = (unsigned char)(d % 10); /* me quedo con la columna */
        carry = d / 10;                      /* paso el sobrante arriba */
    }
    if (carry > 0) {
        a->dig[i++] = (unsigned char)carry; /* acarreo final (ej. 999+1) */
    }
    a->len = i;
    while (a->len > 1 && a->dig[a->len - 1] == 0) {
        a->len--; /* recorto ceros insignificantes a la izquierda */
    }
}

/* Escribe el BigInt como texto: de la posicion len-1 a 0 (el arreglo
 * esta invertido: las unidades van primero por eficiencia). */
static void bi_print(FILE *out, const BigInt *b)
{
    int i;
    for (i = b->len - 1; i >= 0; i--) {
        fputc('0' + b->dig[i], out);
    }
}

/* Convierte a unsigned long long con verificacion de desbordamiento.
 * Devuelve 0 si el numero no cabe en 64 bits (para el paso a paso,
 * que solo demostramos con valores pequenos como n=5). */
static int bi_to_u64(const BigInt *b, unsigned long long *out)
{
    unsigned long long v = 0;
    int i;
    for (i = b->len - 1; i >= 0; i--) {
        if (v > (ULLONG_MAX - b->dig[i]) / 10ULL) {
            return 0; /* desbordaria */
        }
        v = v * 10ULL + b->dig[i];
    }
    *out = v;
    return 1;
}

/*
 * LIBERAR MEMORIA: cada BigInt que creamos con malloc debe devolverse
 * con free() al terminar, o habra fugas de memoria (memory leaks).
 * Primero libramos los digitos (dig) de cada elemento y despues el
 * arreglo de estructuras en si. Este par malloc...free es lo que el
 * profesor busca ver en la defensa.
 */
static void bi_free(BigInt *b)
{
    free(b->dig);   /* libero los digitos (heap) */
    b->dig = NULL;  /* apunto a NULL por seguridad (evita doble free) */
    b->len = 0;
    b->cap = 0;
}

/*
 * Genera la fila n del triangulo de Pascal en *fila (arreglo de n+1 BigInt
 * ya inicializados). Devuelve la longitud utilizada.
 *
 * Como funciona (y por que es memoria dinamica):
 *  - Usamos DOS arreglos grandes creados con calloc() (prev = fila k-1,
 *    cur = fila k): solo necesitamos la fila anterior, no toda la matriz.
 *    calloc = malloc que ademas pone todo en cero.
 *  - Identidad de Pascal: cur[i] = prev[i-1] + prev[i]  (suma de los dos
 *    "de arriba"), con bordes cur[0] = cur[k] = 1. Cada suma se hace con
 *    bi_add (digito por digito) sobre BigInts de precision arbitraria.
 *  - Al terminar cada fila, INTERCAMBIAMOS los punteros prev <-> cur
 *    (swap) en vez de copiar datos: es O(1) y la fila vieja queda lista
 *    para sobreescribirse en la siguiente iteracion.
 *  - Al final copiamos el resultado a *fila y LIBERAMOS TODO: primero
 *    cada BigInt con bi_free (sus digitos) y luego los dos arreglos con
 *    free(). Sin esto habria fugas de memoria en cada corrida.
 */
static int fila_pascal(int n, BigInt *fila)
{
    BigInt *prev = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    BigInt *cur = (BigInt *)calloc((size_t)n + 1, sizeof(BigInt));
    int k, i, len = 1;

    if (prev == NULL || cur == NULL) {
        fallo_memoria();
    }
    /* malloc inicializa con basura; calloc ya da ceros, pero cada
     * BigInt necesita su arreglo de digitos: bi_init hace ese malloc. */
    for (i = 0; i <= n; i++) {
        bi_init(&prev[i]);
        bi_init(&cur[i]);
    }
    bi_set_u64(&prev[0], 1); /* fila 0: [1] */

    for (k = 1; k <= n; k++) {
        BigInt *tmp;
        bi_set_u64(&cur[0], 1);   /* borde izquierdo C(k,0)=1 */
        bi_set_u64(&cur[k], 1);   /* borde derecho  C(k,k)=1 */
        for (i = 1; i < k; i++) {
            /* C(k,i) = C(k-1,i-1) + C(k-1,i), suma digito a digito */
            bi_set_u64(&cur[i], 0);        /* arranco en 0 */
            bi_add(&cur[i], &prev[i - 1]); /* + el de arriba-izquierda */
            bi_add(&cur[i], &prev[i]);     /* + el de arriba-derecha */
        }
        len = k + 1;
        /* swap prev <-> cur: la fila nueva pasa a ser la "anterior" */
        tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Copio el resultado (fila n) al arreglo del llamador y libero
     * toda la memoria dinamica usada: sin free() aqui habria leak. */
    for (i = 0; i <= n; i++) {
        bi_set_u64(&fila[i], 0);
        if (i < len) {
            bi_add(&fila[i], &prev[i]);
        }
        bi_free(&prev[i]); /* libera los digitos de cada BigInt */
        bi_free(&cur[i]);
    }
    free(prev); /* libera los dos arreglos de estructuras */
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
