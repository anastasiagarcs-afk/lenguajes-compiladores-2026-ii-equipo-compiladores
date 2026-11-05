#!/usr/bin/env python3
"""Coeficientes de (x+1)^n con el triangulo de Pascal y memoria dinamica.

Uso:
    python polinomio.py demo
    python polinomio.py poly <n>
    python polinomio.py eval <n> <x>
    python polinomio.py tiempo <n> [repeticiones]

En Python la memoria dinamica se representa con listas que crecen mediante
append/comprension: cada fila nueva se construye a partir de la anterior.
"""

from __future__ import annotations

import sys
import time

DEMO_N = 5
DEMO_X = 2


def generar_pascal(n: int) -> list[int]:
    """Devuelve los coeficientes C(n, 0..n) usando el triangulo de Pascal."""
    fila = [1]
    for _ in range(n):
        nueva = [1] * (len(fila) + 1)
        for i in range(1, len(fila)):
            nueva[i] = fila[i - 1] + fila[i]
        fila = nueva
    return fila


def formato_polinomio(coeficientes) -> str:
    """Formatea el polinomio en orden descendente de potencias."""
    n = len(coeficientes) - 1
    terminos = []
    for i, coeficiente in enumerate(coeficientes):
        potencia = n - i
        if coeficiente == 0:
            continue
        if potencia == 0:
            termino = str(coeficiente)
        elif potencia == 1:
            termino = "x" if coeficiente == 1 else f"{coeficiente}x"
        else:
            termino = f"x^{potencia}" if coeficiente == 1 else f"{coeficiente}x^{potencia}"
        terminos.append(termino)
    return " + ".join(terminos) if terminos else "0"


def evaluar_pasos(coeficientes, x: int) -> None:
    """Muestra el calculo paso a paso de f(x) con el polinomio generado."""
    n = len(coeficientes) - 1
    print(f"Evaluacion paso a paso para x = {x}:")
    acumulado = 0
    for i, coeficiente in enumerate(coeficientes):
        potencia = n - i
        valor = coeficiente * (x ** potencia)
        acumulado += valor
        if potencia == 0:
            print(f"Paso {i + 1}: {coeficiente} * 1 = {valor}; acumulado = {acumulado}")
        else:
            print(
                f"Paso {i + 1}: {coeficiente} * {x}^{potencia} = {valor}; "
                f"acumulado = {acumulado}"
            )
    print(f"Suma acumulada: {acumulado}")
    print(f"f({x}) = {acumulado}")


def mostrar(n: int, x: int) -> None:
    coeficientes = generar_pascal(n)
    print(f"Coeficientes de (x+1)^{n}:")
    print(", ".join(str(c) for c in coeficientes))
    print(f"Polinomio (x+1)^{n}:")
    print(formato_polinomio(coeficientes))
    print()
    evaluar_pasos(coeficientes, x)


def modo_demo() -> int:
    print(f"== Demostracion n={DEMO_N}, x={DEMO_X} ==")
    mostrar(DEMO_N, DEMO_X)
    return 0


def modo_poly(n: int) -> int:
    coeficientes = generar_pascal(n)
    print(f"Coeficientes de (x+1)^{n}:")
    print(", ".join(str(c) for c in coeficientes))
    print(f"Polinomio (x+1)^{n}:")
    print(formato_polinomio(coeficientes))
    return 0


def modo_eval(n: int, x: int) -> int:
    mostrar(n, x)
    return 0


def modo_tiempo(n: int, repeticiones: int) -> int:
    inicio = time.perf_counter()
    for _ in range(repeticiones):
        generar_pascal(n)
    total = time.perf_counter() - inicio
    print(f"python,{total:.6f},{n},{repeticiones}")
    return 0


def leer_entero(texto: str, nombre: str) -> int:
    try:
        return int(texto)
    except ValueError as error:
        raise SystemExit(f"ERROR: {nombre} debe ser un entero") from error


def validar_no_negativo(valor: int, nombre: str) -> int:
    if valor < 0:
        raise SystemExit(f"ERROR: {nombre} debe ser un entero no negativo")
    return valor


def main(argv) -> int:
    if len(argv) < 2:
        print(
            "Uso: polinomio.py {demo|poly|eval|tiempo} ...\n"
            "  demo\n"
            "  poly <n>\n"
            "  eval <n> <x>\n"
            "  tiempo <n> [repeticiones]",
            file=sys.stderr,
        )
        return 2

    modo = argv[1]
    if modo == "demo":
        return modo_demo()
    if modo == "poly":
        if len(argv) < 3:
            print("Uso: polinomio.py poly <n>", file=sys.stderr)
            return 2
        return modo_poly(validar_no_negativo(leer_entero(argv[2], "n"), "n"))
    if modo == "eval":
        if len(argv) < 4:
            print("Uso: polinomio.py eval <n> <x>", file=sys.stderr)
            return 2
        n = validar_no_negativo(leer_entero(argv[2], "n"), "n")
        x = validar_no_negativo(leer_entero(argv[3], "x"), "x")
        return modo_eval(n, x)
    if modo == "tiempo":
        if len(argv) < 3:
            print("Uso: polinomio.py tiempo <n> [repeticiones]", file=sys.stderr)
            return 2
        repeticiones = leer_entero(argv[3], "repeticiones") if len(argv) >= 4 else 10
        if repeticiones <= 0:
            print("ERROR: las repeticiones deben ser mayores que cero", file=sys.stderr)
            return 2
        n = validar_no_negativo(leer_entero(argv[2], "n"), "n")
        return modo_tiempo(n, repeticiones)

    print(f"ERROR: modo desconocido: {modo}", file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
