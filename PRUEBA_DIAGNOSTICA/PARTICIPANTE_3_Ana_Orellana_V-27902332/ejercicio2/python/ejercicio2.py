import os
import sys
import time

def generar_triangulo_pascal(n):
    fila = [1]
    for _ in range(n):
        fila = [1] + [fila[j] + fila[j + 1] for j in range(len(fila) - 1)] + [1]
    return fila

def formatear_polinomio(coefs, n):
    terminos = []
    for idx, c in enumerate(coefs):
        exp = n - idx
        if exp == 0:
            terminos.append(f"{c}")
        elif exp == 1:
            terminos.append(f"{c}x" if c != 1 else "x")
        else:
            terminos.append(f"{c}x^{exp}" if c != 1 else f"x^{exp}")
    return " + ".join(terminos)

def evaluar_paso_a_paso(coefs, n, x, lineas):
    enc = f"\n--- EVALUACION PASO A PASO PARA x = {x} (PYTHON) ---\nf({x}) = ({x} + 1)^{n} = ({x + 1})^{n}"
    print(enc)
    lineas.append(enc)
    total = 0
    for idx, c in enumerate(coefs):
        exp = n - idx
        potencia = x ** exp
        termino = c * potencia
        total += termino
        paso = f"  Termino grado {exp:2d}: {c} * ({x})^{exp} = {termino}"
        print(paso)
        lineas.append(paso)
    res_str = f"Resultado f({x}) = {total}"
    print(res_str)
    lineas.append(res_str)
    return total

if __name__ == "__main__":
    lineas_salida = ["\n--- SALIDA DE EJECUCION EN PYTHON ---"]
    print("=== DEMOSTRACION n=4 y EVALUACION CON x=2 (PYTHON) ===")
    n_demo = 4
    coefs_demo = generar_triangulo_pascal(n_demo)
    poli_str = f"(x + 1)^{n_demo} = {formatear_polinomio(coefs_demo, n_demo)}"
    print(poli_str)
    lineas_salida.append(poli_str)
    evaluar_paso_a_paso(coefs_demo, n_demo, 2, lineas_salida)

    print("\n=== MEDICION DE TIEMPO PARA n=100 (PYTHON) ===")
    t0 = time.perf_counter()
    coefs_100 = generar_triangulo_pascal(100)
    t1 = time.perf_counter()
    elapsed = t1 - t0

    res_100 = (
        f"\n=== MEDICION DE TIEMPO PARA n=100 (PYTHON) ===\n"
        f"Coeficientes generados: {len(coefs_100)} terminos.\n"
        f"Primer termino (coef x^100): {coefs_100[0]}\n"
        f"Termino central (coef x^50): {coefs_100[50]}\n"
        f"Ultimo termino: {coefs_100[-1]}\n"
        f"Tiempo de generacion en Python (n=100): {elapsed:.8f} segundos\n"
    )
    print(res_100)
    lineas_salida.append(res_100)

    # Localiza siempre la carpeta ejercicio2/resultados a partir de la ubicacion de este archivo .py
    carpeta_ejercicio2 = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    carpeta_resultados = os.path.join(carpeta_ejercicio2, "resultados")
    os.makedirs(carpeta_resultados, exist_ok=True)
    ruta_txt = os.path.join(carpeta_resultados, "resultado_ejercicio2.txt")

    with open(ruta_txt, "a", encoding="utf-8") as f:
        f.write("\n".join(lineas_salida) + "\n")

    print(f"-> Resultado de Python agregado exitosamente en: {ruta_txt}")