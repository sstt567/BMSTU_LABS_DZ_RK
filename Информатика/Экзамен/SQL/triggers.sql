CREATE OR REPLACE FUNCTION trg_log_order_status()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.status IS DISTINCT FROM OLD.status THEN
        INSERT INTO orderstatushistory(orderid, oldstatus, newstatus, changedby)
        VALUES (NEW.orderid, OLD.status, NEW.status, NEW.userid);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trigger_order_status ON orders;
CREATE TRIGGER trigger_order_status
AFTER UPDATE OF status ON orders
FOR EACH ROW EXECUTE FUNCTION trg_log_order_status();

CREATE OR REPLACE FUNCTION trg_update_total_on_product()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE orders SET totalprice = (
        SELECT COALESCE(SUM(oi.quantity * oi.price), 0)
        FROM orderitems oi
        WHERE oi.orderid = orders.orderid
    )
    WHERE orderid IN (
        SELECT orderid FROM orderitems WHERE productid = NEW.productid
    );
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trigger_product_price ON products;
CREATE TRIGGER trigger_product_price
AFTER UPDATE OF price ON products
FOR EACH ROW EXECUTE FUNCTION trg_update_total_on_product();

CREATE OR REPLACE FUNCTION trg_audit_orders()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO auditlog(entitytype, entityid, operation, performedby)
    VALUES ('order', COALESCE(NEW.orderid, OLD.orderid), 
            LOWER(TG_OP), COALESCE(NEW.userid, OLD.userid));
    RETURN COALESCE(NEW, OLD);
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trigger_orders_audit ON orders;
CREATE TRIGGER trigger_orders_audit
AFTER INSERT OR UPDATE OR DELETE ON orders
FOR EACH ROW EXECUTE FUNCTION trg_audit_orders();
