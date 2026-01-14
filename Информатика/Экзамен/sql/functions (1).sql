CREATE OR REPLACE FUNCTION calculate_order_total(p_order_id INT)
RETURNS NUMERIC AS $$
  SELECT COALESCE(SUM(price*quantity),0) FROM order_items WHERE order_id=p_order_id;
$$ LANGUAGE sql;

CREATE OR REPLACE FUNCTION is_premium(p_user_id INT)
RETURNS BOOLEAN AS $$
  SELECT loyalty_level=1 FROM users WHERE user_id=p_user_id;
$$ LANGUAGE sql;

CREATE OR REPLACE FUNCTION discount_amount(p_user_id INT, p_total NUMERIC)
RETURNS NUMERIC AS $$
  SELECT CASE WHEN is_premium(p_user_id) THEN p_total*0.10 ELSE 0 END;
$$ LANGUAGE sql;

CREATE OR REPLACE FUNCTION get_product_stock(p_product_id INT)
RETURNS INT AS $$
  SELECT stock_quantity FROM products WHERE product_id=p_product_id;
$$ LANGUAGE sql;

CREATE OR REPLACE FUNCTION completed_orders_count(p_user_id INT)
RETURNS INT AS $$
  SELECT COUNT(*) FROM orders WHERE user_id=p_user_id AND status='completed';
$$ LANGUAGE sql;

CREATE OR REPLACE FUNCTION user_role(p_user_id INT)
RETURNS TEXT AS $$
  SELECT role FROM users WHERE user_id=p_user_id;
$$ LANGUAGE sql;
