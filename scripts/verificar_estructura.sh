#!/usr/bin/env bash
# verificar_estructura.sh — Verifica que existan los elementos clave.
# Imprime "OK: <ruta>" o "FALTA: <ruta>". Sale con 1 si falta algo.
set -uo pipefail

RAIZ_ESPERADA="lenguajes-compiladores-2026-ii-equipo-compiladores"
FALLOS=0

if [ "$(basename "$PWD")" != "$RAIZ_ESPERADA" ]; then
  echo "FALTA: ./ (estás en '$PWD'; debe ser '$RAIZ_ESPERADA')"
  exit 1
fi
echo "OK: ./ (carpeta raíz correcta)"

chequear() {
  local ruta="$1"
  if [ -e "$ruta" ]; then
    echo "OK: $ruta"
  else
    echo "FALTA: $ruta"
    FALLOS=$((FALLOS + 1))
  fi
}

# --- Archivos raíz ---
chequear "./README.md"
chequear "./.gitignore"

# --- docs ---
chequear "./docs/lineamientos_curso.md"
chequear "./docs/lineamientos_defensa.md"
chequear "./docs/plantilla_informe_APA.md"

# --- scripts ---
chequear "./scripts/init_repo.sh"
chequear "./scripts/verificar_estructura.sh"

# --- Prueba diagnóstica ---
PARTICIPANTES=(
  "PARTICIPANTE_1_Fabiola_Garcia_V28694007"
  "PARTICIPANTE_2_Cesar_Velasquez_V-26592592"
  "PARTICIPANTE_3_Ana_Orellana_V-27902332"
  "PARTICIPANTE_4_Elianny_Avila_V-31963299"
)
EJERCICIOS=(ejercicio1 ejercicio2 ejercicio3)
SUBCARPETAS=(python c scripts resultados)

chequear "./PRUEBA_DIAGNOSTICA"
chequear "./PRUEBA_DIAGNOSTICA/README.md"

for p in "${PARTICIPANTES[@]}"; do
  chequear "./PRUEBA_DIAGNOSTICA/$p"
  chequear "./PRUEBA_DIAGNOSTICA/$p/README.md"
  chequear "./PRUEBA_DIAGNOSTICA/$p/enlace_drive.txt"
  for e in "${EJERCICIOS[@]}"; do
    for s in "${SUBCARPETAS[@]}"; do
      chequear "./PRUEBA_DIAGNOSTICA/$p/$e/$s"
    done
  done
done

# --- Temas ---
for t in TEMA1 TEMA2 TEMA3 TEMA4; do
  chequear "./$t"
  chequear "./$t/README.md"
  chequear "./$t/enlace_drive.txt"
  for s in codigo informes evidencias; do
    chequear "./$t/$s"
  done
done

# --- Resultado ---
if [ "$FALLOS" -gt 0 ]; then
  echo "== RESULTADO: $FALLOS elemento(s) faltante(s) =="
  exit 1
fi
echo "== RESULTADO: estructura completa =="
exit 0
