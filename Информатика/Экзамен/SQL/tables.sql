CREATE TABLE users (
    userid SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    role VARCHAR(20) CHECK (role IN ('admin', 'manager', 'customer')),
    passwordhash VARCHAR(255),
    loyaltylevel INTEGER DEFAULT 0
);

CREATE TABLE products (
    productid SERIAL PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    price DECIMAL(10,2) CHECK (price > 0),
    stockquantity INTEGER CHECK (stockquantity >= 0)
);

CREATE TABLE orders (
    orderid SERIAL PRIMARY KEY,
    userid INTEGER REFERENCES users(userid),
    status VARCHAR(20) DEFAULT 'pending' 
        CHECK (status IN ('pending', 'completed', 'canceled', 'returned')),
    totalprice DECIMAL(10,2) DEFAULT 0,
    orderdate TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE orderitems (
    orderitemid SERIAL PRIMARY KEY,
    orderid INTEGER REFERENCES orders(orderid) ON DELETE CASCADE,
    productid INTEGER REFERENCES products(productid),
    quantity INTEGER CHECK (quantity > 0),
    price DECIMAL(10,2) CHECK (price > 0)
);

CREATE TABLE orderstatushistory (
    historyid SERIAL PRIMARY KEY,
    orderid INTEGER REFERENCES orders(orderid) ON DELETE CASCADE,
    oldstatus VARCHAR(20),
    newstatus VARCHAR(20),
    changedat TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changedby INTEGER REFERENCES users(userid)
);

CREATE TABLE auditlog (
    logid SERIAL PRIMARY KEY,
    entitytype VARCHAR(20) CHECK (entitytype IN ('order', 'product', 'user')),
    entityid INTEGER,
    operation VARCHAR(20) CHECK (operation IN ('insert', 'update', 'delete')),
    performedby INTEGER,
    performedat TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_orders_user ON orders(userid);
CREATE INDEX idx_orders_status ON orders(status);
CREATE INDEX idx_order_items ON orderitems(orderid);
CREATE INDEX idx_history ON orderstatushistory(orderid);
CREATE INDEX idx_audit ON auditlog(entitytype);
