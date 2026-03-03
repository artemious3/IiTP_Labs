SELECT "Warehouse".id, "Warehouse".location_id, "Location".address
	FROM "Warehouse"
JOIN 
	"Location"
ON "Warehouse".location_id="Location".id
WHERE is_dropsite=TRUE;
