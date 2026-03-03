DELETE FROM "OrderRoutePoints"
WHERE order_id = 1, -- from app
			index = (
				SELECT MAX(index) FROM "OrderRoutePoints"
				WHERE order_id = 1
			)


