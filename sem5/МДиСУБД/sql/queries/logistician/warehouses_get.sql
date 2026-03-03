SELECT "Warehouse".id, "Warehouse".location_id, "Location".address, "Warehouse".is_dropsite
	FROM "Warehouse"
JOIN 
	"Location"
ON "Warehouse".location_id="Location".id;
