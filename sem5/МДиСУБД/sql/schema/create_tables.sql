
DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'role_enum') THEN
			CREATE TYPE role_enum AS ENUM ('CLIENT', 'ADMIN', 'LOGISTICIAN', 'WAREHOUSE_STAFF');
		END IF;
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'order_state') THEN
			CREATE TYPE  order_state AS ENUM ('Created', 'Confirmed', 'Routed', 'Completed');
		END IF;
END$$;

CREATE TABLE IF NOT EXISTS "User" (
    id BIGSERIAL PRIMARY KEY,
    login VARCHAR(32) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    phone VARCHAR(20) UNIQUE NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    birth_date TIMESTAMP NOT NULL,
    role role_enum NOT NULL
);

CREATE TABLE  IF NOT EXISTS "Client" (
	id SERIAL PRIMARY KEY,
	user_id BIGINT REFERENCES "User" ON DELETE CASCADE
);

CREATE TABLE  IF NOT EXISTS "PaymentMethod" (
    payment_method_id BIGSERIAL PRIMARY KEY,
    client_id BIGINT NOT NULL REFERENCES "Client" ON DELETE CASCADE,
    token VARCHAR(255) UNIQUE NOT NULL,
    card_type VARCHAR(50) NOT NULL,
    last_four_digits CHAR(4),
    expiration_date DATE NOT NULL,
    is_default BOOLEAN NOT NULL DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE
);


CREATE TABLE  IF NOT EXISTS "Location" (
	id BIGSERIAL PRIMARY KEY,
	coordinates POINT NOT NULL,
	address VARCHAR(255) NOT NULL
);

CREATE TABLE  IF NOT EXISTS "Warehouse" (
	id BIGSERIAL PRIMARY KEY,
	location_id BIGINT NOT NULL  REFERENCES "Location" ON DELETE RESTRICT,
	is_dropsite BOOLEAN NOT NULL
);


CREATE TABLE IF NOT EXISTS "Producer"  (
	id SERIAL PRIMARY KEY,
	name VARCHAR(255) NOT NULL,
	warehouse_id BIGINT NOT NULL REFERENCES "Warehouse" ON DELETE RESTRICT
);


CREATE TABLE IF NOT EXISTS "Product" (
	id BIGSERIAL PRIMARY KEY,
	title  VARCHAR(512) NOT NULL,
	description TEXT,
	mass INT NOT NULL,
	price  MONEY NOT NULL CONSTRAINT price_positive CHECK(price::money::numeric > 0),
	producer_id BIGINT NOT NULL REFERENCES "Producer" ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS "Order" (
	id BIGSERIAL PRIMARY KEY,
	client_id BIGINT NOT NULL REFERENCES "Client" ON DELETE CASCADE,

	is_paid BOOLEAN NOT NULL DEFAULT FALSE,
	state order_state NOT NULL DEFAULT 'Created',

	-- DONE in L4 : trigger that checks if dropsite is dropsite
	dropsite BIGINT REFERENCES "Warehouse" ON DELETE RESTRICT,
	current_warehouse BIGINT REFERENCES "Warehouse" ON DELETE RESTRICT,
	target_warehouse BIGINT REFERENCES "Warehouse" ON DELETE RESTRICT,

	CONSTRAINT paid_if_completed CHECK((state  != 'Completed') OR (state = 'Completed' AND is_paid) ),
	CONSTRAINT not_paid_if_not_confirmed CHECK( (state = 'Created' AND !is_paid) OR state != 'Created' ),
	CONSTRAINT current_or_target_not_null CHECK(
		state != 'Routed' Otate = 'Routed' AND
		(current_warehouse != NULL OR target_warehouse != NULL))
	),
	CONSTRAINT dropsite_not_null_if_confirmed CHECK( state = 'Created' OR dropsite IS NOT NULL )
	-- DONE :  check that last OrderRoutePoint is order_dropsite when order is routed
	-- DONE : check that current_warehouse is dropsite when the order is completed
	-- DONE : check that order is nonempty when confirmed
);

C
-- DONE : check that if product added, the order is not confirmed
CREATE TABLE IF NOT EXISTS "OrderProductRelation" (
	order_id BIGINT NOT NULL REFERENCES "Order" ON DELETE CASCADE,
	product_id BIGINT NOT NULL REFERENCES "Product" ON DELETE CASCADE,
	amount INT NOT NULL CHECK(amount > 0),

	PRIMARY KEY (order_id, product_id)
);

CREATE TABLE IF NOT EXISTS "OrderRoutePoints" (
	order_id BIGINT NOT NULL REFERENCES "Order" ON DELETE CASCADE,
	warehouse_id BIGINT NOT NULL REFERENCES "Warehouse" ON DELETE RESTRICT,
	-- TODO in L4 : trigger that checks the existence of index-1
	-- TODO : trigger on removal of route point
	-- DONE : on any update or remove check that order is confirmed and not routed
	index INT NOT NULL CHECK(index >= 0),

	PRIMARY KEY (order_id, index)
);


CREATE TABLE IF NOT EXISTS "Journal" (
    user_id BIGINT REFERENCES "User" ON DELETE SET NULL,
    login VARCHAR(32), -- in case user is deleted, we have at least login
    message TEXT NOT NULL,
    succcess BOOL NOT NULL,
    timestamp TIMESTAMP DEFAULT NOW()
)
