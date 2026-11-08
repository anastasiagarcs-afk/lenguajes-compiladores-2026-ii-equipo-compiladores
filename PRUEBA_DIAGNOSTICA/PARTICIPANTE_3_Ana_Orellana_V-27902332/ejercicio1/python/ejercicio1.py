import time
import os

def validar_movimientos(pos_inicial, n_esperado, cadena_movimientos):
    columnas = 'abcdefgh'
    if len(pos_inicial) != 2 or pos_inicial[0] not in columnas or not pos_inicial[1].isdigit():
        return "INCORRECTO posición inicial inválida"
    
    col_actual = columnas.find(pos_inicial[0])
    fila_actual = int(pos_inicial[1])
    movimientos = cadena_movimientos.split()
    
    for i, mov in enumerate(movimientos):
        if mov[0] != 'K':
            return f"INCORRECTO falla letra {mov[0]} no pertenece a un caballo."
        if len(mov) != 3 or mov[1] not in columnas or not mov[2].isdigit():
            return f"INCORRECTO falla en el movimiento {i+1}"
            
        col_nueva = columnas.find(mov[1])
        fila_nueva = int(mov[2])
        
        dx = abs(col_nueva - col_actual)
        dy = abs(fila_nueva - fila_actual)
        
        if (dx == 1 and dy == 2) or (dx == 2 and dy == 1):
            col_actual = col_nueva
            fila_actual = fila_nueva
        else:
            return f"INCORRECTO falla en el movimiento {i+1}"
            
    return "movimiento CORRECTO."

def main():
    # Ruta relativa para el archivo de resultados
    ruta_resultado = os.path.join(os.path.dirname(__file__), '..', 'resultados', 'resultado_ejercicio1.txt')
    
    cadena_500 = "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4"
    
    inicio = time.perf_counter()
    for _ in range(10):
        validar_movimientos("a2", 500, cadena_500)
    fin = time.perf_counter()
    tiempo_total = fin - inicio

    with open(ruta_resultado, 'w', encoding='utf-8') as f:
        f.write("--- PRUEBA DE RENDIMIENTO EJERCICIO 1 ---\n")
        f.write(f"Posicion inicial: a2\n")
        f.write(f"Total de movimientos evaluados: 500 iterados 10 veces\n")
        f.write(f"Tiempo de validacion total: {tiempo_total:.6f} segundos\n")
        
    print("Ejecución finalizada. Archivo generado en carpeta resultados.")

if __name__ == "__main__":
    main()