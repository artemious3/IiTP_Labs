CREATE INDEX "ProductSearch" ON "Product"
USING GIN (to_tsvector('english', title || ' ' || description));

CREATE INDEX "Order_ClientIdx" ON "Order" (client_id);

CREATE INDEX "Order_CurrentWarehouseIdx" ON "Order" (current_warehouse);

CREATE INDEX "Order_TargetWarehouseIdx" ON "Order" (target_warehouse);

CREATE INDEX "Order_StateIdx" ON "Order" (state);

CREATE INDEX "Producr_ProducerIdx" ON "Product" (producer_id);


