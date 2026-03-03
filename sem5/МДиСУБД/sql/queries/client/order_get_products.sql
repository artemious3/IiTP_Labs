SELECT          
"Product".id as product_id,
"Product".title,
"Product".description,
"OrderProductRelation".amount		
FROM "OrderProductRelation"
JOIN "Product" ON "OrderProductRelation".product_id="Product".id
WHERE order_id=1;
