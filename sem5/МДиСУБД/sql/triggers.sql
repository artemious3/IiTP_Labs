-------------------------------------------------------
------------ Order is nonempty when confirmed ---------
-------------------------------------------------------

CREATE OR REPLACE FUNCTION check_order_nonempty ()
RETURNS TRIGGER
AS
$$
BEGIN
	IF (SELECT COUNT(*) FROM "OrderProductRelation" WHERE order_id=OLD.id) = 0
		THEN
			RAISE 'Order must have some products before confirmation';
		ELSE
			RETURN NEW;
		END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER t_check_order_nonempty BEFORE UPDATE OF state
ON "Order"
FOR EACH ROW
WHEN (NEW.state = 'Confirmed')
EXECUTE FUNCTION check_order_nonempty()



----------------------------------------------------------
------------  When order completed it is at dropsite -----
----------------------------------------------------------

CREATE OR REPLACE FUNCTION check_order_at_dropsite()
RETURNS TRIGGER
AS
$$
BEGIN
	IF NEW.current_warehouse != NEW.dropsite
		THEN
			RAISE 'The order should be at a dropsite when completed';
		ELSE
			RETURN NEW;
		END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER t_check_order_at_dropsite BEFORE UPDATE OF state
ON "Order"
FOR EACH ROW
WHEN (NEW.state = 'Completed')
EXECUTE FUNCTION check_order_at_dropsite()


----------------------------------------------------------
------------ Order of states ----------------------------
----------------------------------------------------------

CREATE OR REPLACE FUNCTION check_prev_and_next_state()
RETURNS TRIGGER
AS
$$
BEGIN
	IF (OLD.state = 'Created' AND NEW.state='Confirmed')
	    OR
			(OLD.state = 'Confirmed' AND NEW.state='Routed')
      OR
			(OLD.state = 'Routed' AND NEW.state='Completed')
		THEN
			RETURN NEW;
		ELSE
  		RAISE 'State pipeline violation : must be Created -> Confirmed -> Routed -> Completed';
		END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER t_check_prev_and_next_state BEFORE UPDATE OF state
ON "Order"
FOR EACH ROW
EXECUTE FUNCTION check_prev_and_next_state();

----------------------------------------------------------
------------ Updating confirmed order is prohibited ------
----------------------------------------------------------

CREATE OR REPLACE FUNCTION check_created_order()
RETURNS TRIGGER
AS
$$
DECLARE
  ostate order_state;
BEGIN

IF (tg_op = 'UPDATE' OR tg_op = 'INSERT')
  THEN
    SELECT state INTO ostate FROM "Order" o WHERE o.id = NEW.order_id;
  ELSE
    SELECT state INTO ostate FROM "Order" o WHERE o.id = OLD.order_id;
END IF;

IF ostate != 'Created' THEN
    RAISE 'Modifying confirmed order is prohibited';
ELSE
    IF tg_op = 'DELETE' THEN
        RETURN OLD;
    ELSE
        RETURN NEW;
    END IF;
END IF;

END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE TRIGGER t_check_confirmed_order BEFORE UPDATE OR INSERT OR DELETE
ON "OrderProductRelation"
FOR EACH ROW
EXECUTE FUNCTION check_created_order();


--------------------------------------------------------------
------------ Routing is allowed only on confirmed order ------
--------------------------------------------------------------

CREATE OR REPLACE FUNCTION check_confirmed_order()
RETURNS TRIGGER
AS
$$
DECLARE
  ostate order_state;
BEGIN

IF (tg_op = 'UPDATE' OR tg_op = 'INSERT')
  THEN
    SELECT state INTO ostate FROM "Order" o WHERE o.id = NEW.order_id;
  ELSE
    SELECT state INTO ostate FROM "Order" o WHERE o.id = OLD.order_id;
END IF;

IF ostate != 'Confirmed' THEN
    RAISE 'Routing is allowed only for confirmed orders';
ELSE
    IF tg_op = 'DELETE' THEN
        RETURN OLD;
    ELSE
        RETURN NEW;
    END IF;
END IF;

END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER t_check_confirmed_order BEFORE UPDATE OR INSERT OR DELETE
ON "OrderRoutePoints"
FOR EACH ROW
EXECUTE FUNCTION check_confirmed_order();



-------------------------------------------------------------------------
------------ Check and update target_warehouse when order is routed------
-------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION on_order_routed ()
RETURNS TRIGGER
AS
$$
DECLARE
    first_wh BIGINT;
    max_index INT;
BEGIN
	IF (SELECT COUNT(*) FROM "OrderRoutePoints" WHERE order_id=NEW.id) = 0
		THEN
			RAISE 'Order route must not be empty';
	END IF;

	SELECT MAX(index) INTO max_index FROM "OrderRoutePoints"
	    WHERE order_id=NEW.id;

	IF (SELECT warehouse_id FROM "OrderRoutePoints" WHERE
	        (order_id,index)=(NEW.id,max_index))
		 != NEW.dropsite THEN
			RAISE 'The last route point must be dropsite';
	END IF;

	SELECT warehouse_id FROM "OrderRoutePoints" INTO first_wh
	WHERE (order_id,index)=(NEW.id,0);

	NEW.current_warehouse=NULL;
	NEW.target_warehouse=first_wh;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER t_check_route_nonempty BEFORE UPDATE OF state
ON "Order"
FOR EACH ROW
WHEN (NEW.state = 'Routed')
EXECUTE FUNCTION on_order_routed()

-------------------------------------------------------------------------
------------ Check that dropsite is dropsite ---------------------------
-------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION check_dropsite()
RETURNS TRIGGER
AS
$$
BEGIN
    IF NOT (SELECT is_dropsite FROM "Warehouse" WHERE id=NEW.dropsite)
        THEN
        RAISE 'The warehouse, specified as dropsite, is not a dropsite';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE TRIGGER t_check_dropsite BEFORE UPDATE OF dropsite
ON "Order"
FOR EACH ROW
EXECUTE FUNCTION check_dropsite();

-------------------------------------------------------------------------
------------ Insert route point trigger ---------------------------
-------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION on_add_route_point()
RETURNS TRIGGER
AS
$$
DECLARE
    max_index INT;
BEGIN
    SELECT COALESCE(MAX(index),-1) INTO max_index FROM "OrderRoutePoints"
    	    WHERE order_id=NEW.order_id;

    NEW.index = max_index+1;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER t_on_add_route_point BEFORE INSERT
ON "OrderRoutePoints"
FOR EACH ROW
EXECUTE FUNCTION on_add_route_point();

-------------------------------------------------------------------------
------------ Remove route point trigger ---------------------------
-------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION on_delete_route_point()
RETURNS TRIGGER
AS
$$
BEGIN
    UPDATE "OrderRoutePoints" SET index=index-1
    WHERE order_id=OLD.order_id AND index > OLD.index;
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER t_on_delete_route_point AFTER DELETE
ON "OrderRoutePoints"
FOR EACH ROW
EXECUTE FUNCTION on_delete_route_point();
