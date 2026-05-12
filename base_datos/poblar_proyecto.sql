SET search_path TO PR2_1_3;

INSERT INTO Cliente (DNI_CIF, Nombre, Direccion_Envio) VALUES 
('12345678A', 'Carlos Martínez', 'Calle Mayor 12, Valencia'),
('87654321B', 'Laura Gómez', 'Av. Libertad 45, Madrid');

INSERT INTO Telefono_Cliente (DNI_CIF, Telefono) VALUES 
('12345678A', '600123456'),
('87654321B', '611234567');

INSERT INTO Proveedor (NIF, Nombre, Pais_Origen) VALUES 
('25379', 'Cartones Manolo', 'Bulgaria'),
('46328', 'Blosas Plastik', 'Francia'),
('58291', 'MegaParts', 'Alemania'),
('77412', 'CompuLego', 'España');

INSERT INTO Telefono_Proveedor (NIF, Telefono) VALUES 
('25379', '875968574'),
('46328', '875693140');


INSERT INTO Almacen (Codigo, Stock_Actual, Stock_Minimo) VALUES 
('C001', 120, 50),
('C002', 300, 100),
('C003', 280, 90),
('C004', 150, 60),
('C005', 200, 80);

INSERT INTO Componente (Codigo, Nombre) VALUES 
('C001', 'Caja vacía'),
('C002', 'Bolsa piezas A'),
('C003', 'Bolsa piezas B'),
('C004', 'Tapa de caja'),
('C005', 'Manual instrucciones');

INSERT INTO Tipo_Caja (Codigo, Stock_Actual, Stock_Minimo, Precio_Fabricacion) VALUES 
('TC01', 80, 20, 14.99), -- Amarilla
('TC02', 120, 30, 19.99); -- Roja

INSERT INTO Pedido (Numero_Localizador, Fecha_Pedido, DNI_CIF) VALUES 
('P001', '2024-05-10', '12345678A'),
('P002', '2024-05-11', '87654321B');

INSERT INTO Caja (Cod, Tipo_Atributo, Acabada, Numero_Localizador, Codigo_Tipo) VALUES 
('BX001', 'Caja Amarilla T1', FALSE, 'P001', 'TC01'),
('BX002', 'Caja Roja T2', FALSE, 'P002', 'TC02');

INSERT INTO Pedido_Tiene_Tipo (Numero_Localizador, Codigo_Tipo, Cantidad) VALUES 
('P001', 'TC01', 2),
('P002', 'TC02', 1);

INSERT INTO Tipo_Contiene_Componente (Codigo_Tipo, Codigo_Componente, Cantidad) VALUES 
('TC01', 'C001', 1),
('TC01', 'C002', 1),
('TC01', 'C004', 1),
('TC02', 'C001', 1),
('TC02', 'C003', 1),
('TC02', 'C004', 1);

INSERT INTO Suministra (Codigo_Componente, NIF_Proveedor, Precio_Compra, Cantidad, Fecha_Pedido) VALUES 
('C001', '46328', 0.45, 1000, '2024-05-01'),
('C002', '58291', 1.20, 500, '2024-05-01'),
('C003', '77412', 1.10, 500, '2024-05-01');
