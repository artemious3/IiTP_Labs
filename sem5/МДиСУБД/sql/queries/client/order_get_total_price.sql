SELECT
    COALESCE(SUM(p.price * opr.amount), 0::money) AS total_price
FROM
    "Order" o
LEFT JOIN
    "OrderProductRelation" opr ON o.id = opr.order_id
LEFT JOIN
    "Product" p ON opr.product_id = p.id
WHERE
    o.id = 1
GROUP BY o.id;
