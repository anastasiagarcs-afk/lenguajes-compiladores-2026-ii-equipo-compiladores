/*
 * Ejercicio 1 - Caballo de ajedrez (Prueba Diagnostica, UNEG 2026-II)
 * Participante 1: Fabiola Garcia (V28694007)
 *
 * Verifica una secuencia de movimientos de caballo desde una posicion
 * inicial. Reglas:
 *   1) cada movimiento comienza con 'K';
 *   2) la casilla destino es valida (columna a-h, fila 1-8);
 *   3) el desplazamiento es un salto de caballo (2+1 o 1+2).
 * Se detiene en el primer movimiento incorrecto (indice 1-based).
 *
 * Modos de uso:
 *   caballo.exe pruebas <archivo>
 *   caballo.exe tiempo <archivo> [repeticiones]
 *   caballo.exe validar <inicial> "<movimientos>"
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

/* Devuelve 1 si la casilla es valida; recibe columnas 0-7 y filas 0-7. */
static int parsear_casilla(const char *s, int *col, int *fil)
{
    char c;
    if (strlen(s) != 2) {
        return 0;
    }
    c = (char)tolower((unsigned char)s[0]);
    if (c < 'a' || c > 'h') {
        return 0;
    }
    if (s[1] < '1' || s[1] > '8') {
        return 0;
    }
    *col = c - 'a';
    *fil = s[1] - '1';
    return 1;
}

/* Valida la cadena de movimientos. Devuelve 1 si es correcta.
 * El mensaje de resultado se escribe en msg. */
static int validar(const char *inicial, const char *cadena_movs,
                   char *msg, size_t sz)
{
    int col, fil, i;
    char *copia, *tok;

    if (!parsear_casilla(inicial, &col, &fil)) {
        snprintf(msg, sz, "INCORRECTO posicion inicial invalida: '%s'", inicial);
        return 0;
    }

    copia = (char *)malloc(strlen(cadena_movs) + 1);
    if (copia == NULL) {
        snprintf(msg, sz, "ERROR: memoria insuficiente");
        return 0;
    }
    strcpy(copia, cadena_movs);

    i = 0;
    for (tok = strtok(copia, " \t\r\n"); tok != NULL;
         tok = strtok(NULL, " \t\r\n")) {
        int dcol, dfil;
        i++;
        if (tok[0] != 'K') {
            snprintf(msg, sz,
                     "INCORRECTO la letra '%c' del movimiento %d "
                     "no pertenece a un caballo",
                     tok[0] == '\0' ? '?' : tok[0], i);
            free(copia);
            return 0;
        }
        if (!parsear_casilla(tok + 1, &dcol, &dfil)) {
            snprintf(msg, sz, "INCORRECTO casilla invalida en el movimiento %d: '%s'",
                     i, tok);
            free(copia);
            return 0;
        }
        {
            int dx = dcol - col;
            int dy = dfil - fil;
            if (dx < 0) dx = -dx;
            if (dy < 0) dy = -dy;
            if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) {
                snprintf(msg, sz, "INCORRECTO falla en el movimiento %d", i);
                free(copia);
                return 0;
            }
        }
        col = dcol;
        fil = dfil;
    }
    free(copia);
    snprintf(msg, sz, "movimiento CORRECTO");
    return 1;
}

static void recortar_nl(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[--n] = '\0';
    }
}

static int modo_pruebas(const char *ruta)
{
    FILE *f = fopen(ruta, "r");
    char linea[8192];
    int total = 0, fallas = 0;

    if (f == NULL) {
        fprintf(stderr, "ERROR: no se pudo abrir %s\n", ruta);
        return 1;
    }
    while (fgets(linea, sizeof linea, f) != NULL) {
        char *p1, *p2, *p3, msg[512];
        recortar_nl(linea);
        if (linea[0] == '\0' || linea[0] == '#') {
            continue;
        }
        p1 = linea;
        p2 = strchr(p1, ';');
        if (p2 == NULL) {
            printf("SKIP (formato invalido): %s\n", linea);
            continue;
        }
        *p2++ = '\0';
        p3 = strchr(p2, ';');
        if (p3 == NULL) {
            printf("SKIP (formato invalido): %s\n", linea);
            continue;
        }
        *p3++ = '\0';

        validar(p1, p2, msg, sizeof msg);
        total++;
        if (strstr(msg, p3) != NULL) {
            printf("OK: %s | %s -> %s\n", p1, p2, msg);
        } else {
            fallas++;
            printf("FAIL: esperado '%s', obtenido '%s'\n", p3, msg);
        }
    }
    fclose(f);
    printf("Pruebas: %d ejecutadas, %d fallas\n", total, fallas);
    return fallas > 0 ? 1 : 0;
}

