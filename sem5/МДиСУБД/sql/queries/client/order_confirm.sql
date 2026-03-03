-- TODO in LR5 check dropsite is dropsite
-- TODO in LR5 ensure order is not empty
UPDATE "Order"
SET dropsite = 1,
		state = 'Confirmed'
WHERE id = 1;
