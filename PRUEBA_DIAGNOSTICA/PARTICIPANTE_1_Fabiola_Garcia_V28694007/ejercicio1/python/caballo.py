#!/usr/bin/env python3
"""Ejercicio 1 - Caballo de ajedrez (Prueba Diagnostica, UNEG 2026-II).

Participante 1: Fabiola Garcia (V28694007).

Dada una posicion inicial (ej. a1) y una cadena de movimientos separados
por espacio (ej. "Kc2 Kd4 Kb3 Kc5"), verifica que cada movimiento sea
correcto:
  1) comienza con 'K' (caballo);
  2) la casilla destino es valida (columna a-h, fila 1-8);
  3) el desplazamiento es un salto de caballo (2 en una direccion y 1 en
     la otra) respecto a la posicion anterior.
Se detiene en el primer movimiento incorrecto (indice 1-based).
"""
import sys
import time

COLUMNAS = "abcdefgh"


def parsear_casilla(txt):
    """Devuelve (columna, fila) 0-based o None si la casilla es invalida."""
    if len(txt) != 2:
        return None
    col, fil = txt[0].lower(), txt[1]
    if col not in COLUMNAS or fil not in "12345678":
        return None
    return COLUMNAS.index(col), int(fil) - 1


def validar(inicial, movimientos):
    """Valida la secuencia. Devuelve (es_correcto, mensaje)."""
    pos = parsear_casilla(inicial)
    if pos is None:
        return False, "INCORRECTO posicion inicial invalida: '%s'" % inicial
    for i, mov in enumerate(movimientos, start=1):
        if not mov or mov[0] != "K":
            letra = mov[0] if mov else "?"
            return False, ("INCORRECTO la letra '%s' del movimiento %d "
                           "no pertenece a un caballo" % (letra, i))
        destino = parsear_casilla(mov[1:])
        if destino is None:
            return False, ("INCORRECTO casilla invalida en el movimiento %d: '%s'"
                           % (i, mov))
        dx = abs(destino[0] - pos[0])
        dy = abs(destino[1] - pos[1])
        if not ((dx == 2 and dy == 1) or (dx == 1 and dy == 2)):
            return False, "INCORRECTO falla en el movimiento %d" % i
        pos = destino
    return True, "movimiento CORRECTO"


def leer_movimientos(ruta):
    """Lee un archivo con linea 'inicial X' y movimientos (ignora #)."""
    inicial = "a1"
    movimientos = []
    with open(ruta, encoding="utf-8") as fh:
        for linea in fh:
            linea = linea.strip()
            if not linea or linea.startswith("#"):
                continue
            if linea.lower().startswith("inicial"):
                partes = linea.split()
                if len(partes) >= 2:
                    inicial = partes[1]
                continue
            movimientos.extend(linea.split())
    return inicial, movimientos


def modo_pruebas(ruta):
    """Ejecuta los casos de prueba (formato: inicial;movimientos;esperado)."""
    total = fallas = 0
    with open(ruta, encoding="utf-8") as fh:
        for num, linea in enumerate(fh, start=1):
            linea = linea.strip()
            if not linea or linea.startswith("#"):
                continue
            partes = linea.split(";")
            if len(partes) != 3:
                print("SKIP linea %d: formato invalido" % num)
                continue
            inicial, movimientos, esperado = partes[0], partes[1].split(), partes[2]
            _, mensaje = validar(inicial, movimientos)
            total += 1
            if esperado in mensaje:
                print("OK: %s | %s -> %s" % (inicial, partes[1], mensaje))
            else:
                fallas += 1
                print("FAIL linea %d: esperado '%s', obtenido '%s'"
                      % (num, esperado, mensaje))
    print("Pruebas: %d ejecutadas, %d fallas" % (total, fallas))
    return 1 if fallas else 0


def modo_tiempo(ruta, repeticiones):
    """Mide el tiempo de verificar 'repeticiones' veces la lista completa."""
    inicial, movimientos = leer_movimientos(ruta)
    if len(movimientos) != 500:
        print("AVISO: %s contiene %d movimientos (se esperaban 500)"
              % (ruta, len(movimientos)), file=sys.stderr)
    inicio = time.perf_counter()
    for _ in range(repeticiones):
        validar(inicial, movimientos)
    total = time.perf_counter() - inicio
    print("python,%.6f,%d,%d" % (total, len(movimientos), repeticiones))
    return 0


def main(argv):
    if len(argv) >= 3 and argv[1] == "pruebas":
        return modo_pruebas(argv[2])
    if len(argv) >= 3 and argv[1] == "tiempo":
        reps = int(argv[3]) if len(argv) >= 4 else 10
        return modo_tiempo(argv[2], reps)
    if len(argv) >= 4 and argv[1] == "validar":
        _, mensaje = validar(argv[2], argv[3].split())
        print(mensaje)
        return 0 if mensaje == "movimiento CORRECTO" else 1
    print("Uso: caballo.py pruebas <archivo> | "
          "tiempo <archivo> [repeticiones] | "
          "validar <inicial> \"<movimientos>\"")
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
