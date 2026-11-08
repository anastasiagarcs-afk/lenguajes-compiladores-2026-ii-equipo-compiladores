import sys
import time
import os

def generar_pascal(n):
    triangulo = []
    for i in range(n + 1):
        fila = [1] * (i + 1)
        for j in range(1, i):
            fila[j] = triangulo[i - 1][j - 1] + triangulo[i - 1][j]
        triangulo.append(fila)
    return triangulo

def evaluar_polinomio(triangulo, n, x, archivo):
    archivo.write(f"a) Polinomio generado (Python):\n(x+1)^{n} = ")
    terminos = []
    for j in range(n + 1):
        coef = triangulo[n][j]
        exp = n - j
        termino = ""
        if coef > 1 or exp == 0: termino += str(coef)
        if exp > 0: termino += "x"
        if exp > 1: termino += f"^{exp}"
        terminos.append(termino)
    archivo.write(" + ".join(terminos) + "\n\n")

    archivo.write(f"b) Evaluacion paso a paso para x = {x} (Python):\n")
    resultado_final = 0
    for j in range(n + 1):
        coef = triangulo[n][j]
        exp = n - j
        valor_termino = coef * (x ** exp)
        resultado_final += valor_termino
        archivo.write(f"Termino {j+1}: {coef} * ({x})^{exp} = {valor_termino}\n")
    archivo.write(f"Resultado Final f({x}) = {resultado_final}\n\n")

def medir_rendimiento(archivo):
    archivo.write("--- RENDIMIENTO GENERACION n=100 (PYTHON) ---\n")
    inicio = time.perf_counter()
    
    # Generar hasta n=100 para medir el tiempo
    _ = generar_pascal(100)
    
    fin = time.perf_counter()
    tiempo = fin - inicio
    archivo.write(f"Tiempo en Python para generar n=100: {tiempo:.6f} segundos\n")
    archivo.write("Nota: Python procesa nativamente enteros de precision arbitraria, evitando el overflow de C.\n\n")

def main():
    # Valores por defecto igual que en C
    n = 7
    x = 2
    if len(sys.argv) >= 3:
        n = int(sys.argv[1])
        x = int(sys.argv[2])

    # Construir la ruta relativa hacia la carpeta 'resultados'
    directorio_script = os.path.dirname(os.path.abspath(__file__))
    ruta_resultado = os.path.normpath(os.path.join(directorio_script, '..', 'resultados', 'resultado_ejercicio2.txt'))

    # Abrir el archivo en modo 'a' (append) para no sobreescribir el output de C
    with open(ruta_resultado, 'a', encoding='utf-8') as f:
        f.write("\n========================================\n")
        f.write("=== EJECUCION DESDE PYTHON (COMPARATIVA) ===\n")
        f.write("========================================\n\n")
        
        triangulo = generar_pascal(n)
        evaluar_polinomio(triangulo, n, x, f)
        medir_rendimiento(f)
        
    print("Ejecucion de Python finalizada. Resultados añadidos a la carpeta 'resultados'.")

if __name__ == '__main__':
    main()