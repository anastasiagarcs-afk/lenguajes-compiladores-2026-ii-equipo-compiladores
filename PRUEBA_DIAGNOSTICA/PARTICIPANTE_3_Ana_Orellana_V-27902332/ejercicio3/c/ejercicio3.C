#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    const char *keyword;
    const char *traduccion;
} TraduccionKW;

static const TraduccionKW DICCIONARIO[] = {
    {"auto", "automatico"},     {"break", "romper"},
    {"case", "caso"},           {"char", "caracter"},
    {"const", "constante"},     {"continue", "continuar"},
    {"default", "defecto"},     {"do", "hacer"},
    {"double", "doble"},        {"else", "sino"},
    {"enum", "enumeracion"},    {"extern", "externo"},
    {"float", "flotante"},      {"for", "para"},
    {"goto", "ir_a"},           {"if", "si"},
    {"int", "entero"},          {"long", "largo"},
    {"register", "registro"},   {"return", "retornar"},
    {"short", "corto"},         {"signed", "con_signo"},
    {"sizeof", "tamano_de"},    {"static", "estatico"},
    {"struct", "estructura"},   {"switch", "segun"},
    {"typedef", "definir_tipo"},{"union", "union"},
    {"unsigned", "sin_signo"},  {"void", "vacio"},
    {"volatile", "volatil"},    {"while", "mientras"},
    {NULL, NULL}
};

const char* buscar_traduccion(const char *palabra) {
    for (int i = 0; DICCIONARIO[i].keyword != NULL; i++) {
        if (strcmp(DICCIONARIO[i].keyword, palabra) == 0) {
            return DICCIONARIO[i].traduccion;
        }
    }
    return NULL;
}

void asegurar_ejemplo_entrada(const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) {
        // Si no existe, lo crea automaticamente
        f = fopen(ruta, "w");
        if (f) {
            fprintf(f,
                "#include <stdio.h>\n\n"
                "int main() {\n"
                "    int contador = 0;\n"
                "    float promedio = 0.0;\n"
                "    char letra = 'a';\n\n"
                "    // Comentario: if (contador == 0) return 1;\n"
                "    /* Bloque de comentario\n"
                "       while (contador < 10) continue;\n"
                "    */\n"
                "    for (int i = 0; i < 5; i++) {\n"
                "        if (i %% 2 == 0) {\n"
                "            contador += i;\n"
                "        } else {\n"
                "            contador--;\n"
                "        }\n"
                "    }\n\n"
                "    switch (contador) {\n"
                "        case 0:\n"
                "            printf(\"Cero\\n\");\n"
                "            break;\n"
                "        default:\n"
                "            printf(\"Distinto de cero\\n\");\n"
                "    }\n\n"
                "    return 0;\n"
                "}\n"
            );
            fclose(f);
        }
    } else {
        fclose(f);
    }
}

char* cargar_archivo_dinamico(const char *ruta, size_t *tamano) {
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = (char*)malloc((len + 1) * sizeof(char));
    if (!buffer) {
        fclose(f);
        return NULL;
    }
    size_t leidos = fread(buffer, 1, len, f);
    buffer[leidos] = '\0';
    *tamano = leidos;
    fclose(f);
    return buffer;
}

