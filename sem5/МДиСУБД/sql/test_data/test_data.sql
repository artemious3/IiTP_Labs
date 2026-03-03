INSERT INTO "Location" (coordinates, address) VALUES
(point(55.751244, 37.618423), '1 Red Square, Moscow'),
(point(59.934280, 30.335099), '2 Palace Embankment, St. Petersburg'),
(point(56.838011, 60.597465), '1905 Square, Yekaterinburg'),
(point(55.030199, 82.920430), 'Lenin Square, Novosibirsk'),
(point(43.115542, 131.885494), 'Svetlanskaya Street, Vladivostok');

INSERT INTO "Warehouse" (location_id, is_dropsite) VALUES
(1, FALSE), 
(2, TRUE), 
(3, FALSE),
(4, TRUE), 
(5, TRUE);

INSERT INTO "Producer" (name, warehouse_id) VALUES
('Red October', 1),
('Babaevsky', 1),
('Ural Gems', 3);

INSERT INTO "Product" (title, descripion, mass, price, producer_id) VALUES
('Alyonka Chocolate', 'Classic milk chocolate bar, a taste of childhood.', 100, 1.50, 1),
('Mishka Kosolapy', 'Wafers covered in rich chocolate with a praline filling.', 250, 3.20, 1),
('Vdokhnovenie (Inspiration)', 'Fine dark chocolate with crushed almonds.', 100, 2.80, 2),
('Malachite Casket', 'Assortment of chocolates with various fillings.', 500, 15.00, 3),
('Siberian Treasure', 'Pine nut brittle in dark chocolate.', 150, 7.50, 3);
