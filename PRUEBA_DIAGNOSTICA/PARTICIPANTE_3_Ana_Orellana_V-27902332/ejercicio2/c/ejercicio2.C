#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* sumar_cadenas(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int max_len = (len_a > len_b ? len_a : len_b) + 2;
    char* res = (char*)malloc(max_len * sizeof(char));
    res[max_len - 1] = '\0';

    int i = len_a - 1;
    int j = len_b - 1;
    int k = max_len - 2;
    int acarreo = 0;

    while (i >= 0 || j >= 0 || acarreo) {
        int sum = acarreo;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        res[k--] = (sum % 10) + '0';
        acarreo = sum / 10;
    }

    char* final_str = strdup(&res[k + 1]);
    free(res);
    return final_str;
}

char** generar_fila_pascal(int n) {
    char** fila = (char**)malloc((n + 1) * sizeof(char*));
    fila[0] = strdup("1");

    for (int i = 1; i <= n; i++) {
        char** nueva_fila = (char**)malloc((i + 1) * sizeof(char*));
        nueva_fila[0] = strdup("1");
        nueva_fila[i] = strdup("1");
        for (int j = 1; j < i; j++) {
            nueva_fila[j] = sumar_cadenas(fila[j - 1], fila[j]);
        }
        for (int j = 0; j <= i - 1; j++) {
            free(fila[j]);
        }
        free(fila);
        fila = nueva_fila;
    }
    return fila;
}

void liberar_fila(char** fila, int n) {
    for (int i = 0; i <= n; i++) {
        free(fila[i]);
    }
    free(fila);
}

void mostrar_polinomio(char** coefs, int n, FILE *fout) {
    printf("(x + 1)^%d = ", n);
    if (fout) fprintf(fout, "(x + 1)^%d = ", n);
    for (int i = n; i >= 0; i--) {
        int exp = i;
        char* coef = coefs[n - i];

        if (exp == 0) {
            printf("%s", coef);
            if (fout) fprintf(fout, "%s", coef);
        } else if (exp == 1) {
            if (strcmp(coef, "1") == 0) {
                printf("x + ");
                if (fout) fprintf(fout, "x + ");
            } else {
                printf("%sx + ", coef);
                if (fout) fprintf(fout, "%sx + ", coef);
            }
        } else {
            if (strcmp(coef, "1") == 0) {
                printf("x^%d + ", exp);
                if (fout) fprintf(fout, "x^%d + ", exp);
            } else {
                printf("%sx^%d + ", coef, exp);
                if (fout) fprintf(fout, "%sx^%d + ", coef, exp);
            }
        }
    }
    printf("\n");
    if (fout) fprintf(fout, "\n");
}

void evaluar_paso_a_paso(char** coefs, int n, double x, FILE *fout) {
    printf("\n--- EVALUACION PASO A PASO PARA x = %.2f ---\n", x);
    printf("f(x) = (%.2f + 1)^%d = (%.2f)^%d\n", x, n, x + 1.0, n);
    if (fout) {
        fprintf(fout, "\n--- EVALUACION PASO A PASO PARA x = %.2f ---\n", x);
        fprintf(fout, "f(x) = (%.2f + 1)^%d = (%.2f)^%d\n", x, n, x + 1.0, n);
    }

    double total = 0.0;
    for (int i = n; i >= 0; i--) {
        int exp = i;
        double coef = atof(coefs[n - i]);
        double potencia = 1.0;
        for (int p = 0; p < exp; p++) potencia *= x;
        double termino = coef * potencia;
        total += termino;
        printf("  Termino grado %2d: %s * (%.2f)^%d = %.4f\n", exp, coefs[n - i], x, exp, termino);
        if (fout) fprintf(fout, "  Termino grado %2d: %s * (%.2f)^%d = %.4f\n", exp, coefs[n - i], x, exp, termino);
    }
    printf("TOTAL f(%.2f) = %.6f\n", x, total);
    if (fout) fprintf(fout, "TOTAL f(%.2f) = %.6f\n", x, total);
}

int main() {
    FILE *fout = fopen("resultados/resultado_ejercicio2.txt", "w");
    if (!fout) fout = fopen("../resultados/resultado_ejercicio2.txt", "w");
    if (!fout) fout = fopen("resultado_ejercicio2.txt", "w");

    if (fout) {
        fprintf(fout, "======================================================================\n");
        fprintf(fout, "UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)\n");
        fprintf(fout, "LENGUAJES Y COMPILADORES - PERIODO LECTIVO 2026-II\n");
        fprintf(fout, "PRUEBA DIAGNOSTICA - EJERCICIO 2: POLINOMIO (x+1)^n Y TRIANGULO DE PASCAL\n");
        fprintf(fout, "Estudiante: Ana Orellana | Seccion: 01\n");
        fprintf(fout, "Grupo: Equipo de Compiladores 2026-II\n");
        fprintf(fout, "Eslogan: \"Comprender el codigo, construir soluciones.\"\n");
        fprintf(fout, "======================================================================\n\n");
        fprintf(fout, "--- SALIDA DE EJECUCION EN C (Memoria Dinamica y BigInt) ---\n");
    }

    printf("=== DEMOSTRACION n=4 y EVALUACION CON x=2 (C) ===\n");
    int n_demo = 4;
    char** fila_demo = generar_fila_pascal(n_demo);
    mostrar_polinomio(fila_demo, n_demo, fout);
    evaluar_paso_a_paso(fila_demo, n_demo, 2.0, fout);
    liberar_fila(fila_demo, n_demo);

    printf("\n=== MEDICION DE TIEMPO PARA n=100 (C) ===\n");
    clock_t start = clock();
    int n_target = 100;
    char** fila_100 = generar_fila_pascal(n_target);
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Coeficientes generados: %d terminos.\n", n_target + 1);
    printf("Termino central (coef x^50): %s\n", fila_100[50]);
    printf("Tiempo de generacion en C (n=100): %f segundos\n", cpu_time);

    if (fout) {
        fprintf(fout, "\n=== MEDICION DE TIEMPO PARA n=100 (C) ===\n");
        fprintf(fout, "Coeficientes generados: %d terminos.\n", n_target + 1);
        fprintf(fout, "Primer termino (coef x^100): %s\n", fila_100[0]);
        fprintf(fout, "Termino central (coef x^50): %s\n", fila_100[50]);
        fprintf(fout, "Ultimo termino (independiente): %s\n", fila_100[100]);
        fprintf(fout, "Tiempo de generacion en C (n=100): %f segundos\n\n", cpu_time);
        fclose(fout);
        printf("-> Resultado de C guardado en resultado_ejercicio2.txt\n");
    }

    liberar_fila(fila_100, n_target);
    return 0;
}