char* traducir_codigo(const char *fuente) {
    size_t cap = strlen(fuente) * 2 + 1024;
    char *salida = (char*)malloc(cap);
    size_t out_idx = 0;

    int i = 0;
    int len = strlen(fuente);

    while (i < len) {
        if (fuente[i] == '/' && i + 1 < len && fuente[i+1] == '/') {
            while (i < len && fuente[i] != '\n') {
                salida[out_idx++] = fuente[i++];
            }
            continue;
        }
        if (fuente[i] == '/' && i + 1 < len && fuente[i+1] == '*') {
            salida[out_idx++] = fuente[i++];
            salida[out_idx++] = fuente[i++];
            while (i + 1 < len && !(fuente[i] == '*' && fuente[i+1] == '/')) {
                salida[out_idx++] = fuente[i++];
            }
            if (i < len) salida[out_idx++] = fuente[i++];
            if (i < len) salida[out_idx++] = fuente[i++];
            continue;
        }
        if (fuente[i] == '"') {
            salida[out_idx++] = fuente[i++];
            while (i < len && fuente[i] != '"') {
                if (fuente[i] == '\\' && i + 1 < len) {
                    salida[out_idx++] = fuente[i++];
                }
                salida[out_idx++] = fuente[i++];
            }
            if (i < len) salida[out_idx++] = fuente[i++];
            continue;
        }
        if (fuente[i] == '\'') {
            salida[out_idx++] = fuente[i++];
            while (i < len && fuente[i] != '\'') {
                if (fuente[i] == '\\' && i + 1 < len) {
                    salida[out_idx++] = fuente[i++];
                }
                salida[out_idx++] = fuente[i++];
            }
            if (i < len) salida[out_idx++] = fuente[i++];
            continue;
        }

        if (isalpha((unsigned char)fuente[i]) || fuente[i] == '_') {
            char token[128];
            int t_idx = 0;
            while (i < len && (isalnum((unsigned char)fuente[i]) || fuente[i] == '_')) {
                if (t_idx < 127) token[t_idx++] = fuente[i];
                i++;
            }
            token[t_idx] = '\0';

            const char *trad = buscar_traduccion(token);
            const char *a_copiar = (trad != NULL) ? trad : token;
            size_t clen = strlen(a_copiar);

            memcpy(&salida[out_idx], a_copiar, clen);
            out_idx += clen;
            continue;
        }

        salida[out_idx++] = fuente[i++];
    }

    salida[out_idx] = '\0';
    return salida;
}

int main(int argc, char *argv[]) {
    // Intentamos asegurar y cargar ejemplo_entrada.c desde distintas rutas relativas posibles
    const char *rutas[] = {
        "ejemplo_entrada.c",
        "c/ejemplo_entrada.c",
        "../c/ejemplo_entrada.c"
    };

    char *codigo = NULL;
    size_t tam = 0;
    const char *ruta_usada = "ejemplo_entrada.c";

    for (int i = 0; i < 3; i++) {
        asegurar_ejemplo_entrada(rutas[i]);
        codigo = cargar_archivo_dinamico(rutas[i], &tam);
        if (codigo) {
            ruta_usada = rutas[i];
            break;
        }
    }

    if (!codigo) {
        printf("Error critico: no se pudo cargar ni crear ejemplo_entrada.c\n");
        return 1;
    }

    // Medicion de tiempo para n=100 iteraciones
    clock_t start = clock();
    char *codigo_traducido = NULL;
    for (int i = 0; i < 100; i++) {
        if (codigo_traducido) free(codigo_traducido);
        codigo_traducido = traducir_codigo(codigo);
    }
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("=== ARCHIVO FUENTE C CARGADO DINAMICAMENTE (%zu bytes desde %s) ===\n", tam, ruta_usada);
    printf("=== CODIGO TRADUCIDO AL ESPANOL (C) ===\n\n%s\n", codigo_traducido);
    printf("Tiempo de ejecucion en C (n=100 iteraciones): %f segundos\n", cpu_time);

    // Guardar exclusivamente en ../resultados/resultado_ejercicio3.txt (subiendo desde c/)
    FILE *fout = fopen("../resultados/resultado_ejercicio3.txt", "w");
    if (!fout) fout = fopen("resultados/resultado_ejercicio3.txt", "w");

    if (fout) {
        fprintf(fout, "======================================================================\n");
        fprintf(fout, "UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)\n");
        fprintf(fout, "LENGUAJES Y COMPILADORES - PERIODO LECTIVO 2026-II\n");
        fprintf(fout, "PRUEBA DIAGNOSTICA - EJERCICIO 3: TRADUCTOR DE PALABRAS RESERVADAS DE C\n");
        fprintf(fout, "Estudiante: Ana Orellana | Seccion: 01\n");
        fprintf(fout, "Grupo: Equipo de Compiladores 2026-II\n");
        fprintf(fout, "Eslogan: \"Comprender el codigo, construir soluciones.\"\n");
        fprintf(fout, "======================================================================\n\n");
        fprintf(fout, "--- SALIDA DE EJECUCION EN C (Memoria Dinamica) ---\n");
        fprintf(fout, "Archivo cargado: %zu bytes\n\n%s\n", tam, codigo_traducido);
        fprintf(fout, "Tiempo de ejecucion en C (n=100 iteraciones): %f segundos\n\n", cpu_time);
        fclose(fout);
        printf("-> Resultado de C guardado perfectamente en resultados/resultado_ejercicio3.txt\n");
    } else {
        printf("Aviso: No se pudo escribir en el archivo de resultados.\n");
    }

    free(codigo);
    free(codigo_traducido);
    return 0;
}