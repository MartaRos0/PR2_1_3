# Gemelo Digital de Línea de Producción en RoboDK

Este repositorio contiene el software de control para un **Gemelo Digital (Digital Twin)** de una celda de fabricación y empaquetado automatizada. El proyecto está desarrollado utilizando **Python** y la API de **RoboDK**, integrando control robótico, simulación física, bases de datos relacionales y comunicaciones IoT en tiempo real.

## Scripts RoboDK

### Requisitos previos

| Software | Versión | Instalación |
|---|---|---|
| **RoboDK** | ≥ 5.6 | [robodk.com/download](https://robodk.com/download) |
| **Python** | 3.9 – 3.12 | Seleccionar *Add to PATH* |
| **paho-mqtt** | ≥ 2.0 | `pip install paho-mqtt` |
| **psycopg (v3)** | ≥ 3.1 | `pip install "psycopg[binary]"` |

> Si RoboDK usa su Python embebido (`C:\RoboDK\Python\`), instalar también allí:
> ```bash
> C:\RoboDK\Python\python.exe -m pip install paho-mqtt "psycopg[binary]"
> ```

---

### Lista de scripts

#### Procesos principales (paralelos permanentes)

| Script | Función |
|---|---|
| `Main_Produccion.py` | Orquestador principal. Gestiona la cinta, lanza subprocesos de robot y controla el ciclo de producción completo. **Punto de entrada del sistema.** |
| `DetectaSensor.py` | Vigía de fotocélulas. Detecta colisiones 3D entre las cajas y los sensores C, I, T. Escribe el **nombre real** de la caja detectada antes de escribir la señal (principio *datos antes que señal*). |
| `MqttListener.py` | Bridge bidireccional con el exterior. Mantiene dos clientes MQTT simultáneos: uno para el panel web (puerto 8084, WSS) y otro para el ESP32 (puerto 1883, TCP). |
| `Base_datos.py` | Registrador en PostgreSQL. Verifica el stock antes de arrancar, registra cada caja al pasar por sensor C y la finaliza (con tiempo) al pasar por sensor T. |

#### Subprocesos de robot (reactivos, lanzados por Main)

| Script | Función |
|---|---|
| `ROBOT1_BOLSA1.py` | Pick & Place de **bolsas Tipo 1** con el robot UR3e de cogida. Realiza 2 ciclos por caja (2 bolsas). |
| `ROBOT1_BOLSA2.py` | Pick & Place de **bolsas Tipo 2** con el robot UR3e de cogida. |
| `ROBOT2_TAPA1.py` | Pick & Place de **tapa Tipo 1** con el robot UR3e de tapas. |
| `ROBOT2_TAPA2.py` | Pick & Place de **tapa Tipo 2** con el robot UR3e de tapas. |

#### Subprocesos de apoyo (lanzados por los scripts de robot)

| Script | Función |
|---|---|
| `DISPENSAR_B1.py` | Avanza la cinta de suministro de bolsas Tipo 1 y señaliza disponibilidad (`BOLSA1_PICK='1'`). |
| `DISPENSAR_B2.py` | Ídem para bolsas Tipo 2 (`BOLSA2_PICK='1'`). |
| `REPONER_TAPA.py` | Clona visualmente una tapa Tipo 1 en el pedestal tras cada cogida. |
| `REPONER_TAPA_2.py` | Ídem para tapa Tipo 2. |
| `PONER_INSTRU.py` | Activa el mecanismo de caída de instrucciones y libera el semáforo `INSTRU_LIBRE`. |

---

### Parámetros de sincronización clave

Estos son los parámetros de RoboDK que coordinan el comportamiento entre scripts:

| Parámetro | Descripción | Escribe → Lee |
|---|---|---|
| `MQTT_ESTACION_ENCENDIDA` | Señal de arranque recibida por MQTT | MqttListener → Main |
| `PARAR_ESTACION` | `'1'` = activa, `'0'` = producción terminada | Main → Todos |
| `PARADA_EMERGENCIA` | E-Stop. `'1'` interrumpe todos los bucles | MqttListener / Base_datos → Todos |
| `ES_REANUDACION` | `'1'` = reanudar pedido tras emergencia | MqttListener → Main, Base_datos |
| `STOCK_VERIFICADO` | Autorización de arranque tras check de BD | Base_datos → Main |
| `R1_LIBRE` / `R2_LIBRE` / `INSTRU_LIBRE` | Semáforos de disponibilidad de robots | Main ↔ Scripts robot |
| `CONTADOR_C` / `CONTADOR_T` / `CONTADOR_I` | Nº de cajas que han pasado por cada sensor | Main → Base_datos |
| `NOMBRE_T_{i:03d}` | Nombre RoboDK congelado de la caja en evento T número `i` | Main → Base_datos |
| `PEDIDO_1_INICIAL` / `PEDIDO_2_INICIAL` | Total fijo del pedido (no varía durante producción) | Main init() → Main bucle |
| `SenyalSensor_C` / `T` / `I` | Estado del sensor (`'0'`/`'1'`) | DetectaSensor → Main |

> ⚠️ **Orden de escritura:** `NOMBRE_T_{i}` **siempre** debe escribirse **antes** que `CONTADOR_T`. Invertir el orden provoca que `Base_datos` lea un nombre vacío y no finalice ninguna caja.

---

### Cómo arrancar el sistema

1. Abrir el proyecto `.rdk` en RoboDK.
2. Verificar que todos los scripts están cargados con su nombre exacto.
3. Ejecutar **únicamente** `Main_Produccion.py` (F5 o doble clic → *Run Script*).
4. El script lanzará automáticamente `MqttListener` y `BASE_DATOS` como procesos paralelos.
5. El sistema quedará en espera de un pedido por MQTT.
