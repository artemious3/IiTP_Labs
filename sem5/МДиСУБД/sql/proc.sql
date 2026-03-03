

CREATE OR REPLACE PROCEDURE "GetOrderPrices"()
LANGUAGE plsql
AS $$
BEGIN

SELECT "Order".id,
				COALESCE(SUM("OrderProductRelation".amount *  "Product".price), 0.00::money) as total_price
FROM "OrderProductRelation"
	JOIN "Product"
			ON "OrderProductRelation".product_id = "Product".id
	RIGHT JOIN "Order"
			ON "OrderProductRelation".order_id = "Order".id
GROUP BY "Order".id;
COMMIT:

END;$$
