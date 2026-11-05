import os
import sys
import time

def es_movimiento_caballo_valido(c1, r1, c2, r2):
    dx = abs(c1 - c2)
    dy = abs(r1 - r2)
    return (dx == 1 and dy == 2) or (dx == 2 and dy == 1)

def validar_secuencia(pos_inicial, cadena_movimientos):
    curr_c = ord(pos_inicial[0].lower()) - ord('a')
    curr_r = int(pos_inicial[1]) - 1

    tokens = cadena_movimientos.strip().split()
    for idx, token in enumerate(tokens, start=1):
        if not token.startswith('K'):
            return False, f"movimiento INCORRECTO: falla letra {token[0]} no pertenece a un caballo en el movimiento {idx}."
        if len(token) < 3:
            return False, f"INCORRECTO: formato invalido en el movimiento {idx}."
        
        col_c = token[1].lower()
        row_c = token[2]

        if not ('a' <= col_c <= 'h' and '1' <= row_c <= '8'):
            return False, f"INCORRECTO: posicion fuera de tablero en el movimiento {idx} ({token})."
        
        nxt_c = ord(col_c) - ord('a')
        nxt_r = int(row_c) - 1

        if not es_movimiento_caballo_valido(curr_c, curr_r, nxt_c, nxt_r):
            return False, f"INCORRECTO falla en el movimiento {idx}."

        curr_c, curr_r = nxt_c, nxt_r

    return True, "movimiento CORRECTO."

if __name__ == "__main__":
    cadena_500 = "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8"
    pos_ini = "c2"

    print("=== PRUEBA DE CASOS DE EJEMPLO (PYTHON) ===")
    tests = [
        ("a1", "Kc2 Kd4 Kb3 Kc5"),
        ("a1", "Kc2 Kd4 Kb4 Kc5"),
        ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7"),
        ("a2", "F Kc8 Ke4 Kf6 Ke8 Kg7"),
        ("a8", "Kb6 Kd7"),
        ("a8", "Kb6 Kb7"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4")
    ]

    for p, c in tests:
        val, msg = validar_secuencia(p, c)
        print(f"Ak={p} | C={c[:25]}... -> {msg}")

    print("\n=== MEDICION DE TIEMPO (10 iteraciones de 500 movimientos) ===")
    start_t = time.perf_counter()
    for _ in range(10):
        ok, res = validar_secuencia(pos_ini, cadena_500)
    end_t = time.perf_counter()
    elapsed = end_t - start_t

    print(f"Resultado 500 movimientos: {res}")
    print(f"Tiempo total (10 verificaciones en Python): {elapsed:.6f} segundos")

    # Ubica siempre la carpeta ejercicio1/resultados a partir de la ubicacion de este archivo .py
    carpeta_ejercicio1 = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    carpeta_resultados = os.path.join(carpeta_ejercicio1, "resultados")
    os.makedirs(carpeta_resultados, exist_ok=True)
    ruta_txt = os.path.join(carpeta_resultados, "resultado_ejercicio1.txt")

    with open(ruta_txt, "a", encoding="utf-8") as f:
        f.write("\n--- SALIDA DE EJECUCION EN PYTHON ---\n")
        for p, c in tests:
            _, msg = validar_secuencia(p, c)
            f.write(f"Ak={p} | C={c[:25]}... -> {msg}\n")
        f.write(f"\nResultado 500 movimientos: {res}\n")
        f.write(f"Tiempo total (10 verificaciones en Python): {elapsed:.6f} segundos\n")

    print(f"-> Resultados de Python agregados en: {ruta_txt}")