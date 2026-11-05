#!/usr/bin/env python3
"""Detecta palabras reservadas de C y las traduce al espanol.

El archivo de entrada se carga completo en memoria. Se ignoran comentarios
(// y /* */), cadenas ("...") y caracteres ('...'). El reporte usa columnas
y lineas base 1.

Uso:
    python analizador.py <entrada.c> <reporte.txt> [traducido.c]
"""

from __future__ import annotations

import sys

PALABRAS = [
    ("auto", "Automático", "automático"),
    ("break", "Romper", "romper"),
    ("case", "Caso", "caso"),
    ("char", "Carácter", "carácter"),
    ("const", "Constante", "constante"),
    ("continue", "Continuar", "continuar"),
    ("default", "Defecto", "defecto"),
    ("do", "Hacer", "hacer"),
    ("double", "Doble", "doble"),
    ("else", "Sino", "sino"),
    ("enum", "Enumeración", "enumeración"),
    ("extern", "Externo", "externo"),
    ("float", "Flotante", "flotante"),
    ("for", "Para", "para"),
    ("goto", "Ir a", "ir a"),
    ("if", "Si", "si"),
    ("inline", "En línea", "en línea"),
    ("int", "Entero", "entero"),
    ("long", "Largo", "largo"),
    ("register", "Registro", "registro"),
    ("restrict", "Restringido", "restringido"),
    ("return", "Retornar", "retornar"),
    ("short", "Corto", "corto"),
    ("signed", "Con signo", "con signo"),
    ("sizeof", "Tamaño de", "tamaño de"),
    ("static", "Estático", "estático"),
    ("struct", "Estructura", "estructura"),
    ("switch", "Según", "según"),
    ("typedef", "Tipo definido", "tipo definido"),
    ("union", "Unión", "unión"),
    ("unsigned", "Sin signo", "sin signo"),
    ("void", "Vacío", "vacío"),
    ("volatile", "Volátil", "volátil"),
    ("while", "Mientras", "mientras"),
    ("_Bool", "Booleano", "booleano"),
    ("_Complex", "Complejo", "complejo"),
    ("_Imaginary", "Imaginario", "imaginario"),
    ("_Alignas", "Alinear como", "alinear como"),
    ("_Alignof", "Alineación de", "alineación de"),
    ("_Atomic", "Atómico", "atómico"),
    ("_Generic", "Genérico", "genérico"),
    ("_Noreturn", "Sin retorno", "sin retorno"),
    ("_Static_assert", "Afirmación estática", "afirmación estática"),
    ("_Thread_local", "Hilo local", "hilo local"),
]

TABLA = {nombre: (espanol, minusculas) for nombre, espanol, minusculas in PALABRAS}


def es_inicio_identificador(caracter: str) -> bool:
    return caracter.isalpha() or caracter == "_"


def es_parte_identificador(caracter: str) -> bool:
    return caracter.isalnum() or caracter == "_"


def avanzar_posicion(segmento: str, linea: int, columna: int):
    for caracter in segmento:
        if caracter == "\n":
            linea += 1
            columna = 1
        else:
            columna += 1
    return linea, columna


def analizar(texto: str):
    """Devuelve (texto_traducido, registros) con registros=(palabra, es, linea, columna)."""
    salida = []
    registros = []
    i = 0
    linea = 1
    columna = 1
    n = len(texto)

    while i < n:
        caracter = texto[i]

        if texto.startswith("//", i):
            fin = texto.find("\n", i)
            if fin == -1:
                fin = n
            segmento = texto[i:fin]
            salida.append(segmento)
            columna += len(segmento)
            i = fin
            continue

        if texto.startswith("/*", i):
            fin = texto.find("*/", i + 2)
            if fin == -1:
                fin = n
            else:
                fin += 2
            segmento = texto[i:fin]
            salida.append(segmento)
            linea, columna = avanzar_posicion(segmento, linea, columna)
            i = fin
            continue

        if caracter == '"' or caracter == "'":
            comilla = caracter
            j = i + 1
            while j < n:
                if texto[j] == "\\":
                    j += 2
                    continue
                if texto[j] == comilla:
                    j += 1
                    break
                j += 1
            segmento = texto[i:j]
            salida.append(segmento)
            linea, columna = avanzar_posicion(segmento, linea, columna)
            i = j
            continue

        if es_inicio_identificador(caracter):
            j = i + 1
            while j < n and es_parte_identificador(texto[j]):
                j += 1
            palabra = texto[i:j]
            if palabra in TABLA:
                espanol, minusculas = TABLA[palabra]
                registros.append((palabra, espanol, linea, columna))
                salida.append(minusculas)
            else:
                salida.append(palabra)
            columna += j - i
            i = j
            continue

        salida.append(caracter)
        if caracter == "\n":
            linea += 1
            columna = 1
        else:
            columna += 1
        i += 1

    return "".join(salida), registros


def main(argv) -> int:
    if len(argv) < 3:
        print(
            "Uso: analizador.py <entrada.c> <reporte.txt> [traducido.c]",
            file=sys.stderr,
        )
        return 2

    entrada = argv[1]
    reporte = argv[2]
    traducido = argv[3] if len(argv) >= 4 else None

    with open(entrada, "r", encoding="utf-8", newline="") as archivo:
        texto = archivo.read()

    texto_traducido, registros = analizar(texto)

    with open(reporte, "w", encoding="utf-8", newline="\n") as archivo:
        for palabra, espanol, linea, columna in registros:
            archivo.write(f"{palabra} -> {espanol}, linea {linea}, columna {columna}\n")

    if traducido is not None:
        with open(traducido, "w", encoding="utf-8", newline="\n") as archivo:
            archivo.write(texto_traducido)

    print(f"Encontradas: {len(registros)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
