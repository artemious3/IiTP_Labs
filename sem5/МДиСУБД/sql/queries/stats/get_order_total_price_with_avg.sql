
WITH t AS 

(SELECT "Order".id AS order_id, "Order".client_id,
				COALESCE(SUM("OrderProductRelation".amount *  "Product".price), 0.00::money) as total_price
FROM "OrderProductRelation"
	JOIN "Product"
			ON "OrderProductRelation".product_id = "Product".id
	RIGHT JOIN "Order"
			ON "OrderProductRelation".order_id = "Order".id
-- HAVING  total_price > 100.00::money;
GROUP BY "Order".id)

SELECT order_id, client_id, total_price, AVG(t.total_price::money::numeric) OVER(PARTITION BY t.client_id)
FROM t;
