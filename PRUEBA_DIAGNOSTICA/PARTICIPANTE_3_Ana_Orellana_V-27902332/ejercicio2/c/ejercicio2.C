#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> // Para medir el tiempo

void calcularPascal(int n, int x, FILE *salida) {
    // Memoria dinámica para el triángulo
    long long **triangulo = (long long **)malloc((n + 1) * sizeof(long long *));
    for (int i = 0; i <= n; i++) {
        triangulo[i] = (long long *)malloc((i + 1) * sizeof(long long));
        triangulo[i][0] = 1;
        triangulo[i][i] = 1;
        for (int j = 1; j < i; j++) {
            triangulo[i][j] = triangulo[i - 1][j - 1] + triangulo[i - 1][j];
        }
    }

    fprintf(salida, "--- TRIANGULO DE PASCAL ---\n");
    // Imprimir el triángulo en formato piramidal (hasta n)
    for (int i = 0; i <= n; i++) {
        fprintf(salida, "(n=%d): ", i);
        // Espacios para simular la pirámide
        for (int esp = 0; esp < n - i; esp++) {
            fprintf(salida, "  ");
        }
        for (int j = 0; j <= i; j++) {
            fprintf(salida, "%4lld", triangulo[i][j]);
        }
        fprintf(salida, "\n");
    }
    fprintf(salida, "\n");

    // a) Generar el polinomio para el n solicitado
    fprintf(salida, "a) Polinomio generado:\n(x+1)^%d = ", n);
    for (int j = 0; j <= n; j++) {
        long long coef = triangulo[n][j];
        int exp = n - j;
        if (coef > 1 || exp == 0) fprintf(salida, "%lld", coef);
        if (exp > 0) fprintf(salida, "x");
        if (exp > 1) fprintf(salida, "^%d", exp);
        if (j < n) fprintf(salida, " + ");
    }
    fprintf(salida, "\n\n");

    // b) Evaluación paso a paso
    fprintf(salida, "b) Evaluacion paso a paso para x = %d:\n", x);
    long long resultadoFinal = 0;
    for (int j = 0; j <= n; j++) {
        long long coef = triangulo[n][j];
        int exp = n - j;
        long long valorTermino = coef * pow(x, exp);
        resultadoFinal += valorTermino;
        fprintf(salida, "Termino %d: %lld * (%d)^%d = %lld\n", j+1, coef, x, exp, valorTermino);
    }
    fprintf(salida, "Resultado Final f(%d) = %lld\n\n", x, resultadoFinal);

    // c) Prueba de rendimiento n = 100
    fprintf(salida, "--- RENDIMIENTO GENERACION n=100 (C) ---\n");
    clock_t inicio = clock();
    
    // Generar triángulo hasta n=100 (solo para medir tiempo, sin imprimir para no saturar memoria/archivo)
    int n_prueba = 100;
    long long **t_prueba = (long long **)malloc((n_prueba + 1) * sizeof(long long *));
    for (int i = 0; i <= n_prueba; i++) {
        t_prueba[i] = (long long *)malloc((i + 1) * sizeof(long long));
        t_prueba[i][0] = 1;
        t_prueba[i][i] = 1;
        for (int j = 1; j < i; j++) {
            // Nota: En C nativo ocurrirá desbordamiento (overflow) en números muy grandes, 
            // pero la asignación en memoria y cálculo se medirá correctamente.
            t_prueba[i][j] = t_prueba[i - 1][j - 1] + t_prueba[i - 1][j];
        }
    }
    
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    fprintf(salida, "Tiempo en C para generar n=100: %f segundos\n", tiempo);

    // Liberar memorias
    for (int i = 0; i <= n; i++) free(triangulo[i]);
    free(triangulo);
    for (int i = 0; i <= n_prueba; i++) free(t_prueba[i]);
    free(t_prueba);
}

int main(int argc, char *argv[]) {
    int n = 7; // Valor por defecto del pdf
    int x = 2; 
    
    if (argc >= 3) {
        n = atoi(argv[1]);
        x = atoi(argv[2]);
    }

    FILE *salida = fopen("../resultados/resultado_ejercicio2.txt", "w");
    if (!salida) {
        printf("Error: No se pudo crear el archivo de resultados.\n");
        return 1;
    }

    calcularPascal(n, x, salida);
    fclose(salida);
    printf("Ejecucion finalizada. Archivo con triangulo y tiempos generado.\n");
    return 0;
}