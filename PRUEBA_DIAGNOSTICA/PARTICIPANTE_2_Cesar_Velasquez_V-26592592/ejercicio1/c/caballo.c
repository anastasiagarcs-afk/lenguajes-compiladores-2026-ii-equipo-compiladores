/*
 * Verificacion de movimientos de un caballo de ajedrez.
 *
 * Uso:
 *   caballo validar <inicial> "<movimientos>" [n]
 *   caballo pruebas <archivo>
 *   caballo tiempo <archivo> [repeticiones]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    int columna;
    int fila;
} Casilla;

static char *duplicar(const char *texto)
{
    size_t largo = strlen(texto) + 1;
    char *copia = (char *)malloc(largo);
    if (copia != NULL) {
        memcpy(copia, texto, largo);
    }
    return copia;
}

static int analizar_casilla(const char *texto, Casilla *casilla)
{
    char columna;
    char fila;

    if (texto == NULL || strlen(texto) != 2) {
        return 0;
    }
    columna = texto[0];
    fila = texto[1];
    if (columna < 'a' || columna > 'h' || fila < '1' || fila > '8') {
        return 0;
    }
    casilla->columna = columna - 'a';
    casilla->fila = fila - '1';
    return 1;
}

static int es_salto_caballo(Casilla a, Casilla b)
{
    int dx = abs(a.columna - b.columna);
    int dy = abs(a.fila - b.fila);
    return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

static int validar(const char *inicial, char **movimientos, int cantidad,
                   char *mensaje, size_t tam_mensaje)
{
    Casilla posicion;
    int i;

    if (!analizar_casilla(inicial, &posicion)) {
        snprintf(mensaje, tam_mensaje,
                 "INCORRECTO posicion inicial invalida: %s", inicial);
        return 0;
    }

    for (i = 0; i < cantidad; i++) {
        const char *movimiento = movimientos[i];
        Casilla destino;

        if (movimiento == NULL || movimiento[0] == '\0') {
            continue;
        }
        if (movimiento[0] != 'K') {
            snprintf(mensaje, tam_mensaje,
                     "movimiento INCORRECTO falla letra %c no pertenece a un caballo",
                     movimiento[0]);
            return 0;
        }
        if (strlen(movimiento) != 3 ||
            !analizar_casilla(movimiento + 1, &destino)) {
            snprintf(mensaje, tam_mensaje,
                     "INCORRECTO falla en el movimiento %d", i + 1);
            return 0;
        }
        if (!es_salto_caballo(posicion, destino)) {
            snprintf(mensaje, tam_mensaje,
                     "INCORRECTO falla en el movimiento %d", i + 1);
            return 0;
        }
        posicion = destino;
    }

    snprintf(mensaje, tam_mensaje, "movimiento CORRECTO");
    return 1;
}

static char **dividir(char *texto, int *cantidad)
{
    char **partes = NULL;
    int usados = 0;
    int capacidad = 0;
    char *p = texto;

    while (*p != '\0') {
        char *inicio;

        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        inicio = p;
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') {
            p++;
        }
        if (*p != '\0') {
            *p = '\0';
            p++;
        }
        if (usados == capacidad) {
            int nueva_capacidad = capacidad == 0 ? 16 : capacidad * 2;
            char **nuevas = (char **)realloc(partes, (size_t)nueva_capacidad * sizeof(char *));
            if (nuevas == NULL) {
                free(partes);
                *cantidad = 0;
                return NULL;
            }
            partes = nuevas;
            capacidad = nueva_capacidad;
        }
        partes[usados++] = inicio;
    }

    *cantidad = usados;
    return partes;
}

static char *leer_archivo(const char *ruta, size_t *largo)
{
    FILE *archivo = fopen(ruta, "rb");
    long tam;
    char *contenido;
    size_t leidos;

    if (archivo == NULL) {
        return NULL;
    }
    if (fseek(archivo, 0, SEEK_END) != 0) {
        fclose(archivo);
        return NULL;
    }
    tam = ftell(archivo);
    if (tam < 0) {
        fclose(archivo);
        return NULL;
    }
    rewind(archivo);
    contenido = (char *)malloc((size_t)tam + 1);
    if (contenido == NULL) {
        fclose(archivo);
        return NULL;
    }
    leidos = fread(contenido, 1, (size_t)tam, archivo);
    fclose(archivo);
    contenido[leidos] = '\0';
    if (largo != NULL) {
        *largo = leidos;
    }
    return contenido;
}

static int cargar_movimientos(const char *ruta, char *inicial,
                              size_t tam_inicial, char ***movimientos,
                              int *cantidad)
{
    size_t largo;
    char *texto = leer_archivo(ruta, &largo);
    char *p;
    char **lista = NULL;
    int usados = 0;
    int capacidad = 0;
    int primero = 1;

    (void)largo;
    if (texto == NULL) {
        return 0;
    }
    snprintf(inicial, tam_inicial, "a2");

    p = texto;
    while (*p != '\0') {
        char *linea = p;
        char *q;
        char **partes;
        int npartes;
        int i;

        while (*p != '\0' && *p != '\n') {
            p++;
        }
        if (*p == '\n') {
            *p = '\0';
            p++;
        }
        q = linea;
        while (*q == ' ' || *q == '\t' || *q == '\r') {
            q++;
        }
        if (*q == '\0' || *q == '#') {
            continue;
        }

        partes = dividir(q, &npartes);
        if (partes == NULL) {
            free(texto);
            return 0;
        }
        for (i = 0; i < npartes; i++) {
            char *token = partes[i];
            if (primero && strcmp(token, "inicial") == 0) {
                if (i + 1 < npartes) {
                    snprintf(inicial, tam_inicial, "%s", partes[i + 1]);
                    i++;
                }
                primero = 0;
                continue;
            }
            primero = 0;
            if (usados == capacidad) {
                int nueva_capacidad = capacidad == 0 ? 64 : capacidad * 2;
                char **nuevas = (char **)realloc(lista, (size_t)nueva_capacidad * sizeof(char *));
                if (nuevas == NULL) {
                    int k;
                    for (k = 0; k < usados; k++) {
                        free(lista[k]);
                    }
                    free(lista);
                    free(partes);
                    free(texto);
                    return 0;
                }
                lista = nuevas;
                capacidad = nueva_capacidad;
            }
            lista[usados] = duplicar(token);
            if (lista[usados] == NULL) {
                int k;
                for (k = 0; k < usados; k++) {
                    free(lista[k]);
                }
                free(lista);
                free(partes);
                free(texto);
                return 0;
            }
            usados++;
        }
        free(partes);
    }

    free(texto);
    *movimientos = lista;
    *cantidad = usados;
    return 1;
}

static void liberar_movimientos(char **movimientos, int cantidad)
{
    int i;
    for (i = 0; i < cantidad; i++) {
        free(movimientos[i]);
    }
    free(movimientos);
}

static void recortar(char *texto)
{
    char *inicio = texto;
    char *fin;

    while (*inicio == ' ' || *inicio == '\t' || *inicio == '\r' || *inicio == '\n') {
        inicio++;
    }
    if (inicio != texto) {
        memmove(texto, inicio, strlen(inicio) + 1);
    }
    fin = texto + strlen(texto);
    while (fin > texto && (fin[-1] == ' ' || fin[-1] == '\t' || fin[-1] == '\r' || fin[-1] == '\n')) {
        fin--;
        *fin = '\0';
    }
}

static int modo_pruebas(const char *ruta)
{
    size_t largo;
    char *texto = leer_archivo(ruta, &largo);
    char *p;
    int total = 0;
    int correctos = 0;

    (void)largo;
    if (texto == NULL) {
        fprintf(stderr, "ERROR: no se pudo leer %s\n", ruta);
        return 2;
    }

    p = texto;
    while (*p != '\0') {
        char *linea = p;
        char *s1;
        char *s2;
        char *inicial;
        char *movimientos_txt;
        char *esperado;
        char *copia;
        char **movimientos;
        int cantidad;
        char mensaje[256];

        while (*p != '\0' && *p != '\n') {
            p++;
        }
        if (*p == '\n') {
            *p = '\0';
            p++;
        }
        recortar(linea);
        if (*linea == '\0' || *linea == '#') {
            continue;
        }
        s1 = strchr(linea, ';');
        if (s1 == NULL) {
            continue;
        }
        *s1 = '\0';
        s2 = strchr(s1 + 1, ';');
        if (s2 == NULL) {
            continue;
        }
        *s2 = '\0';
        inicial = linea;
        movimientos_txt = s1 + 1;
        esperado = s2 + 1;
        recortar(inicial);
        recortar(movimientos_txt);
        recortar(esperado);

        copia = duplicar(movimientos_txt);
        if (copia == NULL) {
            free(texto);
            return 2;
        }
        movimientos = dividir(copia, &cantidad);
        if (movimientos == NULL) {
            free(copia);
            free(texto);
            return 2;
        }

        total++;
        validar(inicial, movimientos, cantidad, mensaje, sizeof(mensaje));
        if (strcmp(mensaje, esperado) == 0) {
            correctos++;
            printf("CASO %d: OK -> %s\n", total, mensaje);
        } else {
            printf("CASO %d: ERROR -> esperado '%s' obtenido '%s'\n",
                   total, esperado, mensaje);
        }

        free(movimientos);
        free(copia);
    }

    printf("RESULTADO: %d/%d OK\n", correctos, total);
    free(texto);
    return (total > 0 && correctos == total) ? 0 : 1;
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

static int modo_tiempo(const char *ruta, int repeticiones)
{
    char inicial[8];
    char **movimientos = NULL;
    int cantidad = 0;
    char mensaje[256];
    double inicio;
    double total;
    int i;

    if (!cargar_movimientos(ruta, inicial, sizeof(inicial), &movimientos, &cantidad)) {
        fprintf(stderr, "ERROR: no se pudo leer %s\n", ruta);
        return 2;
    }
    if (!validar(inicial, movimientos, cantidad, mensaje, sizeof(mensaje))) {
        fprintf(stderr, "ERROR: %s\n", mensaje);
        liberar_movimientos(movimientos, cantidad);
        return 1;
    }

    inicio = ahora();
    for (i = 0; i < repeticiones; i++) {
        if (!validar(inicial, movimientos, cantidad, mensaje, sizeof(mensaje))) {
            fprintf(stderr, "ERROR: %s\n", mensaje);
            liberar_movimientos(movimientos, cantidad);
            return 1;
        }
    }
    total = ahora() - inicio;

    printf("c,%.6f,%d,%d\n", total, cantidad, repeticiones);
    liberar_movimientos(movimientos, cantidad);
    return 0;
}

static int modo_validar(const char *inicial, const char *texto, int n)
{
    char *copia = duplicar(texto);
    char **movimientos;
    int cantidad;
    char mensaje[256];

    if (copia == NULL) {
        fprintf(stderr, "ERROR: sin memoria\n");
        return 2;
    }
    movimientos = dividir(copia, &cantidad);
    if (movimientos == NULL) {
        free(copia);
        fprintf(stderr, "ERROR: sin memoria\n");
        return 2;
    }
    if (n >= 0 && n != cantidad) {
        fprintf(stderr, "AVISO: n=%d pero se recibieron %d movimientos\n", n, cantidad);
    }
    validar(inicial, movimientos, cantidad, mensaje, sizeof(mensaje));
    printf("%s\n", mensaje);
    free(movimientos);
    free(copia);
    return strcmp(mensaje, "movimiento CORRECTO") == 0 ? 0 : 1;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr,
                "Uso: caballo {validar|pruebas|tiempo} ...\n"
                "  validar <inicial> \"<movimientos>\" [n]\n"
                "  pruebas <archivo>\n"
                "  tiempo <archivo> [repeticiones]\n");
        return 2;
    }

    if (strcmp(argv[1], "validar") == 0) {
        int n = -1;
        if (argc < 4) {
            fprintf(stderr, "Uso: caballo validar <inicial> \"<movimientos>\" [n]\n");
            return 2;
        }
        if (argc >= 5) {
            n = atoi(argv[4]);
        }
        return modo_validar(argv[2], argv[3], n);
    }
    if (strcmp(argv[1], "pruebas") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Uso: caballo pruebas <archivo>\n");
            return 2;
        }
        return modo_pruebas(argv[2]);
    }
    if (strcmp(argv[1], "tiempo") == 0) {
        int repeticiones = 10;
        if (argc < 3) {
            fprintf(stderr, "Uso: caballo tiempo <archivo> [repeticiones]\n");
            return 2;
        }
        if (argc >= 4) {
            repeticiones = atoi(argv[3]);
        }
        if (repeticiones <= 0) {
            fprintf(stderr, "ERROR: las repeticiones deben ser mayores que cero\n");
            return 2;
        }
        return modo_tiempo(argv[2], repeticiones);
    }

    fprintf(stderr, "ERROR: modo desconocido: %s\n", argv[1]);
    return 2;
}
