-- Users table
CREATE TABLE IF NOT EXISTS users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    role VARCHAR(20) NOT NULL CHECK (role IN ('admin', 'manager', 'customer')),
    password_hash VARCHAR(255) NOT NULL,
    loyalty_level INTEGER DEFAULT 0
);

-- Products table
CREATE TABLE IF NOT EXISTS products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    price NUMERIC(10,2) NOT NULL CHECK (price > 0),
    stock_quantity INTEGER NOT NULL CHECK (stock_quantity >= 0)
);

-- Orders table
CREATE TABLE IF NOT EXISTS orders (
    order_id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL REFERENCES users(user_id),
    status VARCHAR(20) NOT NULL DEFAULT 'pending' 
        CHECK (status IN ('pending','completed','canceled','returned')),
    total_price NUMERIC(10,2) DEFAULT 0,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Order items table
CREATE TABLE IF NOT EXISTS order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INTEGER NOT NULL REFERENCES orders(order_id) ON DELETE CASCADE,
    product_id INTEGER NOT NULL REFERENCES products(product_id),
    quantity INTEGER NOT NULL CHECK (quantity > 0),
    price NUMERIC(10,2) NOT NULL CHECK (price > 0)
);

-- Order status history table
CREATE TABLE IF NOT EXISTS order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INTEGER NOT NULL REFERENCES orders(order_id) ON DELETE CASCADE,
    old_status VARCHAR(20),
    new_status VARCHAR(20) NOT NULL,
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INTEGER REFERENCES users(user_id)
);

