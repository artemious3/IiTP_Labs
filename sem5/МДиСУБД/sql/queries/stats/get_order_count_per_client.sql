SELECT  "User".first_name, "User".last_name, COUNT(*) as count
FROM "Order"
JOIN "Client" ON "Client".id=client_id
JOIN "User" ON "User".id="Client".user_id
GROUP BY "User".id
ORDER BY count DESC;
