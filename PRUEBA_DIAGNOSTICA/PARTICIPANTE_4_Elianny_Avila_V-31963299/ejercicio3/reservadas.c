#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estructura para nuestro diccionario
typedef struct {
    char *ingles;
    char *espanol;
} Traduccion;

// Diccionario de palabras reservadas
Traduccion dicc[] = {
    {"int", "entero"}, {"char", "caracter"}, {"float", "flotante"}, {"double", "doble"},
    {"void", "vacio"}, {"if", "si"}, {"else", "sino"}, {"for", "para"}, 
    {"while", "mientras"}, {"return", "retornar"}, {"break", "romper"}, 
    {"continue", "continuar"}, {"struct", "estructura"}, {"const", "constante"},
    {"sizeof", "tamano_de"}, {"switch", "segun"}, {"case", "caso"}, {"default", "defecto"}
};
int num_palabras = sizeof(dicc) / sizeof(dicc[0]);

// Función para buscar si una palabra es reservada
char* traducir(char *palabra) {
    for (int i = 0; i < num_palabras; i++) {
        if (strcmp(palabra, dicc[i].ingles) == 0) {
            return dicc[i].espanol;
        }
    }
    return NULL; // No es reservada
}

int main() {
    FILE *archivo = fopen("programa_prueba.c", "r");
    if (!archivo) {
        printf("Error: No se encontro programa_prueba.c\n");
        return 1;
    }

    // 1. CARGAR EN MEMORIA DINÁMICA
    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *buffer = (char*)malloc(tamano + 1);
    if (!buffer) {
        printf("Error de memoria\n");
        fclose(archivo);
        return 1;
    }
    fread(buffer, 1, tamano, archivo);
    buffer[tamano] = '\0';
    fclose(archivo);

    printf("--- ANALIZANDO PROGRAMA C ---\n\n");

    // 2. BUSCAR Y TRADUCIR PALABRAS
    // Usamos strtok para separar el texto por espacios, saltos de línea y símbolos
    char *delimitadores = " \n\t\r(){};,=+-*/<>!&|\"";
    char *token = strtok(buffer, delimitadores);

    FILE *salida = fopen("resultados/traduccion.txt", "w");
    if (salida) fprintf(salida, "--- PALABRAS RESERVADAS ENCONTRADAS ---\n");

    while (token != NULL) {
        char *trad = traducir(token);
        if (trad != NULL) {
            printf("Encontrada: '%s' -> Traduccion: '%s'\n", token, trad);
            if (salida) fprintf(salida, "'%s' -> '%s'\n", token, trad);
        }
        token = strtok(NULL, delimitadores);
    }

    if (salida) {
        fclose(salida);
        printf("\nReporte guardado en resultados/traduccion.txt\n");
    }

    // Liberar memoria dinámica
    free(buffer);
    return 0;
}