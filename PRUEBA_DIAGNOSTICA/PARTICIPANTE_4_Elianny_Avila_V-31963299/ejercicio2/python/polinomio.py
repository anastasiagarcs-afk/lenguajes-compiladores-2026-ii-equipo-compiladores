import time
import os

def generar_polinomio(n):
    coefs = [1]
    for i in range(1, n + 1):
        nuevos = [1] * (i + 1)
        for j in range(1, i):
            nuevos[j] = coefs[j - 1] + coefs[j]
        coefs = nuevos
    return coefs

def formatear_polinomio(coefs):
    n = len(coefs) - 1
    terminos = []
    for i, c in enumerate(coefs):
        pot = n - i
        if pot == 0:
            terminos.append(str(c))
        elif pot == 1:
            terminos.append(f"{c}x" if c != 1 else "x")
        else:
            terminos.append(f"{c}x^{pot}" if c != 1 else f"x^{pot}")
    return " + ".join(terminos)

def evaluar_pasos(coefs, x):
    n = len(coefs) - 1
    total = 0
    print(f"\n--- Cálculo paso a paso para x = {x} ---")
    for i, c in enumerate(coefs):
        pot = n - i
        val = c * (x ** pot)
        total += val
        print(f"  {c} * ({x})^{pot} = {val}")
    print(f"  Total f({x}) = {total}")

if __name__ == "__main__":
    n = 100
    x = 2
    
    print(f"Generando polinomio para n={n} con memoria dinamica...")
    start = time.perf_counter()
    coefs = generar_polinomio(n)
    end = time.perf_counter()
    tiempo_gen = end - start

    print(f"Tiempo de generacion: {tiempo_gen:.6f} segundos")
    
    p_str = formatear_polinomio(coefs)
    print(f"\nPolinomio (resumido para n={n}):")
    print(f"{p_str[:150]} ... {p_str[-150:]}")

    evaluar_pasos(coefs, x)

    # --- ESTA ES LA PARTE NUEVA QUE ARREGLA EL ERROR ---
    # Obtiene la ruta exacta donde está este archivo .py
    ruta_actual = os.path.dirname(os.path.abspath(__file__))
    carpeta_resultados = os.path.join(ruta_actual, "resultados")
    
    # Crea la carpeta por si no existe
    os.makedirs(carpeta_resultados, exist_ok=True)
    
    # Guarda el archivo en la ruta correcta
    ruta_txt = os.path.join(carpeta_resultados, "tiempos_python.txt")
    with open(ruta_txt, "w", encoding="utf-8") as f:
        f.write(f"Lenguaje: Python\nn: {n}\nTiempo de generacion: {tiempo_gen:.9f} s\n")
    print(f"\nArchivo guardado en: {ruta_txt}")