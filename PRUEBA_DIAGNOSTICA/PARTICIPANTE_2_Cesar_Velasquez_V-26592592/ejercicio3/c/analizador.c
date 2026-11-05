/*
 * Detecta palabras reservadas de C y las traduce al espanol.
 * El archivo se carga completo en memoria con malloc.
 *
 * Uso:
 *   analizador <entrada.c> <reporte.txt> [traducido.c]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char *palabra;
    const char *espanol;
    const char *minusculas;
} Traduccion;

static const Traduccion TABLA[] = {
    {"auto", "Automático", "automático"},
    {"break", "Romper", "romper"},
    {"case", "Caso", "caso"},
    {"char", "Carácter", "carácter"},
    {"const", "Constante", "constante"},
    {"continue", "Continuar", "continuar"},
    {"default", "Defecto", "defecto"},
    {"do", "Hacer", "hacer"},
    {"double", "Doble", "doble"},
    {"else", "Sino", "sino"},
    {"enum", "Enumeración", "enumeración"},
    {"extern", "Externo", "externo"},
    {"float", "Flotante", "flotante"},
    {"for", "Para", "para"},
    {"goto", "Ir a", "ir a"},
    {"if", "Si", "si"},
    {"inline", "En línea", "en línea"},
    {"int", "Entero", "entero"},
    {"long", "Largo", "largo"},
    {"register", "Registro", "registro"},
    {"restrict", "Restringido", "restringido"},
    {"return", "Retornar", "retornar"},
    {"short", "Corto", "corto"},
    {"signed", "Con signo", "con signo"},
    {"sizeof", "Tamaño de", "tamaño de"},
    {"static", "Estático", "estático"},
    {"struct", "Estructura", "estructura"},
    {"switch", "Según", "según"},
    {"typedef", "Tipo definido", "tipo definido"},
    {"union", "Unión", "unión"},
    {"unsigned", "Sin signo", "sin signo"},
    {"void", "Vacío", "vacío"},
    {"volatile", "Volátil", "volátil"},
    {"while", "Mientras", "mientras"},
    {"_Bool", "Booleano", "booleano"},
    {"_Complex", "Complejo", "complejo"},
    {"_Imaginary", "Imaginario", "imaginario"},
    {"_Alignas", "Alinear como", "alinear como"},
    {"_Alignof", "Alineación de", "alineación de"},
    {"_Atomic", "Atómico", "atómico"},
    {"_Generic", "Genérico", "genérico"},
    {"_Noreturn", "Sin retorno", "sin retorno"},
    {"_Static_assert", "Afirmación estática", "afirmación estática"},
    {"_Thread_local", "Hilo local", "hilo local"},
};

#define TOTAL_PALABRAS (sizeof(TABLA) / sizeof(TABLA[0]))

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

static int buscar_traduccion(const char *palabra, const char **espanol,
                             const char **minusculas)
{
    size_t i;
    for (i = 0; i < TOTAL_PALABRAS; i++) {
        if (strcmp(palabra, TABLA[i].palabra) == 0) {
            *espanol = TABLA[i].espanol;
            *minusculas = TABLA[i].minusculas;
            return 1;
        }
    }
    return 0;
}

static int es_inicio_identificador(unsigned char c)
{
    return isalpha(c) || c == '_';
}

static int es_parte_identificador(unsigned char c)
{
    return isalnum(c) || c == '_';
}

int main(int argc, char **argv)
{
    const char *ruta_entrada;
    const char *ruta_reporte;
    const char *ruta_traducido;
    size_t largo;
    char *texto;
    FILE *reporte;
    FILE *traducido;
    size_t i = 0;
    int linea = 1;
    int columna = 1;
    int encontradas = 0;

    if (argc < 3) {
        fprintf(stderr, "Uso: analizador <entrada.c> <reporte.txt> [traducido.c]\n");
        return 2;
    }
    ruta_entrada = argv[1];
    ruta_reporte = argv[2];
    ruta_traducido = argc >= 4 ? argv[3] : NULL;

    texto = leer_archivo(ruta_entrada, &largo);
    if (texto == NULL) {
        fprintf(stderr, "ERROR: no se pudo leer %s\n", ruta_entrada);
        return 1;
    }

    reporte = fopen(ruta_reporte, "wb");
    if (reporte == NULL) {
        fprintf(stderr, "ERROR: no se pudo crear %s\n", ruta_reporte);
        free(texto);
        return 1;
    }
    traducido = NULL;
    if (ruta_traducido != NULL) {
        traducido = fopen(ruta_traducido, "wb");
        if (traducido == NULL) {
            fprintf(stderr, "ERROR: no se pudo crear %s\n", ruta_traducido);
            fclose(reporte);
            free(texto);
            return 1;
        }
    }

    while (i < largo) {
        unsigned char c = (unsigned char)texto[i];

        if (c == '/' && i + 1 < largo && texto[i + 1] == '/') {
            while (i < largo && texto[i] != '\n') {
                if (traducido != NULL) {
                    fputc(texto[i], traducido);
                }
                i++;
                columna++;
            }
            continue;
        }

        if (c == '/' && i + 1 < largo && texto[i + 1] == '*') {
            if (traducido != NULL) {
                fputc('/', traducido);
                fputc('*', traducido);
            }
            i += 2;
            columna += 2;
            while (i < largo) {
                if (texto[i] == '*' && i + 1 < largo && texto[i + 1] == '/') {
                    if (traducido != NULL) {
                        fputc('*', traducido);
                        fputc('/', traducido);
                    }
                    i += 2;
                    columna += 2;
                    break;
                }
                if (traducido != NULL) {
                    fputc(texto[i], traducido);
                }
                if (texto[i] == '\n') {
                    linea++;
                    columna = 1;
                } else {
                    columna++;
                }
                i++;
            }
            continue;
        }

        if (c == '"' || c == '\'') {
            char comilla = (char)c;
            if (traducido != NULL) {
                fputc(c, traducido);
            }
            i++;
            columna++;
            while (i < largo) {
                char d = texto[i];
                if (traducido != NULL) {
                    fputc(d, traducido);
                }
                if (d == '\\' && i + 1 < largo) {
                    char escapado;
                    i++;
                    escapado = texto[i];
                    if (traducido != NULL) {
                        fputc(escapado, traducido);
                    }
                    if (escapado == '\n') {
                        linea++;
                        columna = 1;
                    } else {
                        columna += 2;
                    }
                    i++;
                    continue;
                }
                if (d == '\n') {
                    linea++;
                    columna = 1;
                } else {
                    columna++;
                }
                i++;
                if (d == comilla) {
                    break;
                }
            }
            continue;
        }

        if (es_inicio_identificador(c)) {
            size_t j = i + 1;
            size_t longitud;
            char palabra[64];

            while (j < largo && es_parte_identificador((unsigned char)texto[j])) {
                j++;
            }
            longitud = j - i;
            if (longitud < sizeof(palabra)) {
                const char *espanol;
                const char *minusculas;
                memcpy(palabra, texto + i, longitud);
                palabra[longitud] = '\0';
                if (buscar_traduccion(palabra, &espanol, &minusculas)) {
                    fprintf(reporte, "%s -> %s, linea %d, columna %d\n",
                            palabra, espanol, linea, columna);
                    if (traducido != NULL) {
                        fputs(minusculas, traducido);
                    }
                    encontradas++;
                } else if (traducido != NULL) {
                    fwrite(texto + i, 1, longitud, traducido);
                }
            } else if (traducido != NULL) {
                fwrite(texto + i, 1, longitud, traducido);
            }
            columna += (int)longitud;
            i = j;
            continue;
        }

        if (traducido != NULL) {
            fputc(c, traducido);
        }
        if (c == '\n') {
            linea++;
            columna = 1;
        } else {
            columna++;
        }
        i++;
    }

    fclose(reporte);
    if (traducido != NULL) {
        fclose(traducido);
    }
    free(texto);
    printf("Encontradas: %d\n", encontradas);
    return 0;
}
