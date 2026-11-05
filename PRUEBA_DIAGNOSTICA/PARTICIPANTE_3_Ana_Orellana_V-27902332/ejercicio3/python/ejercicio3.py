import os
import re
import sys
import time

DICCIONARIO_C = {
    "auto": "automatico", "break": "romper", "case": "caso", "char": "caracter",
    "const": "constante", "continue": "continuar", "default": "defecto", "do": "hacer",
    "double": "doble", "else": "sino", "enum": "enumeracion", "extern": "externo",
    "float": "flotante", "for": "para", "goto": "ir_a", "if": "si",
    "int": "entero", "long": "largo", "register": "registro", "return": "retornar",
    "short": "corto", "signed": "con_signo", "sizeof": "tamano_de", "static": "estatico",
    "struct": "estructura", "switch": "segun", "typedef": "definir_tipo", "union": "union",
    "unsigned": "sin_signo", "void": "vacio", "volatile": "volatil", "while": "mientras"
}

def traducir_palabras_reservadas(codigo_c: str) -> str:
    patron = re.compile(
        r'(?P<line_comment>//[^\n]*)'
        r'|(?P<block_comment>/\*[\s\S]*?\*/)'
        r'|(?P<string>"(?:\\.|[^"\\])*")'
        r'|(?P<char>\'(?:\\.|[^\'\\])*\')'
        r'|(?P<ident>\b[a-zA-Z_][a-zA-Z0-9_]*\b)'
    )

    def reemplazar(match):
        if match.group("ident"):
            token = match.group("ident")
            return DICCIONARIO_C.get(token, token)
        return match.group(0)

    return patron.sub(reemplazar, codigo_c)

def asegurar_archivo_entrada(ruta):
    if not os.path.exists(ruta):
        os.makedirs(os.path.dirname(ruta), exist_ok=True)
        with open(ruta, "w", encoding="utf-8") as f:
            f.write(
                "#include <stdio.h>\n\n"
                "int main() {\n"
                "    int contador = 0;\n"
                "    float promedio = 0.0;\n"
                "    char letra = 'a';\n\n"
                "    // Comentario: if (contador == 0) return 1;\n"
                "    /* Bloque de comentario\n"
                "       while (contador < 10) continue;\n"
                "    */\n"
                "    for (int i = 0; i < 5; i++) {\n"
                "        if (i % 2 == 0) {\n"
                "            contador += i;\n"
                "        } else {\n"
                "            contador--;\n"
                "        }\n"
                "    }\n\n"
                "    switch (contador) {\n"
                "        case 0:\n"
                "            printf(\"Cero\\n\");\n"
                "            break;\n"
                "        default:\n"
                "            printf(\"Distinto de cero\\n\");\n"
                "    }\n\n"
                "    return 0;\n"
                "}\n"
            )

if __name__ == "__main__":
    # Localiza la ruta absoluta de la carpeta ejercicio3
    carpeta_python = os.path.dirname(os.path.abspath(__file__))
    carpeta_ejercicio3 = os.path.dirname(carpeta_python)

    # El archivo ejemplo_entrada.c esta en ejercicio3/c/ejemplo_entrada.c
    ruta_entrada = os.path.join(carpeta_ejercicio3, "c", "ejemplo_entrada.c")
    asegurar_archivo_entrada(ruta_entrada)

    with open(ruta_entrada, "r", encoding="utf-8") as f:
        contenido = f.read()

    # Medición de tiempo para n=100 iteraciones
    t0 = time.perf_counter()
    for _ in range(100):
        traducido = traducir_palabras_reservadas(contenido)
    t1 = time.perf_counter()
    elapsed = t1 - t0

    print(f"=== ARCHIVO C CARGADO ({len(contenido)} caracteres) ===")
    print("=== CODIGO TRADUCIDO AL ESPANOL (PYTHON) ===\n")
    print(traducido)
    print(f"\nTiempo de ejecucion en Python (n=100 iteraciones): {elapsed:.6f} segundos")

    # Guardar en resultados/resultado_ejercicio3.txt
    carpeta_resultados = os.path.join(carpeta_ejercicio3, "resultados")
    os.makedirs(carpeta_resultados, exist_ok=True)
    ruta_txt = os.path.join(carpeta_resultados, "resultado_ejercicio3.txt")

    with open(ruta_txt, "a", encoding="utf-8") as f:
        f.write("\n--- SALIDA DE EJECUCION EN PYTHON ---\n")
        f.write(f"Archivo cargado: {len(contenido)} caracteres\n\n")
        f.write(traducido + "\n")
        f.write(f"Tiempo de ejecucion en Python (n=100 iteraciones): {elapsed:.6f} segundos\n")

    print(f"-> Resultado de Python agregado exitosamente en: {ruta_txt}")