# lenguajes-compiladores-2026-ii-equipo-compiladores

**Grupo:** Equipo de Compiladores 2026-II
**Eslogan:** "Comprender el código, construir soluciones."

## Datos del curso

| Campo | Detalle |
| --- | --- |
| Universidad | Universidad Nacional Experimental de Guayana (UNEG) |
| Asignatura | Lenguajes y Compiladores |
| Profesor | Msc. Félix Márquez |
| Correo del profesor | lyc.fmarquez@e.uneg.edu.ve |
| Período lectivo | 2026-II |
| Modalidad | Curso virtual |
| Repositorio | `https://github.com/anastasiagarcs-afk/lenguajes-compiladores-2026-ii-equipo-compiladores` |

## Integrantes del grupo

| # | Nombre | Cédula | Sección | Rol | Usuario de GitHub |
| --- | --- | --- | --- | --- | --- |
| 1 | Fabiola García | V28694007 | 1 | **Líder** (Participante 1) | [`anastasiagarcs-afk`](https://github.com/anastasiagarcs-afk) |
| 2 | Cesar Velasquez | V-26592592 | 3 | Participante 2 | `<!-- PENDIENTE: usuario de GitHub -->` |
| 3 | Ana Orellana | V-27902332 | 1 | Participante 3 | `<!-- PENDIENTE: usuario de GitHub -->` |
| 4 | Elianny Avila | V-31963299 | 3 | Participante 4 | `<!-- PENDIENTE: usuario de GitHub -->` |

> La líder formaliza todas las entregas vía correo al profesor.

## Estructura del repositorio

```text
.
├── README.md                 # Este archivo
├── .gitignore
├── docs/
│   ├── lineamientos_curso.md      # Resumen operativo de los lineamientos del profesor
│   ├── lineamientos_defensa.md    # Reglas del video de defensa
│   └── plantilla_informe_APA.md   # Plantilla de informe con normas APA
├── scripts/
│   ├── init_repo.sh               # Crea la estructura (idempotente)
│   └── verificar_estructura.sh    # Verifica que no falte nada
├── PRUEBA_DIAGNOSTICA/            # Producto: prueba diagnóstica (individual)
│   ├── README.md
│   ├── PARTICIPANTE_1_Fabiola_Garcia_V28694007/
│   ├── PARTICIPANTE_2_Cesar_Velasquez_V-26592592/
│   ├── PARTICIPANTE_3_Ana_Orellana_V-27902332/
│   └── PARTICIPANTE_4_Elianny_Avila_V-31963299/
│       └── <cada participante>
│           ├── enlace_drive.txt    # Enlace al video de defensa
│           ├── README.md
│           ├── ejercicio1/  {python, c, scripts, resultados}
│           ├── ejercicio2/  {python, c, scripts, resultados}
│           └── ejercicio3/  {python, c, scripts, resultados}
├── TEMA1/   {README.md, codigo/, informes/, evidencias/, enlace_drive.txt}
├── TEMA2/   {README.md, codigo/, informes/, evidencias/, enlace_drive.txt}
├── TEMA3/   {README.md, codigo/, informes/, evidencias/, enlace_drive.txt}
└── TEMA4/   {README.md, codigo/, informes/, evidencias/, enlace_drive.txt}
```

## Reglas de trabajo

1. **Todos son colaboradores** del repositorio: cada integrante hace `push` de su trabajo.
2. **Commits claros y frecuentes**: formato `tipo: descripción` (ej. `feat: ejercicio1 en Python`).
3. **No subir videos pesados**: los videos van a Google Drive y solo se pega el enlace en el `enlace_drive.txt` correspondiente.
4. **No subir credenciales**: nada de tokens, contraseñas, `.env`, llaves ni binarios pesados.
5. **No subir archivos de prompt**: los `.txt` con instrucciones para la IA nunca se commitean (están en `.gitignore`).
6. **Normas APA** para citas y referencias en todos los informes (ver `docs/plantilla_informe_APA.md`).
7. **Uso de IA permitido**, pero cada integrante debe **entender y defender** su código.

## Flujo de trabajo con Git

```bash
# 1. Clonar (una sola vez)
git clone https://github.com/anastasiagarcs-afk/lenguajes-compiladores-2026-ii-equipo-compiladores.git
cd lenguajes-compiladores-2026-ii-equipo-compiladores

# 2. Actualizar antes de trabajar
git pull origin main

# 3. Trabajar en tu carpeta (nada fuera de ella sin consultar a la líder)

# 4. Ver qué cambió
git status

# 5. Registrar el cambio
git add <tus-archivos>
git commit -m "feat: descripción clara del cambio"

# 6. Subir
git push origin main
```

Si se prefiere trabajar en rama:

```bash
git checkout -b tarea/nombre-corta
# ... trabajo y commits ...
git push -u origin tarea/nombre-corta
# y pedir la fusión (pull request o aviso a la líder)
git checkout main
```

## Sobre el uso de IA

Se permite usar asistentes de IA para apoyar el desarrollo. Sin embargo:

- Cada integrante debe **leer, entender y explicar** su propio código.
- El profesor puede pedir explicación en vivo durante la defensa.
- Copiar sin entender no es aceptado y se considera falta de integridad académica.

## Sobre los informes

- Formato **APA** (portada, introducción, desarrollo, conclusiones, referencias).
- Usar `docs/plantilla_informe_APA.md` como base.
- Citar correctamente las fuentes consultadas, incluidas las entrevistas o documentos del curso.

## Entregas

- El repositorio es **público** para que cualquiera pueda consultarlo.
- La líder (Fabiola García) es quien **formaliza las entregas por correo** al profesor:
  `lyc.fmarquez@e.uneg.edu.ve`
- Cada video de defensa se sube a Google Drive y se enlaza en la carpeta correspondiente.

## Placeholders pendientes por completar

- [ ] Usuario de GitHub del Participante 2 (Cesar Velasquez)
- [ ] Usuario de GitHub del Participante 3 (Ana Orellana)
- [ ] Usuario de GitHub del Participante 4 (Elianny Avila)
- [ ] Enlace de Drive del video de defensa — Participante 1 (`PRUEBA_DIAGNOSTICA/.../enlace_drive.txt`)
- [ ] Enlace de Drive del video de defensa — Participante 2
- [ ] Enlace de Drive del video de defensa — Participante 3
- [ ] Enlace de Drive del video de defensa — Participante 4
- [ ] Enlaces de Drive de TEMA1, TEMA2, TEMA3 y TEMA4

---

*Equipo de Compiladores 2026-II — UNEG — Período 2026-II*