-- Audit log table
CREATE TABLE IF NOT EXISTS audit_log (
    log_id SERIAL PRIMARY KEY,
    entity_type VARCHAR(20) NOT NULL CHECK (entity_type IN ('order','product','user')),
    entity_id INTEGER NOT NULL,
    operation VARCHAR(20) NOT NULL CHECK (operation IN ('insert','update','delete')),
    performed_by INTEGER,
    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Indexes
CREATE INDEX IF NOT EXISTS idx_orders_user_id ON orders(user_id);
CREATE INDEX IF NOT EXISTS idx_orders_status ON orders(status);
CREATE INDEX IF NOT EXISTS idx_order_items_order_id ON order_items(order_id);

-- Function: getOrderStatus
CREATE OR REPLACE FUNCTION getOrderStatus(p_order_id INT)
RETURNS VARCHAR AS $$
DECLARE
    v_status VARCHAR;
BEGIN
    SELECT status INTO v_status FROM orders WHERE order_id = p_order_id;
    RETURN COALESCE(v_status, 'NOT_FOUND');
END;
$$ LANGUAGE plpgsql;

-- Function: getUserOrderCount
CREATE OR REPLACE FUNCTION getUserOrderCount(p_user_id INT)
RETURNS INT AS $$
BEGIN
    RETURN COALESCE((SELECT COUNT(*) FROM orders WHERE user_id = p_user_id), 0);
END;
$$ LANGUAGE plpgsql;

-- Function: getTotalSpentByUser
CREATE OR REPLACE FUNCTION getTotalSpentByUser(p_user_id INT)
RETURNS NUMERIC AS $$
BEGIN
    RETURN COALESCE((SELECT SUM(total_price) FROM orders 
                     WHERE user_id = p_user_id AND status != 'canceled'), 0);
END;
$$ LANGUAGE plpgsql;

-- Function: canReturnOrder
CREATE OR REPLACE FUNCTION canReturnOrder(p_order_id INT)
RETURNS BOOLEAN AS $$
BEGIN
    RETURN EXISTS(SELECT 1 FROM orders WHERE order_id = p_order_id AND status = 'completed');
END;
$$ LANGUAGE plpgsql;

-- Function: getOrderStatusHistory
CREATE OR REPLACE FUNCTION getOrderStatusHistory(p_order_id INT)
RETURNS TABLE(history_id INT, old_status VARCHAR, new_status VARCHAR, 
              changed_at TIMESTAMP, changed_by INT) AS $$
BEGIN
    RETURN QUERY
    SELECT h.history_id, h.old_status, h.new_status, h.changed_at, h.changed_by
    FROM order_status_history h
    WHERE h.order_id = p_order_id
    ORDER BY h.changed_at DESC;
END;
$$ LANGUAGE plpgsql;

-- Function: createOrder
CREATE OR REPLACE FUNCTION createOrder(p_user_id INT, p_product_id INT, p_quantity INT)
RETURNS INT AS $$
DECLARE
    v_price NUMERIC;
    v_stock INT;
    v_order_id INT;
    v_total NUMERIC;
BEGIN
    SELECT price, stock_quantity INTO v_price, v_stock
    FROM products WHERE product_id = p_product_id FOR UPDATE;

    IF v_price IS NULL THEN
        RAISE EXCEPTION 'Product not found';
    END IF;

    IF v_stock < p_quantity THEN
        RAISE EXCEPTION 'Not enough stock';
    END IF;

    v_total := v_price * p_quantity;

    INSERT INTO orders (user_id, status, total_price)
    VALUES (p_user_id, 'pending', v_total)
    RETURNING order_id INTO v_order_id;

    INSERT INTO order_items (order_id, product_id, quantity, price)
    VALUES (v_order_id, p_product_id, p_quantity, v_price);

    UPDATE products SET stock_quantity = stock_quantity - p_quantity
    WHERE product_id = p_product_id;

    RETURN v_order_id;
END;
$$ LANGUAGE plpgsql;

-- Trigger: log order status changes
CREATE OR REPLACE FUNCTION trg_log_order_status()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.status IS DISTINCT FROM OLD.status THEN
        INSERT INTO order_status_history(order_id, old_status, new_status, changed_by)
        VALUES (NEW.order_id, OLD.status, NEW.status, NEW.user_id);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_orders_status ON orders;
CREATE TRIGGER trg_orders_status
AFTER UPDATE OF status ON orders
FOR EACH ROW
EXECUTE FUNCTION trg_log_order_status();

-- Trigger: update total price on product price change
CREATE OR REPLACE FUNCTION trg_update_total_on_product()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE orders SET total_price = (
        SELECT SUM(oi.quantity * oi.price)
        FROM order_items oi
        WHERE oi.order_id = orders.order_id
    )
    WHERE order_id IN (
        SELECT order_id FROM order_items WHERE product_id = NEW.product_id
    );
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_product_price ON products;
CREATE TRIGGER trg_product_price
AFTER UPDATE OF price ON products
FOR EACH ROW
EXECUTE FUNCTION trg_update_total_on_product();

-- Trigger: audit log
CREATE OR REPLACE FUNCTION trg_audit_orders()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO audit_log(entity_type, entity_id, operation, performed_by)
    VALUES ('order', COALESCE(NEW.order_id, OLD.order_id), 
            LOWER(TG_OP), COALESCE(NEW.user_id, OLD.user_id));
    RETURN COALESCE(NEW, OLD);
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_orders_audit ON orders;
CREATE TRIGGER trg_orders_audit
AFTER INSERT OR UPDATE OR DELETE ON orders
FOR EACH ROW
EXECUTE FUNCTION trg_audit_orders();

-- Test data
INSERT INTO users (name, email, role, password_hash, loyalty_level) VALUES
('Admin User', 'admin@shop.com', 'admin', 'hash1', 0),
('Manager User', 'manager@shop.com', 'manager', 'hash2', 0),
('Ivan Petrov', 'ivan@shop.com', 'customer', 'hash3', 1),
('Maria Smirnova', 'maria@shop.com', 'customer', 'hash4', 0)
ON CONFLICT DO NOTHING;

INSERT INTO products (name, price, stock_quantity) VALUES
('Laptop', 999.99, 10),
('Mouse', 25.99, 50),
('Keyboard', 79.99, 30),
('Monitor', 299.99, 15),
('Headphones', 149.99, 25)
ON CONFLICT DO NOTHING;

INSERT INTO orders (user_id, status, total_price, order_date) VALUES
(3, 'completed', 1025.98, NOW() - INTERVAL '5 days'),
(4, 'pending', 0, NOW())
ON CONFLICT DO NOTHING;
