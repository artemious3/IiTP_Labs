SELECT "OrderRoutePoints".index, "Warehouse".id, "Location".address, "Warehouse".is_dropsite
	FROM "OrderRoutePoints"
JOIN 
	"Warehouse"
ON "OrderRoutePoints".warehouse_id = "Warehouse".id
JOIN 
	"Location"
ON "Warehouse".location_id="Location".id

WHERE "OrderRoutePoints".order_id = 1
ORDER BY "OrderRoutePoints".index;
