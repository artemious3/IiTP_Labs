-- TODO in LR5: check route has at least 2 route points
UPDATE "Order"
SET target_warehouse = (

	SELECT warehouse_id FROM "OrderRoutePoints"
	WHERE order_id = 1
	ORDER BY index 
	FETCH FIRST 1 ROW ONLY

),
		state = 'Routed'
WHERE id = 1;
