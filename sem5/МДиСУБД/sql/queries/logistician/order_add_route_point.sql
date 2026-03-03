-- ensure "Order" is confirmed
INSERT 
     INTO "OrderRoutePoints" (order_id, warehouse_id, index)
SELECT 1, -- from app
       1, -- from app
       COALESCE(MAX(index), -1)+1
FROM "OrderRoutePoints" 
WHERE order_id = 1 -- from app
