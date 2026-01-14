CREATE OR REPLACE FUNCTION getOrderStatus(p_orderid INT)
RETURNS VARCHAR AS $$
DECLARE
    v_status VARCHAR;
BEGIN
    SELECT status INTO v_status FROM orders WHERE orderid = p_orderid;
    RETURN COALESCE(v_status, 'NOT_FOUND');
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getUserOrderCount(p_userid INT)
RETURNS INT AS $$
BEGIN
    RETURN COALESCE((SELECT COUNT(*) FROM orders WHERE userid = p_userid), 0);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getTotalSpentByUser(p_userid INT)
RETURNS NUMERIC AS $$
BEGIN
    RETURN COALESCE((SELECT SUM(totalprice) FROM orders 
                     WHERE userid = p_userid AND status != 'canceled'), 0);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION canReturnOrder(p_orderid INT)
RETURNS BOOLEAN AS $$
BEGIN
    RETURN EXISTS(SELECT 1 FROM orders WHERE orderid = p_orderid AND status = 'completed');
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getOrderStatusHistory(p_orderid INT)
RETURNS TABLE(historyid INT, oldstatus VARCHAR, newstatus VARCHAR, 
              changedat TIMESTAMP, changedby INT) AS $$
BEGIN
    RETURN QUERY
    SELECT h.historyid, h.oldstatus, h.newstatus, h.changedat, h.changedby
    FROM orderstatushistory h
    WHERE h.orderid = p_orderid
    ORDER BY h.changedat DESC;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION createOrder(p_userid INT, p_productid INT, p_quantity INT)
RETURNS INT AS $$
DECLARE
    v_price NUMERIC;
    v_stock INT;
    v_orderid INT;
    v_total NUMERIC;
BEGIN
    SELECT price, stockquantity INTO v_price, v_stock
    FROM products WHERE productid = p_productid FOR UPDATE;

    IF v_price IS NULL THEN
        RAISE EXCEPTION 'Product not found';
    END IF;

    IF v_stock < p_quantity THEN
        RAISE EXCEPTION 'Not enough stock';
    END IF;

    v_total := v_price * p_quantity;

    INSERT INTO orders (userid, status, totalprice)
    VALUES (p_userid, 'pending', v_total)
    RETURNING orderid INTO v_orderid;

    INSERT INTO orderitems (orderid, productid, quantity, price)
    VALUES (v_orderid, p_productid, p_quantity, v_price);

    UPDATE products SET stockquantity = stockquantity - p_quantity
    WHERE productid = p_productid;

    RETURN v_orderid;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getAuditLogByUser(p_userid INT)
RETURNS TABLE(logid INT, entitytype VARCHAR, entityid INT, operation VARCHAR, 
              performedby INT, performedat TIMESTAMP) AS $$
BEGIN
    RETURN QUERY
    SELECT a.logid, a.entitytype, a.entityid, a.operation, a.performedby, a.performedat
    FROM auditlog a
    WHERE a.performedby = p_userid
    ORDER BY a.performedat DESC;
END;
$$ LANGUAGE plpgsql;
