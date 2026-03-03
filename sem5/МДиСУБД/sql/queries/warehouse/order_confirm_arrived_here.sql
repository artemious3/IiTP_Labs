UPDATE "Order"
SET current_warehouse = target_warehouse,
		target_warehouse = NULL
WHERE id = 1;

