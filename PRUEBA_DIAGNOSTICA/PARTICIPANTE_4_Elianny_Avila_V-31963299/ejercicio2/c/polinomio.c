#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Estructura para números gigantes usando memoria dinámica
typedef struct {
    int *digitos;
    int tam;
    int capacidad;
} BigInt;

BigInt crear_bigint(int valor) {
    BigInt b;
    b.capacidad = 10;
    b.digitos = (int*)malloc(b.capacidad * sizeof(int));
    b.tam = 0;
    if (valor == 0) {
        b.digitos[0] = 0;
        b.tam = 1;
    } else {
        while (valor > 0) {
            b.digitos[b.tam++] = valor % 10;
            valor /= 10;
        }
    }
    return b;
}

BigInt clonar_bigint(BigInt original) {
    BigInt nuevo;
    nuevo.capacidad = original.capacidad;
    nuevo.tam = original.tam;
    nuevo.digitos = (int*)malloc(nuevo.capacidad * sizeof(int));
    for (int i = 0; i < original.tam; i++) {
        nuevo.digitos[i] = original.digitos[i];
    }
    return nuevo;
}

BigInt sumar_bigint(BigInt a, BigInt b) {
    BigInt res;
    int max_tam = (a.tam > b.tam) ? a.tam : b.tam;
    res.capacidad = max_tam + 1;
    res.digitos = (int*)malloc(res.capacidad * sizeof(int));
    res.tam = res.capacidad;
    
    int acarreo = 0;
    for (int i = 0; i < res.tam; i++) {
        int suma = acarreo;
        if (i < a.tam) suma += a.digitos[i];
        if (i < b.tam) suma += b.digitos[i];
        res.digitos[i] = suma % 10;
        acarreo = suma / 10;
    }
    if (res.digitos[res.tam - 1] == 0 && res.tam > 1) res.tam--;
    return res;
}

BigInt multiplicar_entero(BigInt a, int x) {
    BigInt res;
    res.capacidad = a.tam + 20; // Espacio extra por si crece
    res.digitos = (int*)malloc(res.capacidad * sizeof(int));
    res.tam = a.tam;
    
    int acarreo = 0;
    for (int i = 0; i < a.tam; i++) {
        long long prod = (long long)a.digitos[i] * x + acarreo;
        res.digitos[i] = prod % 10;
        acarreo = prod / 10;
    }
    while (acarreo > 0) {
        if (res.tam == res.capacidad) {
            res.capacidad *= 2;
            res.digitos = (int*)realloc(res.digitos, res.capacidad * sizeof(int));
        }
        res.digitos[res.tam++] = acarreo % 10;
        acarreo /= 10;
    }
    return res;
}

void imprimir_bigint(BigInt b) {
    for (int i = b.tam - 1; i >= 0; i--) {
        printf("%d", b.digitos[i]);
    }
}

void liberar_bigint(BigInt *b) {
    free(b->digitos);
}

int main() {
    int n = 100;
    int x = 2;

    printf("Generando polinomio (x+1)^%d...\n", n);
    clock_t inicio = clock();

    // Memoria dinámica para los coeficientes del polinomio
    BigInt *coefs = (BigInt*)malloc((n + 1) * sizeof(BigInt));
    coefs[0] = crear_bigint(1);

    for (int i = 1; i <= n; i++) {
        coefs[i] = crear_bigint(1);
        for (int j = i - 1; j > 0; j--) {
            BigInt suma = sumar_bigint(coefs[j - 1], coefs[j]);
            liberar_bigint(&coefs[j]);
            coefs[j] = suma;
        }
    }

    clock_t fin = clock();
    double tiempo_gen = (double)(fin - inicio) / CLOCKS_PER_SEC;

    printf("Tiempo de generacion en C: %f segundos\n", tiempo_gen);

    // Cálculo paso a paso para x
    printf("\n--- Calculo paso a paso para x = %d ---\n", x);
    BigInt total = crear_bigint(0);

    for (int i = 0; i <= n; i++) {
        int potencia = n - i;
        BigInt termino = clonar_bigint(coefs[i]);
        
        // Multiplicar el coeficiente por x^potencia
        for (int k = 0; k < potencia; k++) {
            BigInt temp = multiplicar_entero(termino, x);
            liberar_bigint(&termino);
            termino = temp;
        }

        // Imprimir solo los primeros y últimos términos para no saturar la pantalla
        if (i < 3 || i >= n - 2) {
            imprimir_bigint(coefs[i]);
            printf(" * (%d)^%d = ", x, potencia);
            imprimir_bigint(termino);
            printf("\n");
        } else if (i == 3) {
            printf("...\n");
        }

        // Sumar al total
        BigInt nuevo_total = sumar_bigint(total, termino);
        liberar_bigint(&total);
        liberar_bigint(&termino);
        total = nuevo_total;
    }

    printf("Total f(%d) = ", x);
    imprimir_bigint(total);
    printf("\n");

    // Guardar resultados en txt
    FILE *f = fopen("resultados/tiempos_c.txt", "w");
    if (f) {
        fprintf(f, "Lenguaje: C\nn: %d\nTiempo de generacion: %.9f s\n", n, tiempo_gen);
        fclose(f);
        printf("\nArchivo resultados/tiempos_c.txt generado.\n");
    } else {
        printf("\nError al crear el archivo. Asegurate de que la carpeta 'resultados' existe.\n");
    }

    // Liberar memoria
    for (int i = 0; i <= n; i++) {
        liberar_bigint(&coefs[i]);
    }
    free(coefs);
    liberar_bigint(&total);

    return 0;
}