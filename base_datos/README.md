## Base de Datos

El sistema utiliza **PostgreSQL** con una base de datos llamada **`gdi`**. No existe subesquema: todas las tablas se crean directamente en el schema `public` de `gdi`.

### Requisitos

| Software | Versión |
|---|---|
| **PostgreSQL** | ≥ 14 |
| **pgAdmin 4** | Última estable (recomendado para gestión) |

---

### Configuración de conexión en `Base_datos.py`

```python
DB_CONFIG = {
    "dbname":   "gdi",
    "user":     "postgres",
    "password": "postgres",   # cambiar si es necesario
    "host":     "localhost",
    "port":     "5432"
}
```

---

### Creación de la base de datos y tablas

Ejecutar el siguiente SQL en el **Query Tool de pgAdmin** sobre la base de datos `gdi`:

```sql
-- ─── TABLAS MAESTRAS ──────────────────────────────────────────

CREATE TABLE IF NOT EXISTS Cliente (
    DNI_CIF         VARCHAR(15)  PRIMARY KEY,
    Nombre          VARCHAR(100) NOT NULL,
    Direccion_Envio VARCHAR(200) NOT NULL
);

CREATE TABLE IF NOT EXISTS Tipo_Caja (
    Codigo              VARCHAR(20)   PRIMARY KEY,
    Stock_Actual        INTEGER,
    Stock_Minimo        INTEGER       NOT NULL,
    Precio_Fabricacion  NUMERIC(10,2) NOT NULL
);

CREATE TABLE IF NOT EXISTS Almacen (
    Codigo       VARCHAR(20) PRIMARY KEY,
    Stock_Actual INTEGER,
    Stock_Minimo INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS Componente (
    Codigo VARCHAR(20) PRIMARY KEY REFERENCES Almacen(Codigo) ON DELETE CASCADE,
    Nombre VARCHAR(100) NOT NULL
);

-- ─── TABLAS DE NEGOCIO ────────────────────────────────────────

CREATE TABLE IF NOT EXISTS Pedido (
    Numero_Localizador VARCHAR(20) PRIMARY KEY,
    Fecha_Pedido       DATE        NOT NULL,
    DNI_CIF            VARCHAR(15) REFERENCES Cliente(DNI_CIF)
);

CREATE TABLE IF NOT EXISTS Caja (
    Cod                VARCHAR(50)  PRIMARY KEY,  -- mínimo 50 para IDs tipo LEGO_PED_xxx_001
    Tipo_Atributo      VARCHAR(50),
    Acabada            BOOLEAN      NOT NULL DEFAULT FALSE,
    Numero_Localizador VARCHAR(20)  REFERENCES Pedido(Numero_Localizador) ON DELETE CASCADE,
    Codigo_Tipo        VARCHAR(20)  REFERENCES Tipo_Caja(Codigo),
    Tiempo_Produccion  NUMERIC(8,1)
);

-- ─── DATOS INICIALES ─────────────────────────────────────────

INSERT INTO Cliente VALUES
    ('12345678A', 'Carlos Martinez', 'Calle Mayor 12, Valencia'),
    ('87654321B', 'Laura Gomez',     'Av. Libertad 45, Madrid')
ON CONFLICT DO NOTHING;

INSERT INTO Tipo_Caja VALUES
    ('TC01', 80,  20, 14.99),   -- Caja Amarilla Tipo 1
    ('TC02', 120, 30, 19.99)    -- Caja Roja Tipo 2
ON CONFLICT DO NOTHING;

INSERT INTO Almacen VALUES
    ('C001', 120, 50),   -- Caja vacía
    ('C002', 300, 100),  -- Bolsa piezas A (Tipo 1)
    ('C003', 280, 90),   -- Bolsa piezas B (Tipo 2)
    ('C004', 150, 60),   -- Tapa
    ('C005', 200, 80)    -- Manual de instrucciones
ON CONFLICT DO NOTHING;

INSERT INTO Componente VALUES
    ('C001', 'Caja vacia'),
    ('C002', 'Bolsa piezas A'),
    ('C003', 'Bolsa piezas B'),
    ('C004', 'Tapa de caja'),
    ('C005', 'Manual instrucciones')
ON CONFLICT DO NOTHING;
```

---

### Estructura de las tablas
gdi (base de datos)
│
├── Cliente          → datos del cliente que realiza el pedido
├── Tipo_Caja        → tipos de producto (TC01 Amarilla, TC02 Roja)
├── Almacen          → stock actual y mínimo de cada componente
├── Componente       → descripción de cada componente (FK → Almacen)
├── Pedido           → registro de cada orden de producción (FK → Cliente)
└── Caja             → cada caja producida, con estado y tiempo (FK → Pedido, Tipo_Caja)

#### Tabla `Caja` — campos relevantes

| Campo | Tipo | Descripción |
|---|---|---|
| `Cod` | `VARCHAR(50)` | ID único generado por el sistema (ej: `LEGO_PED_1234_001`) |
| `Tipo_Atributo` | `VARCHAR(50)` | `'Caja Amarilla T1'` o `'Caja Roja T2'` |
| `Acabada` | `BOOLEAN` | `FALSE` mientras está en producción, `TRUE` al colocar la tapa |
| `Numero_Localizador` | `VARCHAR(20)` | FK al pedido al que pertenece |
| `Tiempo_Produccion` | `NUMERIC(8,1)` | Segundos desde sensor C hasta sensor T |

---

### Consultas útiles para verificar la producción

```sql
-- Estado de todas las cajas del último pedido
SELECT Cod, Tipo_Atributo, Acabada, Tiempo_Produccion
FROM Caja
ORDER BY Cod;

-- Stock actual de componentes
SELECT a.Codigo, c.Nombre, a.Stock_Actual, a.Stock_Minimo
FROM Almacen a
JOIN Componente c ON a.Codigo = c.Codigo;

-- Cajas no finalizadas (eliminadas manualmente o error)
SELECT * FROM Caja WHERE Acabada = FALSE;

-- Tiempo medio de producción por tipo
SELECT Tipo_Atributo,
       COUNT(*)                        AS total,
       ROUND(AVG(Tiempo_Produccion),1) AS tiempo_medio_s
FROM Caja
WHERE Acabada = TRUE
GROUP BY Tipo_Atributo;
```

---

### Solución de problemas frecuentes de BD

| Síntoma | Causa | Solución |
|---|---|---|
| `value too long for type character varying(20)` | Columna `Caja.Cod` demasiado corta | `ALTER TABLE Caja ALTER COLUMN Cod TYPE VARCHAR(50);` |
| `relation "almacen" does not exist` | Tablas no creadas | Ejecutar el SQL de creación completo |
| Cajas sin `Acabada=TRUE` | Principio datos-antes-señal violado en Main | Verificar que `NOMBRE_T_{i}` se escribe antes que `CONTADOR_T` |
| `ModuleNotFoundError: psycopg` | Instalada versión 2 en lugar de 3 | `pip uninstall psycopg2 && pip install "psycopg[binary]"` |
| Stock insuficiente bloquea producción | `Stock_Actual` por debajo del pedido | Actualizar con `UPDATE Almacen SET Stock_Actual = 200 WHERE Codigo = 'C00X';` |

## Autores
SoniaS. Galeas, Marcos Mérida, Lucía Remolar y Marta Ros
Grupo PR2‑1-3  
Grado en Informática Industrial y Robótica — UPV  
Curso 2026
