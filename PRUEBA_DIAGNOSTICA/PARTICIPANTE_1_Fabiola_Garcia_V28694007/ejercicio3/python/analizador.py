#!/usr/bin/env python3
"""Ejercicio 3 - Deteccion de palabras reservadas de C con traduccion.

Participante 1: Fabiola Garcia (V28694007) - UNEG 2026-II.

Lee un programa C cargado en memoria, ignora comentarios (// y /* */)
y cadenas ("..." y '...'), detecta las palabras reservadas de la tabla
y escribe el reporte:

    palabra_original -> Traduccion, linea N, columna M

Como extra, escribe una copia del programa con las palabras reservadas
reemplazadas por su traduccion en espanol.

Uso: analizador.py <entrada.c> <reporte.txt> <programa_traducido.c>
"""
import sys

TABLA = {
    "int": "entero",
    "float": "flotante",
    "if": "si",
    "else": "sino",
    "while": "mientras",
    "for": "para",
    "return": "retornar",
    "void": "vacio",
    "char": "caracter",
    "double": "doble",
    "struct": "estructura",
    "break": "romper",
    "continue": "continuar",
    "switch": "segun",
    "case": "caso",
    "default": "defecto",
}


def es_inicio_id(c):
    return c.isalpha() or c == "_"


def es_cuerpo_id(c):
    return c.isalnum() or c == "_"


def capitalizar(palabra):
    return palabra[0].upper() + palabra[1:]


def analizar(texto):
    """Devuelve (reporte, texto_traducido).

    reporte: lista de (palabra, linea, columna) 1-based en orden de
    aparicion. Solo se considera codigo (fuera de comentarios/cadenas).
    """
    reporte = []
    salida = []
    n = len(texto)
    i = 0
    linea = 1
    col = 1
    estado = "code"  # code | linea | bloque | cadena | caracter

    while i < n:
        c = texto[i]
        if estado == "code":
            if c == "/" and i + 1 < n and texto[i + 1] == "/":
                estado = "linea"
                salida.append(c)
                i += 1
                col += 1
            elif c == "/" and i + 1 < n and texto[i + 1] == "*":
                estado = "bloque"
                salida.append(c)
                i += 1
                col += 1
            elif c == '"':
                estado = "cadena"
                salida.append(c)
                i += 1
                col += 1
            elif c == "'":
                estado = "caracter"
                salida.append(c)
                i += 1
                col += 1
            elif es_inicio_id(c):
                j = i
                while j < n and es_cuerpo_id(texto[j]):
                    j += 1
                palabra = texto[i:j]
                if palabra in TABLA:
                    reporte.append((palabra, linea, col))
                    salida.append(TABLA[palabra])
                else:
                    salida.append(palabra)
                col += len(palabra)
                i = j
            else:
                salida.append(c)
                if c == "\n":
                    linea += 1
                    col = 1
                else:
                    col += 1
                i += 1
        elif estado == "linea":
            salida.append(c)
            i += 1
            if c == "\n":
                estado = "code"
                linea += 1
                col = 1
            else:
                col += 1
        elif estado == "bloque":
            if c == "*" and i + 1 < n and texto[i + 1] == "/":
                salida.append("*")
                salida.append("/")
                i += 2
                col += 2
                estado = "code"
            else:
                salida.append(c)
                i += 1
                if c == "\n":
                    linea += 1
                    col = 1
                else:
                    col += 1
        else:  # cadena o caracter
            fin = '"' if estado == "cadena" else "'"
            if c == "\\" and i + 1 < n:
                salida.append(c)
                salida.append(texto[i + 1])
                if texto[i + 1] == "\n":
                    linea += 1
                    col = 1
                else:
                    col += 2
                i += 2
            elif c == fin:
                salida.append(c)
                i += 1
                col += 1
                estado = "code"
            else:
                salida.append(c)
                i += 1
                if c == "\n":
                    linea += 1
                    col = 1
                else:
                    col += 1
    return reporte, "".join(salida)


def main(argv):
    if len(argv) != 4:
        print("Uso: analizador.py <entrada.c> <reporte.txt> <programa_traducido.c>")
        return 2
    entrada, ruta_reporte, ruta_traducido = argv[1], argv[2], argv[3]

    with open(entrada, encoding="utf-8", errors="replace") as fh:
        texto = fh.read()

    reporte, traducido = analizar(texto)

    with open(ruta_reporte, "w", encoding="utf-8", newline="\n") as fh:
        for palabra, linea, col in reporte:
            trad = capitalizar(TABLA[palabra])
            fh.write("%s -> %s, linea %d, columna %d\n"
                     % (palabra, trad, linea, col))

    with open(ruta_traducido, "w", encoding="utf-8", newline="\n") as fh:
        fh.write(traducido)

    print("OK: %d palabras reservadas encontradas (Python)" % len(reporte))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
