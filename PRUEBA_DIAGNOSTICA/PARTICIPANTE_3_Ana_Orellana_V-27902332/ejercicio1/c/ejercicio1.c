#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Valida si el desplazamiento cumple la geometria en L del caballo (|dx*dy| == 2)
int es_movimiento_caballo_valido(int col1, int row1, int col2, int row2) {
    int dx = abs(col1 - col2);
    int dy = abs(row1 - row2);
    return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

// Retorna 0 si es CORRECTO, o el indice del movimiento (1-indexado) donde fallo
int validar_secuencia(const char *pos_inicial, const char *cadena_movs, char *error_msg, size_t err_size) {
    int curr_col = pos_inicial[0] - 'a';
    int curr_row = pos_inicial[1] - '1';

    char *copia = strdup(cadena_movs);
    char *token = strtok(copia, " \t\r\n");
    int mov_idx = 1;

    while (token != NULL) {
        // Validacion sintactica del token: debe empezar con 'K'
        if (token[0] != 'K') {
            snprintf(error_msg, err_size, "movimiento INCORRECTO: falla letra %c no pertenece a un caballo en el movimiento %d.", token[0], mov_idx);
            free(copia);
            return mov_idx;
        }
        if (strlen(token) < 3) {
            snprintf(error_msg, err_size, "INCORRECTO: formato invalido en el movimiento %d.", mov_idx);
            free(copia);
            return mov_idx;
        }

        char next_col_c = token[1];
        char next_row_c = token[2];

        // Validacion de rango en el tablero
        if (next_col_c < 'a' || next_col_c > 'h' || next_row_c < '1' || next_row_c > '8') {
            snprintf(error_msg, err_size, "INCORRECTO: posicion fuera de rango en el movimiento %d (%s).", mov_idx, token);
            free(copia);
            return mov_idx;
        }

        int next_col = next_col_c - 'a';
        int next_row = next_row_c - '1';

        // Validacion de regla geometrica de salto
        if (!es_movimiento_caballo_valido(curr_col, curr_row, next_col, next_row)) {
            snprintf(error_msg, err_size, "INCORRECTO falla en el movimiento %d.", mov_idx);
            free(copia);
            return mov_idx;
        }

        curr_col = next_col;
        curr_row = next_row;
        mov_idx++;
        token = strtok(NULL, " \t\r\n");
    }

    free(copia);
    snprintf(error_msg, err_size, "movimiento CORRECTO.");
    return 0;
}

int main() {
    const char *cadena_500 = "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8";
    const char *pos_ini = "c2";
    char err[256];

    // Intenta abrir en ../resultados/ (si se corre desde c/) o en resultados/ (si se corre desde ejercicio1/)
    FILE *fout = fopen("../resultados/resultado_ejercicio1.txt", "w");
    if (!fout) fout = fopen("resultados/resultado_ejercicio1.txt", "w");
    if (!fout) fout = fopen("resultado_ejercicio1.txt", "w");

    printf("=== PRUEBA DE CASOS DE EJEMPLO (C) ===\n");
    if (fout) fprintf(fout, "=== SALIDA DE EJECUCION EN C ===\n");

    const char *tests[][2] = {
        {"a1", "Kc2 Kd4 Kb3 Kc5"},
        {"a1", "Kc2 Kd4 Kb4 Kc5"},
        {"a2", "Kc3 Ke4 Kf6 Ke8 Kg7"},
        {"a2", "F Kc8 Ke4 Kf6 Ke8 Kg7"},
        {"a8", "Kb6 Kd7"},
        {"a8", "Kb6 Kb7"},
        {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"},
        {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"}
    };

    for (int t = 0; t < 8; t++) {
        validar_secuencia(tests[t][0], tests[t][1], err, sizeof(err));
        printf("Test %d: %s\n", t + 1, err);
        if (fout) fprintf(fout, "Test %d: %s\n", t + 1, err);
    }

    clock_t start = clock();
    for (int i = 0; i < 10; i++) {
        validar_secuencia(pos_ini, cadena_500, err, sizeof(err));
    }
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nResultado 500 movimientos: %s\n", err);
    printf("Tiempo total (10 verificaciones en C): %f segundos\n", cpu_time);

    if (fout) {
        fprintf(fout, "\nResultado 500 movimientos: %s\n", err);
        fprintf(fout, "Tiempo total (10 verificaciones en C): %f segundos\n\n", cpu_time);
        fclose(fout);
        printf("-> Archivo resultado_ejercicio1.txt generado correctamente.\n");
    }

    return 0;
}