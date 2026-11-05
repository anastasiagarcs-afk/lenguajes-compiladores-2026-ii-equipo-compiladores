/*
 * Ejercicio 3 - Deteccion de palabras reservadas de C con traduccion.
 * Participante 1: Fabiola Garcia (V28694007) - UNEG 2026-II.
 *
 * Lee un programa C cargado dinamicamente en memoria (malloc), ignora
 * comentarios de linea y de bloque y cadenas ("..." y '...'), detecta las
 * palabras reservadas de la tabla y escribe el reporte:
 *
 *     palabra_original -> Traduccion, linea N, columna M
 *
 * Como extra, escribe una copia del programa con las palabras
 * reservadas reemplazadas por su traduccion en espanol.
 *
 * Uso: analizador.exe <entrada.c> <reporte.txt> <programa_traducido.c>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char *orig;
    const char *trad;
} Par;

static const Par TABLA[] = {
    {"int", "entero"},
    {"float", "flotante"},
    {"if", "si"},
    {"else", "sino"},
    {"while", "mientras"},
    {"for", "para"},
    {"return", "retornar"},
    {"void", "vacio"},
    {"char", "caracter"},
    {"double", "doble"},
    {"struct", "estructura"},
    {"break", "romper"},
    {"continue", "continuar"},
    {"switch", "segun"},
    {"case", "caso"},
    {"default", "defecto"},
};
#define N_TABLA ((int)(sizeof TABLA / sizeof TABLA[0]))

static const char *buscar(const char *palabra, int len)
{
    int i;
    for (i = 0; i < N_TABLA; i++) {
        if ((int)strlen(TABLA[i].orig) == len &&
            strncmp(TABLA[i].orig, palabra, (size_t)len) == 0) {
            return TABLA[i].trad;
        }
    }
    return NULL;
}

static int es_inicio_id(char c)
{
    return isalpha((unsigned char)c) || c == '_';
}

static int es_cuerpo_id(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

/* Salida dinamica (puede crecer: "continue" -> "continuar"). */
typedef struct {
    char *datos;
    size_t len;
    size_t cap;
} Buffer;

static void buf_init(Buffer *b, size_t cap_inicial)
{
    b->cap = (cap_inicial > 16) ? cap_inicial : 16;
    b->datos = (char *)malloc(b->cap);
    if (b->datos == NULL) {
        fprintf(stderr, "ERROR: memoria insuficiente (malloc)\n");
        exit(1);
    }
    b->len = 0;
}

static void buf_push(Buffer *b, char c)
{
    if (b->len + 1 >= b->cap) {
        b->cap *= 2;
        b->datos = (char *)realloc(b->datos, b->cap);
        if (b->datos == NULL) {
            fprintf(stderr, "ERROR: memoria insuficiente (realloc)\n");
            exit(1);
        }
    }
    b->datos[b->len++] = c;
}

static void buf_push_str(Buffer *b, const char *s)
{
    while (*s) {
        buf_push(b, *s++);
    }
}