/* Carga archivo con linea 'inicial X' y movimientos; ignora '#' y lineas vacias. */
static int cargar_movimientos(const char *ruta, char **inicial, char ***movs,
                              int *n)
{
    FILE *f = fopen(ruta, "r");
    char linea[8192];
    int cap = 0;
    char *ini = NULL;
    char **lista = NULL;
    int total = 0;

    if (f == NULL) {
        fprintf(stderr, "ERROR: no se pudo abrir %s\n", ruta);
        return -1;
    }
    ini = (char *)malloc(16);
    if (ini == NULL) {
        fclose(f);
        return -1;
    }
    strcpy(ini, "a1");

    while (fgets(linea, sizeof linea, f) != NULL) {
        char *tok;
        recortar_nl(linea);
        if (linea[0] == '\0' || linea[0] == '#') {
            continue;
        }
        if (strncmp(linea, "inicial", 7) == 0 ||
            strncmp(linea, "Inicial", 7) == 0) {
            char tmp[16];
            if (sscanf(linea, "%*s %15s", tmp) == 1) {
                strcpy(ini, tmp);
            }
            continue;
        }
        for (tok = strtok(linea, " \t"); tok != NULL; tok = strtok(NULL, " \t")) {
            if (total == cap) {
                cap = (cap == 0) ? 64 : cap * 2;
                lista = (char **)realloc(lista, (size_t)cap * sizeof(char *));
                if (lista == NULL) {
                    fclose(f);
                    free(ini);
                    return -1;
                }
            }
            lista[total] = (char *)malloc(strlen(tok) + 1);
            if (lista[total] == NULL) {
                fclose(f);
                free(ini);
                return -1;
            }
            strcpy(lista[total], tok);
            total++;
        }
    }
    fclose(f);
    *inicial = ini;
    *movs = lista;
    *n = total;
    return 0;
}

static int modo_tiempo(const char *ruta, int repeticiones)
{
    char *inicial = NULL;
    char **movs = NULL;
    int n = 0, i, r;
    char *cadena = NULL;
    size_t tam = 0;
    char msg[512];
    double t0, dt;

    if (cargar_movimientos(ruta, &inicial, &movs, &n) != 0) {
        return 1;
    }
    if (n != 500) {
        fprintf(stderr, "AVISO: %s contiene %d movimientos (se esperaban 500)\n",
                ruta, n);
    }
    for (i = 0; i < n; i++) {
        tam += strlen(movs[i]) + 1;
    }
    cadena = (char *)malloc(tam + 1);
    if (cadena == NULL) {
        return 1;
    }
    cadena[0] = '\0';
    for (i = 0; i < n; i++) {
        if (i > 0) {
            strcat(cadena, " ");
        }
        strcat(cadena, movs[i]);
    }

    t0 = ahora();
    for (r = 0; r < repeticiones; r++) {
        validar(inicial, cadena, msg, sizeof msg);
    }
    dt = ahora() - t0;
    printf("c,%.6f,%d,%d\n", dt, n, repeticiones);

    for (i = 0; i < n; i++) {
        free(movs[i]);
    }
    free(movs);
    free(cadena);
    free(inicial);
    return 0;
}

static int modo_validar(const char *inicial, const char *movimientos)
{
    char msg[512];
    int ok = validar(inicial, movimientos, msg, sizeof msg);
    printf("%s\n", msg);
    return ok ? 0 : 1;
}

int main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "pruebas") == 0) {
        return modo_pruebas(argv[2]);
    }
    if (argc >= 3 && strcmp(argv[1], "tiempo") == 0) {
        int reps = (argc >= 4) ? atoi(argv[3]) : 10;
        return modo_tiempo(argv[2], reps);
    }
    if (argc >= 4 && strcmp(argv[1], "validar") == 0) {
        return modo_validar(argv[2], argv[3]);
    }
    fprintf(stderr,
            "Uso: caballo.exe pruebas <archivo> | "
            "tiempo <archivo> [repeticiones] | "
            "validar <inicial> \"<movimientos>\"\n");
    return 2;
}
