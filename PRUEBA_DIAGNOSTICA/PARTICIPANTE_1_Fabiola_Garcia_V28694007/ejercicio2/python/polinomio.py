#!/usr/bin/env python3
"""Ejercicio 2 - Coeficientes de (x+1)^n con memoria dinamica.

Participante 1: Fabiola Garcia (V28694007) - UNEG 2026-II.

a) Genera los coeficientes del polinomio (x+1)^n con el triangulo de
   Pascal usando listas de Python (estructura dinamica que crece con
   append) y muestra el polinomio.
b) Para un x dado, muestra paso a paso el calculo de f(x) = (x+1)^n
   con los coeficientes generados.

Modos:
  polinomio.py demo              # n=5, x=2 (paso a paso)
  polinomio.py poly <n>           # imprime el polinomio (x+1)^n
  polinomio.py tiempo [n]         # mide la generacion (por defecto n=100)
"""
import sys
import time


def pascal(n):
    """Devuelve la fila n del triangulo de Pascal: [C(n,0), ..., C(n,n)].

    Equivalente en Python a la memoria dinámica en C:
    - Las listas de Python crecen y encogen solas (como realloc): aquí
      `nueva = [1]` crea una lista VACÍA con un 1 y `append` va
      agregando los coeficientes calculados. Cada append puede reservar
      más espacio interno: eso es "memoria dinámica" sin pedirla a mano.
    - La fila anterior (`fila`) se descarta en cada iteración; Python la
      libera automáticamente con la recolección de basura (equivalente
      al `free()` manual que hacemos en C).

    Identidad central del triángulo de Pascal (por qué este cálculo
    genera exactamente los coeficientes de (x+1)^n):

        C(k, i) = C(k-1, i-1) + C(k-1, i)

    Es decir, cada número es la suma de los dos que tiene encima.
    Empezamos con la fila 0 = [1] y por eso el primer y último elemento
    de cada fila nueva son 1 (los "bordes" del triángulo).

    Para n=100 obtenemos 101 coeficientes; el mayor es C(100,50),
    un número de 29 dígitos. En Python los enteros son de precisión
    arbitraria, así que no hay problema; en C sí lo es (ver polinomio.c).
    """
    fila = [1]  # fila 0: C(0,0) = 1
    for k in range(1, n + 1):
        nueva = [1]                     # C(k,0) = 1 (borde izquierdo)
        for i in range(1, k):
            # C(k,i) = C(k-1,i-1) + C(k-1,i): suma de los dos de arriba
            nueva.append(fila[i - 1] + fila[i])
        nueva.append(1)                 # C(k,k) = 1 (borde derecho)
        fila = nueva                    # la fila anterior se libera (colecta de basura)
    return fila


def mostrar_polinomio(n, coefs):
    """Imprime (x+1)^n = C(n,0)*x^n + ... + C(n,n)*x^0 (grado primero)."""
    partes = []
    for k in range(n, -1, -1):
        partes.append("%d*x^%d" % (coefs[k], k))
    print("(x+1)^%d = %s" % (n, " + ".join(partes)))


def evaluar_paso_a_paso(n, coefs, x):
    """Muestra el calculo de f(x) = sum C(n,k)*x^k acumulando termino a termino."""
    print("Calculo paso a paso de f(%d) = (%d+1)^%d con los coeficientes generados:"
          % (x, x, n))
    acumulado = 0
    for k in range(0, n + 1):
        potencia = x ** k
        termino = coefs[k] * potencia
        acumulado += termino
        print("  paso %d: C(%d,%d)*%d^%d = %d*%d = %d   acumulado = %d"
              % (k + 1, n, k, x, k, coefs[k], potencia, termino, acumulado))
    print("f(%d) = %d" % (x, acumulado))
    print("Verificacion: (%d+1)^%d = %d" % (x, n, (x + 1) ** n))
    return acumulado


def modo_demo():
    n, x = 5, 2
    coefs = pascal(n)
    mostrar_polinomio(n, coefs)
    print()
    evaluar_paso_a_paso(n, coefs, x)
    return 0


def modo_poly(n):
    mostrar_polinomio(n, pascal(n))
    return 0


def modo_tiempo(n):
    inicio = time.perf_counter()
    coefs = pascal(n)
    total = time.perf_counter() - inicio
    if len(coefs) != n + 1:
        print("ERROR: se generaron %d coeficientes (se esperaban %d)"
              % (len(coefs), n + 1))
        return 1
    print("python,%.6f,%d" % (total, n))
    return 0


def main(argv):
    if len(argv) >= 2 and argv[1] == "demo":
        return modo_demo()
    if len(argv) >= 3 and argv[1] == "poly":
        return modo_poly(int(argv[2]))
    if len(argv) >= 2 and argv[1] == "tiempo":
        n = int(argv[2]) if len(argv) >= 3 else 100
        return modo_tiempo(n)
    print("Uso: polinomio.py demo | poly <n> | tiempo [n]")
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
