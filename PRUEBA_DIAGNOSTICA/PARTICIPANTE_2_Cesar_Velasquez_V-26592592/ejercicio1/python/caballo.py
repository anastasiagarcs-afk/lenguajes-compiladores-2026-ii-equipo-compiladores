#!/usr/bin/env python3
"""Verificacion de movimientos de un caballo de ajedrez.

Uso:
    python caballo.py validar <inicial> "<movimientos>" [n]
    python caballo.py validar-archivo <archivo>
    python caballo.py pruebas <archivo>
    python caballo.py tiempo <archivo> [repeticiones]

El tablero se representa con columnas a-h y filas 1-8. Cada movimiento
debe tener la forma K<casa>, por ejemplo Kc2.
"""

from __future__ import annotations

import sys
import time


def casilla(texto: str):
    """Convierte 'c2' en (columna, fila) base 0, o None si es invalida."""
    if len(texto) != 2:
        return None
    columna, fila = texto[0], texto[1]
    if "a" <= columna <= "h" and "1" <= fila <= "8":
        return ord(columna) - ord("a"), int(fila) - 1
    return None


def salto_caballo(a, b) -> bool:
    dx = abs(a[0] - b[0])
    dy = abs(a[1] - b[1])
    return (dx == 1 and dy == 2) or (dx == 2 and dy == 1)


def validar(inicial: str, movimientos) -> tuple[bool, str]:
    """Valida la secuencia completa y se detiene en el primer error."""
    posicion = casilla(inicial)
    if posicion is None:
        return False, f"INCORRECTO posicion inicial invalida: {inicial}"

    for numero, movimiento in enumerate(movimientos, start=1):
        if not movimiento:
            continue
        if movimiento[0] != "K":
            return (
                False,
                "movimiento INCORRECTO falla letra "
                f"{movimiento[0]} no pertenece a un caballo",
            )
        if len(movimiento) != 3:
            return False, f"INCORRECTO falla en el movimiento {numero}"
        destino = casilla(movimiento[1:])
        if destino is None:
            return False, f"INCORRECTO falla en el movimiento {numero}"
        if not salto_caballo(posicion, destino):
            return False, f"INCORRECTO falla en el movimiento {numero}"
        posicion = destino

    return True, "movimiento CORRECTO"


def leer_movimientos(ruta: str):
    """Lee un archivo con 'inicial <casa>' y los movimientos separados por espacios."""
    inicial = "a2"
    movimientos = []
    with open(ruta, "r", encoding="utf-8") as archivo:
        for linea in archivo:
            linea = linea.strip()
            if not linea or linea.startswith("#"):
                continue
            partes = linea.split()
            if partes[0].lower() == "inicial" and len(partes) >= 2:
                inicial = partes[1]
                partes = partes[2:]
            movimientos.extend(partes)
    return inicial, movimientos


def modo_validar(args) -> int:
    if len(args) < 2:
        print("Uso: caballo.py validar <inicial> \"<movimientos>\" [n]", file=sys.stderr)
        return 2
    inicial = args[0]
    movimientos = args[1].split()
    if len(args) >= 3:
        try:
            esperados = int(args[2])
        except ValueError:
            print("ERROR: n debe ser un entero", file=sys.stderr)
            return 2
        if esperados != len(movimientos):
            print(
                f"AVISO: n={esperados} pero se recibieron {len(movimientos)} movimientos",
                file=sys.stderr,
            )
    correcto, mensaje = validar(inicial, movimientos)
    print(mensaje)
    return 0 if correcto else 1


def modo_validar_archivo(args) -> int:
    if len(args) != 1:
        print("Uso: caballo.py validar-archivo <archivo>", file=sys.stderr)
        return 2
    inicial, movimientos = leer_movimientos(args[0])
    correcto, mensaje = validar(inicial, movimientos)
    print(f"Movimientos: {len(movimientos)}")
    print(f"Resultado: {mensaje}")
    return 0 if correcto else 1


def modo_pruebas(args) -> int:
    if not args:
        print("Uso: caballo.py pruebas <archivo>", file=sys.stderr)
        return 2
    total = 0
    correctos = 0
    with open(args[0], "r", encoding="utf-8") as archivo:
        for linea in archivo:
            linea = linea.strip()
            if not linea or linea.startswith("#"):
                continue
            partes = linea.split(";", 2)
            if len(partes) != 3:
                print(f"AVISO: linea ignorada: {linea}", file=sys.stderr)
                continue
            inicial, movimientos_txt, esperado = (p.strip() for p in partes)
            total += 1
            _, mensaje = validar(inicial, movimientos_txt.split())
            if mensaje == esperado:
                correctos += 1
                print(f"CASO {total}: OK -> {mensaje}")
            else:
                print(
                    f"CASO {total}: ERROR -> esperado '{esperado}' obtenido '{mensaje}'"
                )
    print(f"RESULTADO: {correctos}/{total} OK")
    return 0 if total > 0 and correctos == total else 1


def modo_tiempo(args) -> int:
    if not args:
        print("Uso: caballo.py tiempo <archivo> [repeticiones]", file=sys.stderr)
        return 2
    ruta = args[0]
    repeticiones = int(args[1]) if len(args) >= 2 else 10
    if repeticiones <= 0:
        print("ERROR: las repeticiones deben ser mayores que cero", file=sys.stderr)
        return 2

    inicial, movimientos = leer_movimientos(ruta)
    correcto, mensaje = validar(inicial, movimientos)
    if not correcto:
        print(f"ERROR: {mensaje}", file=sys.stderr)
        return 1

    inicio = time.perf_counter()
    for _ in range(repeticiones):
        correcto, mensaje = validar(inicial, movimientos)
        if not correcto:
            print(f"ERROR: {mensaje}", file=sys.stderr)
            return 1
    total = time.perf_counter() - inicio

    print(f"python,{total:.6f},{len(movimientos)},{repeticiones}")
    return 0


def main(argv) -> int:
    if len(argv) < 2:
        print(
            "Uso: caballo.py {validar|validar-archivo|pruebas|tiempo} ...\n"
            "  validar <inicial> \"<movimientos>\" [n]\n"
            "  validar-archivo <archivo>\n"
            "  pruebas <archivo>\n"
            "  tiempo <archivo> [repeticiones]",
            file=sys.stderr,
        )
        return 2

    modo = argv[1]
    if modo == "validar":
        return modo_validar(argv[2:])
    if modo == "validar-archivo":
        return modo_validar_archivo(argv[2:])
    if modo == "pruebas":
        return modo_pruebas(argv[2:])
    if modo == "tiempo":
        return modo_tiempo(argv[2:])
    print(f"ERROR: modo desconocido: {modo}", file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
