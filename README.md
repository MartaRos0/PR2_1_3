# Grupo PR2_1_3
## Diseño de una solución de integración en el ámbito de una fábrica

Proyecto desarrollado para la simulación y gestión de una línea de producción de Lego automatizada utilizando **Python**, **ESP32**, **PostgreSQL** y **RoboDK**.

El sistema integra control de producción, comunicación con hardware, gestión de stock y persistencia de datos en una arquitectura modular orientada a automatización industrial.

---

# Estructura del proyecto

```bash
 proyecto/
│
├──  base_datos/      # Scripts SQL y gestión de base de datos
├──  esp/             # Código del microcontrolador ESP32
├──  python/          # Scripts principales de control y automatización
├──  web/             # Como extra se ha implementado una web
└── README.md
```

---

#  Tecnologías utilizadas

-  Python 3
-  PostgreSQL
-  Arduino
-  FreeRTOS
-  psycopg
-  RoboDK API
-  ESP32
-  SQL

---

#  Descripción del sistema

El proyecto simula una línea de producción de llenado de cajas de Lego automatizada capaz de:

- Gestionar pedidos
- Controlar el stock de componentes
- Registrar cajas fabricadas
- Detectar errores y paradas de emergencia
- Comunicarse con sensores y dispositivos externos
- Sincronizar datos entre simulación y base de datos

---

#  Contexto académico

Proyecto desarrollado dentro del ámbito de la automatización y robótica industrial, aplicando conceptos de:

- Bases de datos
- Programación
- Sistemas industriales
- Integración hardware/software
- Simulación robótica

---

## Autores
SoniaS. Galeas, Marcos Mérida, Lucía Remolar y Marta Ros  
Grupo PR2-1-3  
Grado en Informática Industrial y Robótica — UPV  
Curso 2026
