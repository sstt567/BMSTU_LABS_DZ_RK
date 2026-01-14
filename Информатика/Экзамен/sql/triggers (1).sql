CREATE OR REPLACE FUNCTION trg_product_insert()
RETURNS TRIGGER AS $$
BEGIN
  INSERT INTO audit_log(entity_type,entity_id,operation,performed_by)
  VALUES('product', NEW.product_id, 'insert', 1);
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS product_insert_trigger ON products;
CREATE TRIGGER product_insert_trigger
AFTER INSERT ON products
FOR EACH ROW EXECUTE FUNCTION trg_product_insert();

CREATE OR REPLACE FUNCTION trg_product_update()
RETURNS TRIGGER AS $$
BEGIN
  INSERT INTO audit_log(entity_type,entity_id,operation,performed_by)
  VALUES('product', NEW.product_id, 'update', 1);
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS product_update_trigger ON products;
CREATE TRIGGER product_update_trigger
AFTER UPDATE ON products
FOR EACH ROW EXECUTE FUNCTION trg_product_update();

CREATE OR REPLACE FUNCTION trg_order_update()
RETURNS TRIGGER AS $$
BEGIN
  IF NEW.status <> OLD.status THEN
    INSERT INTO audit_log(entity_type,entity_id,operation,performed_by)
    VALUES('order', NEW.order_id, 'update', 1);
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS order_update_trigger ON orders;
CREATE TRIGGER order_update_trigger
AFTER UPDATE ON orders
FOR EACH ROW EXECUTE FUNCTION trg_order_update();
