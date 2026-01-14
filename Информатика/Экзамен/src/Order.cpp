#include "Order.h"
#include <pqxx/pqxx>

int OrderService::createEmptyOrder(DatabaseConnection<std::string>& db, int userId) {
    pqxx::work tx(db.raw());
    auto r = tx.exec_params("INSERT INTO orders(user_id,status,total_price) VALUES($1,'pending',0) RETURNING order_id", userId);
    tx.commit();
    return r[0][0].as<int>();
}

void OrderService::addItem(DatabaseConnection<std::string>& db, int orderId, int productId, int qty) {
    pqxx::work tx(db.raw());

    auto pr = tx.exec_params("SELECT price, stock_quantity FROM products WHERE product_id=$1", productId);
    if (pr.empty()) throw std::runtime_error("product not found");
    double price = pr[0][0].as<double>();
    int stock = pr[0][1].as<int>();
    if (stock < qty) throw std::runtime_error("not enough stock");

    tx.exec_params("INSERT INTO order_items(order_id,product_id,quantity,price) VALUES($1,$2,$3,$4)",
                   orderId, productId, qty, price);

    tx.exec_params("UPDATE products SET stock_quantity=stock_quantity-$1 WHERE product_id=$2", qty, productId);
    tx.exec_params("UPDATE orders SET total_price=calculate_order_total($1) WHERE order_id=$1", orderId);

    tx.commit();
}

void OrderService::payOrder(DatabaseConnection<std::string>& db, int orderId, std::unique_ptr<Payment> p) {
    if (!p) throw std::runtime_error("payment is null");

    pqxx::work tx(db.raw());
    auto r = tx.exec_params("SELECT total_price, status FROM orders WHERE order_id=$1", orderId);
    if (r.empty()) throw std::runtime_error("order not found");

    double total = r[0][0].as<double>();
    std::string st = r[0][1].c_str();
    if (st != "pending") throw std::runtime_error("order not pending");

    if (!p->pay(total)) throw std::runtime_error("payment failed");

    tx.exec_params("SELECT update_order_status($1,'completed',$2)", orderId, 1);
    tx.commit();
}

void OrderService::requestReturn(DatabaseConnection<std::string>& db, int orderId, int userId) {
    pqxx::work tx(db.raw());
    auto r = tx.exec_params("SELECT user_id FROM orders WHERE order_id=$1", orderId);
    if (r.empty()) throw std::runtime_error("order not found");
    if (r[0][0].as<int>() != userId) throw std::runtime_error("not your order");

    auto ok = tx.exec_params("SELECT process_return_order($1,$2)", orderId, userId);
    tx.commit();
    if (ok.empty() || !ok[0][0].as<bool>()) throw std::runtime_error("return failed");
}

std::string OrderService::getStatus(DatabaseConnection<std::string>& db, int orderId) {
    auto rows = db.executeQuery("SELECT status FROM orders WHERE order_id=" + std::to_string(orderId));
    if (rows.empty()) return "unknown";
    return rows[0][0];
}
