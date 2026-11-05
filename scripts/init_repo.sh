#!/usr/bin/env bash
# init_repo.sh — Crea (o repara) la estructura de carpetas del repositorio.
# Idempotente: puede ejecutarse cuantas veces se quiera.
set -euo pipefail

RAIZ_ESPERADA="lenguajes-compiladores-2026-ii-equipo-compiladores"

# --- Verificación de carpeta raíz ---
if [ "$(basename "$PWD")" != "$RAIZ_ESPERADA" ]; then
  echo "ERROR: Estás en '$PWD'." >&2
  echo "Debes ejecutar este script desde la carpeta raíz '$RAIZ_ESPERADA'." >&2
  exit 1
fi

if [ ! -f ./README.md ] && [ ! -d ./docs ]; then
  echo "ERROR: no se reconoce la raíz del repositorio (faltan README.md y docs/)." >&2
  exit 1
fi

crear_carpeta() {
  local d="$1"
  if [ -d "$d" ]; then
    echo "EXISTE: $d"
  else
    mkdir -p "$d"
    echo "CREADA: $d"
  fi
}

# Crea la carpeta y su .gitkeep si la carpeta está vacía de contenido útil.
crear_carpeta_con_gitkeep() {
  local d="$1"
  crear_carpeta "$d"
  if [ ! -f "$d/.gitkeep" ]; then
    touch "$d/.gitkeep"
    echo "  + .gitkeep en $d"
  fi
}

echo "== Inicializando estructura en $PWD =="

# --- Documentación y utilidades ---
crear_carpeta "docs"
crear_carpeta "scripts"

# --- Prueba diagnóstica: 4 participantes x 3 ejercicios x 4 subcarpetas ---
PARTICIPANTES=(
  "PARTICIPANTE_1_Fabiola_Garcia_V28694007"
  "PARTICIPANTE_2_Cesar_Velasquez_V-26592592"
  "PARTICIPANTE_3_Ana_Orellana_V-27902332"
  "PARTICIPANTE_4_Elianny_Avila_V-31963299"
)
EJERCICIOS=(ejercicio1 ejercicio2 ejercicio3)
SUBCARPETAS=(python c scripts resultados)

crear_carpeta "PRUEBA_DIAGNOSTICA"
for p in "${PARTICIPANTES[@]}"; do
  crear_carpeta "PRUEBA_DIAGNOSTICA/$p"
  for e in "${EJERCICIOS[@]}"; do
    for s in "${SUBCARPETAS[@]}"; do
      crear_carpeta_con_gitkeep "PRUEBA_DIAGNOSTICA/$p/$e/$s"
    done
  done
done

# --- Temas ---
for t in TEMA1 TEMA2 TEMA3 TEMA4; do
  for s in codigo informes evidencias; do
    crear_carpeta_con_gitkeep "$t/$s"
  done
done

echo "== Estructura lista =="
exit 0
