#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Diccionario de palabras reservadas en C y su traduccion al español
const char *reservadasC[] = {
    "int", "return", "if", "else", "while", "for", 
    "void", "char", "float", "double", "struct", 
    "break", "continue", "switch", "case", "default"
};
const char *reservadasESP[] = {
    "entero", "retornar", "si", "sino", "mientras", "para", 
    "vacio", "caracter", "flotante", "doble", "estructura", 
    "romper", "continuar", "seleccionar", "caso", "defecto"
};
#define TOTAL_RES 16

int main() {
    // 1. Abrir un archivo fuente en C (usaremos este mismo archivo como prueba)
    FILE *archivo = fopen("ejercicio3.c", "r");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo fuente (ejercicio3.c).\n");
        return 1;
    }

    // Calcular el tamaño total del archivo en bytes
    fseek(archivo, 0, SEEK_END);
    long size = ftell(archivo);
    rewind(archivo);

    // 2. Cargar el programa en MEMORIA DINÁMICA
    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        printf("Error de asignacion de memoria dinamica.\n");
        fclose(archivo);
        return 1;
    }
    
    fread(buffer, 1, size, archivo);
    buffer[size] = '\0'; // Asegurar el fin de la cadena
    fclose(archivo);

    // Abrir el archivo de destino en la carpeta de resultados
    FILE *salida = fopen("../resultados/resultado_ejercicio3.txt", "w");
    if (!salida) {
        printf("Error: No se pudo crear el archivo de resultados.\n");
        free(buffer);
        return 1;
    }

    fprintf(salida, "--- TRADUCCION DE PALABRAS RESERVADAS ---\n");
    fprintf(salida, "Archivo analizado: ejercicio3.c\n");
    fprintf(salida, "Memoria dinamica asignada: %ld bytes\n\n", size);

    // 3. Analisis lexico (Separar el texto por tokens/palabras)
    // Se definen los caracteres que separan las palabras en C (espacios, saltos, llaves, etc.)
    char delimitadores[] = " \t\n\r(){}[];,+-*/=<>#\"&|!";
    char *token = strtok(buffer, delimitadores);

    int encontradas = 0;
    
    // Verificar cada palabra detectada contra nuestro diccionario
    while (token != NULL) {
        for (int i = 0; i < TOTAL_RES; i++) {
            if (strcmp(token, reservadasC[i]) == 0) {
                fprintf(salida, "Detectado: '%s' \t-> Traduccion: '%s'\n", token, reservadasESP[i]);
                encontradas++;
                break; 
            }
        }
        token = strtok(NULL, delimitadores); // Pasar a la siguiente palabra
    }

    fprintf(salida, "\nTotal de palabras reservadas identificadas y traducidas: %d\n", encontradas);

    // Liberar la memoria dinamica utilizada
    free(buffer);
    fclose(salida);
    
    printf("Analisis completado exitosamente. Revisa la carpeta resultados.\n");
    return 0;
}