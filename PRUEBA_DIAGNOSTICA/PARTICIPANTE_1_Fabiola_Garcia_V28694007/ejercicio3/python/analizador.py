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

    Analizador lexico con MAQUINA DE ESTADOS. Recorremos el archivo
    caracter por caracter y mantenemos un "estado" que dice en que
    contexto estamos:

        code     -> estamos en CODIGO: aqui si busamos palabras reservadas
        linea    -> comentario // hasta el fin de la linea: IGNORAR
        bloque   -> comentario /* ... */: IGNORAR
        cadena   -> texto entre comillas dobles "...": IGNORAR
        caracter -> texto entre comillas simples '...': IGNORAR

    La idea central: solo cuando el estado es "code" un identificador
    puede ser palabra reservada. Si entramos a un comentario o una
    cadena, cualquier palabra que aparezca ahi (ej. "if" dentro de un
    string) se COPIA tal cual al salida pero NO se agrega al reporte.

    Como detectamos la entrada a esos contextos?
      - "//" o "/*" vistos en estado code  -> cambio a linea/bloque
      - '"' o "'" vistos en estado code    -> cambio a cadena/caracter
      - '\n' en estado linea               -> vuelta a code
      - "*/" en estado bloque              -> vuelta a code
      - la misma comilla que abrio en cadena/caracter -> vuelta a code
    Dentro de cadenas tambien respetamos escapes: \\" no cierra la
    cadena (el siguiente caracter se consume en pareja).

    Dentro de code, cuando vemos el inicio de un identificador
    (letra o '_'), comemos todos los caracteres [A-Za-z0-9_] de una
    vez y preguntamos si la palabra completa esta en TABLA. Esto
    evita falsos positivos: "contador" contiene "for" pero NO es la
    palabra "for"; "ifx" tampoco es "if".

    linea y col son 1-based y se actualizan con cada caracter leido;
    col0/linea0 se guardan AL EMPEZAR el identificador para reportar
    la posicion exacta donde empieza la palabra.
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
            # --- Detectamos si EMPEZAMOS un comentario o una cadena ---
            # En ese momento cambiamos de estado: de aqui en adelante
            # no se buscara ninguna palabra reservada hasta volver a code.
            if c == "/" and i + 1 < n and texto[i + 1] == "/":
                estado = "linea"       # comentario de linea
                salida.append(c)
                i += 1
                col += 1
            elif c == "/" and i + 1 < n and texto[i + 1] == "*":
                estado = "bloque"      # comentario de bloque
                salida.append(c)
                i += 1
                col += 1
            elif c == '"':
                estado = "cadena"      # cadena de texto
                salida.append(c)
                i += 1
                col += 1
            elif c == "'":
                estado = "caracter"    # caracter entre comillas simples
                salida.append(c)
                i += 1
                col += 1
            elif es_inicio_id(c):
                # --- Identificador (posible palabra reservada) ---
                # Consumimos el identificador completo de una sola vez
                j = i
                while j < n and es_cuerpo_id(texto[j]):
                    j += 1
                palabra = texto[i:j]
                if palabra in TABLA:
                    # Solo aqui SI es reserva: la reportamos y la
                    # traducimos (en minuscula para el .c traducido).
                    reporte.append((palabra, linea, col))
                    salida.append(TABLA[palabra])
                else:
                    # Cualquier otro identificador (main, x, contador...)
                    # se copia intacto.
                    salida.append(palabra)
                col += len(palabra)
                i = j
            else:
                # Caracter normal de codigo: lo copiamos y avanzamos
                # contando lineas/columnas para el reporte.
                salida.append(c)
                if c == "\n":
                    linea += 1
                    col = 1
                else:
                    col += 1
                i += 1
        elif estado == "linea":
            # --- Comentario //: copiamos TODO hasta el salto de linea ---
            # (ahi volvemos a code: la linea siguiente si se analiza).
            salida.append(c)
            i += 1
            if c == "\n":
                estado = "code"
                linea += 1
                col = 1
            else:
                col += 1
        elif estado == "bloque":
            # --- Comentario /* ... */: copiamos hasta el cierre ---.
            # El cierre es el par "*/"; ojo: un '*' suelto NO cierra.
            if c == "*" and i + 1 < n and texto[i + 1] == "/":
                salida.append("*")
                salida.append("/")
                i += 2
                col += 2
                estado = "code"        # volvemos a analizar codigo
            else:
                salida.append(c)
                i += 1
                if c == "\n":
                    linea += 1
                    col = 1
                else:
                    col += 1
        else:
            # --- Cadena "..." o caracter '...' ---
            # Mientras estamos aqui, cualquier palabra (if, while...)
            # se copia al archivo traducido PERO NO se reporta: esa es
            # la razon de existir de los estados.
            fin = '"' if estado == "cadena" else "'"
            if c == "\\" and i + 1 < n:
                # Escape: \" o \' no cierran; consumimos la pareja
                # completa para no "desfasar" el analizador.
                salida.append(c)
                salida.append(texto[i + 1])
                if texto[i + 1] == "\n":
                    linea += 1
                    col = 1
                else:
                    col += 2
                i += 2
            elif c == fin:
                # Encontramos la comilla que ABRE: cerramos y volvemos
                # a code (las palabras volvieran a ser relevantes).
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
