SELECT * FROM "Order"
WHERE 
	EXISTS(SELECT id FROM "OrderProductRelation" WHERE "OrderProductRelation".order_id = "Order".id);
