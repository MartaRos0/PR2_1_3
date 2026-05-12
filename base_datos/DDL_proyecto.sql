-- DROP SCHEMA IF EXISTS PR2_1_3 CASCADE;  
CREATE SCHEMA PR2_1_3;
SET search_path TO PR2_1_3;

-- TABLAS MAESTRAS

CREATE TABLE Cliente (
DNI_CIF VARCHAR(15) PRIMARY KEY,
Nombre VARCHAR(100) NOT NULL,
Direccion_Envio VARCHAR(200) NOT NULL
);

CREATE TABLE Telefono_Cliente (
DNI_CIF VARCHAR(15) REFERENCES Cliente(DNI_CIF) ON DELETE CASCADE,
Telefono VARCHAR(20),
PRIMARY KEY (DNI_CIF, Telefono)
);

CREATE TABLE Proveedor (
NIF VARCHAR(15) PRIMARY KEY,
Nombre VARCHAR(100) NOT NULL,
Pais_Origen VARCHAR(50)
);

CREATE TABLE Telefono_Proveedor (
NIF VARCHAR(15) REFERENCES Proveedor(NIF) ON DELETE CASCADE,
Telefono VARCHAR(20),
PRIMARY KEY (NIF, Telefono)
);


CREATE TABLE Tipo_Caja (
Codigo VARCHAR(20) PRIMARY KEY,
Stock_Actual INTEGER,
Stock_Minimo INTEGER NOT NULL, 
Precio_Fabricacion NUMERIC(10,2) NOT NULL
);

CREATE TABLE Almacen (
    Codigo VARCHAR(20) PRIMARY KEY,
    Stock_Actual INTEGER,
    Stock_Minimo INTEGER NOT NULL  
);

-- TABLAS DEPENDIENTES (Relaciones de 1:1 y 1:muchos)


CREATE TABLE Componente (
    Codigo VARCHAR(20) PRIMARY KEY REFERENCES Almacen(Codigo) ON DELETE CASCADE,
    Nombre VARCHAR(100) NOT NULL
);


CREATE TABLE Pedido (
Numero_Localizador VARCHAR(20) PRIMARY KEY,
Fecha_Pedido DATE NOT NULL,
DNI_CIF VARCHAR(15) REFERENCES Cliente(DNI_CIF) -- Relación "Realiza"
);

CREATE TABLE Caja (
Cod VARCHAR(20) PRIMARY KEY, -- ID único (ej. LEGO-001)
Tipo_Atributo VARCHAR(50),    -- Atributo "Tipo" del UML
Acabada BOOLEAN NOT NULL DEFAULT FALSE,
Numero_Localizador VARCHAR(20) REFERENCES Pedido(Numero_Localizador) ON DELETE CASCADE,
Codigo_Tipo VARCHAR(20) REFERENCES Tipo_Caja(Codigo) -- Relación 1:1 a 1..*
);

--TABLAS DE ASOCIACIÓN 
-- Relación "Tiene"  es la que especifica que tipos y cuántos lleva el pedido
CREATE TABLE Pedido_Tiene_Tipo (
Numero_Localizador VARCHAR(20) REFERENCES Pedido(Numero_Localizador) ON DELETE CASCADE,
Codigo_Tipo VARCHAR(20) REFERENCES Tipo_Caja(Codigo),
Cantidad INTEGER NOT NULL DEFAULT 1, -- Atributo de asociación en UML
PRIMARY KEY (Numero_Localizador, Codigo_Tipo)
);

-- Relación "Contiene" es básicamente la receta del producto
CREATE TABLE Tipo_Contiene_Componente (
Codigo_Tipo VARCHAR(20) REFERENCES Tipo_Caja(Codigo) ON DELETE CASCADE,
Codigo_Componente VARCHAR(20) REFERENCES Componente(Codigo),
Cantidad INTEGER NOT NULL DEFAULT 1, -- Atributo de asociación en UML
PRIMARY KEY (Codigo_Tipo, Codigo_Componente)
);

-- Relación "Suministra"  relaciona al componente con quien lo suministra
CREATE TABLE Suministra (
Codigo_Componente VARCHAR(20) REFERENCES Componente(Codigo),
NIF_Proveedor VARCHAR(15) REFERENCES Proveedor(NIF),
Precio_Compra NUMERIC(10,2), 
Cantidad INTEGER,          
Fecha_Pedido DATE,          
PRIMARY KEY (Codigo_Componente, NIF_Proveedor) );