/* Carga todo el archivo en memoria. Devuelve malloc'd NUL-terminated. */
static char *cargar_archivo(const char *ruta, size_t *out_len)
{
    FILE *f = fopen(ruta, "rb");
    long tam;
    char *datos;
    size_t leido;

    if (f == NULL) {
        fprintf(stderr, "ERROR: no se pudo abrir %s\n", ruta);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    tam = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (tam < 0) {
        fclose(f);
        return NULL;
    }
    datos = (char *)malloc((size_t)tam + 1);
    if (datos == NULL) {
        fclose(f);
        fprintf(stderr, "ERROR: memoria insuficiente (malloc)\n");
        return NULL;
    }
    leido = fread(datos, 1, (size_t)tam, f);
    fclose(f);
    datos[leido] = '\0';
    *out_len = leido;
    return datos;
}

static void capitalizar(const char *orig, char *dest, size_t sz)
{
    size_t n = strlen(orig);
    if (n >= sz) {
        n = sz - 1;
    }
    memcpy(dest, orig, n);
    dest[n] = '\0';
    if (n > 0) {
        dest[0] = (char)toupper((unsigned char)dest[0]);
    }
}

int main(int argc, char **argv)
{
    size_t tam = 0, i = 0;
    char *texto;
    Buffer salida;
    FILE *rep, *trad;
    int linea = 1, col = 1, encontradas = 0;
    int estado = 0; /* 0=code 1=linea 2=bloque 3=cadena 4=caracter */

    if (argc != 4) {
        fprintf(stderr,
                "Uso: analizador.exe <entrada.c> <reporte.txt> <programa_traducido.c>\n");
        return 2;
    }

    texto = cargar_archivo(argv[1], &tam);
    if (texto == NULL) {
        return 1;
    }

    /* Modo binario: conserva finales de linea LF igual que Python. */
    rep = fopen(argv[2], "wb");
    if (rep == NULL) {
        fprintf(stderr, "ERROR: no se pudo escribir %s\n", argv[2]);
        free(texto);
        return 1;
    }
    trad = fopen(argv[3], "wb");
    if (trad == NULL) {
        fprintf(stderr, "ERROR: no se pudo escribir %s\n", argv[3]);
        fclose(rep);
        free(texto);
        return 1;
    }

    buf_init(&salida, tam + 64);

    while (i < tam) {
        char c = texto[i];
        if (estado == 0) { /* codigo */
            if (c == '/' && i + 1 < tam && texto[i + 1] == '/') {
                estado = 1;
                buf_push(&salida, c);
                i++;
                col++;
            } else if (c == '/' && i + 1 < tam && texto[i + 1] == '*') {
                estado = 2;
                buf_push(&salida, c);
                i++;
                col++;
            } else if (c == '"') {
                estado = 3;
                buf_push(&salida, c);
                i++;
                col++;
            } else if (c == '\'') {
                estado = 4;
                buf_push(&salida, c);
                i++;
                col++;
            } else if (es_inicio_id(c)) {
                size_t j = i;
                int len;
                const char *trad_p;
                while (j < tam && es_cuerpo_id(texto[j])) {
                    j++;
                }
                len = (int)(j - i);
                trad_p = buscar(texto + i, len);
                if (trad_p != NULL) {
                    char cap[64];
                    capitalizar(trad_p, cap, sizeof cap);
                    fprintf(rep, "%.*s -> %s, linea %d, columna %d\n",
                            len, texto + i, cap, linea, col);
                    encontradas++;
                    buf_push_str(&salida, trad_p); /* minusc. en el archivo */
                } else {
                    size_t k;
                    for (k = 0; k < (size_t)len; k++) {
                        buf_push(&salida, texto[i + k]);
                    }
                }
                col += len;
                i = j;
            } else {
                buf_push(&salida, c);
                if (c == '\n') {
                    linea++;
                    col = 1;
                } else {
                    col++;
                }
                i++;
            }
        } else if (estado == 1) { /* comentario de linea */
            buf_push(&salida, c);
            i++;
            if (c == '\n') {
                estado = 0;
                linea++;
                col = 1;
            } else {
                col++;
            }
        } else if (estado == 2) { /* comentario de bloque */
            if (c == '*' && i + 1 < tam && texto[i + 1] == '/') {
                buf_push(&salida, '*');
                buf_push(&salida, '/');
                i += 2;
                col += 2;
                estado = 0;
            } else {
                buf_push(&salida, c);
                i++;
                if (c == '\n') {
                    linea++;
                    col = 1;
                } else {
                    col++;
                }
            }
        } else { /* cadena o caracter */
            char fin = (estado == 3) ? '"' : '\'';
            if (c == '\\' && i + 1 < tam) {
                buf_push(&salida, c);
                buf_push(&salida, texto[i + 1]);
                if (texto[i + 1] == '\n') {
                    linea++;
                    col = 1;
                } else {
                    col += 2;
                }
                i += 2;
            } else if (c == fin) {
                buf_push(&salida, c);
                i++;
                col++;
                estado = 0;
            } else {
                buf_push(&salida, c);
                i++;
                if (c == '\n') {
                    linea++;
                    col = 1;
                } else {
                    col++;
                }
            }
        }
    }

    buf_push(&salida, '\0');
    fwrite(salida.datos, 1, salida.len - 1, trad);
    fclose(rep);
    fclose(trad);
    free(salida.datos);
    free(texto);

    printf("OK: %d palabras reservadas encontradas (C)\n", encontradas);
    return 0;
}
