#include <stdio.h>

int main() {
    int contador = 0;
    float promedio = 0.0;
    char letra = 'a';

    // Comentario: if (contador == 0) return 1;
    /* Bloque de comentario
       while (contador < 10) continue;
    */
    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) {
            contador += i;
        } else {
            contador--;
        }
    }

    switch (contador) {
        case 0:
            printf("Cero\n");
            break;
        default:
            printf("Distinto de cero\n");
    }

    return 0;
}
