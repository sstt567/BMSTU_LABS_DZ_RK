CREATE OR REPLACE FUNCTION update_order_status(p_order_id INT, p_new_status TEXT, p_changed_by INT)
RETURNS VOID AS $$
DECLARE oldst TEXT;
BEGIN
  SELECT status INTO oldst FROM orders WHERE order_id=p_order_id;
  IF oldst IS NULL THEN RAISE EXCEPTION 'order not found'; END IF;

  UPDATE orders SET status=p_new_status WHERE order_id=p_order_id;

  INSERT INTO order_status_history(order_id,old_status,new_status,changed_by)
  VALUES(p_order_id,oldst,p_new_status,p_changed_by);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION create_order_with_transaction(p_user_id INT, p_products INT[], p_qty INT[])
RETURNS INT AS $$
DECLARE
  new_id INT;
  i INT;
  stock INT;
BEGIN
  IF array_length(p_products,1) IS NULL OR array_length(p_qty,1) IS NULL THEN
    RAISE EXCEPTION 'empty arrays';
  END IF;

  INSERT INTO orders(user_id,status,total_price) VALUES(p_user_id,'pending',0)
  RETURNING order_id INTO new_id;

  FOR i IN 1..array_length(p_products,1) LOOP
    SELECT stock_quantity INTO stock FROM products WHERE product_id=p_products[i];
    IF stock IS NULL THEN RAISE EXCEPTION 'product not found'; END IF;
    IF stock < p_qty[i] THEN RAISE EXCEPTION 'not enough stock'; END IF;

    INSERT INTO order_items(order_id,product_id,quantity,price)
    SELECT new_id, p_products[i], p_qty[i], price FROM products WHERE product_id=p_products[i];

    UPDATE products SET stock_quantity=stock_quantity-p_qty[i] WHERE product_id=p_products[i];
  END LOOP;

  UPDATE orders SET total_price=calculate_order_total(new_id) WHERE order_id=new_id;
  RETURN new_id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION process_return_order(p_order_id INT, p_by INT)
RETURNS BOOLEAN AS $$
DECLARE
  od TIMESTAMP;
  days INT;
  r RECORD;
BEGIN
  SELECT order_date INTO od FROM orders WHERE order_id=p_order_id;
  IF od IS NULL THEN RAISE EXCEPTION 'order not found'; END IF;

  days := EXTRACT(DAY FROM (CURRENT_TIMESTAMP - od));
  IF days > 30 THEN RETURN FALSE; END IF;

  FOR r IN SELECT product_id, quantity FROM order_items WHERE order_id=p_order_id LOOP
    UPDATE products SET stock_quantity=stock_quantity+r.quantity WHERE product_id=r.product_id;
  END LOOP;

  PERFORM update_order_status(p_order_id,'returned',p_by);
  RETURN TRUE;
END;
$$ LANGUAGE plpgsql;
