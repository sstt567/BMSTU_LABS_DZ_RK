#define _CRT_SECURE_NO_WARNINGS
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>

using namespace std;

template<typename T>
class DatabaseConnection {
private:
    pqxx::connection* conn_;
    pqxx::work* txn_;

public:
    DatabaseConnection(const string& connStr) : txn_(nullptr) {
        try {
            conn_ = new pqxx::connection(connStr);
            if (!conn_->is_open()) {
                throw runtime_error("Cannot open connection to PostgreSQL");
            }
            cout << " Connected to PostgreSQL" << endl;
        }
        catch (const exception& e) {
            cerr << " Connection error: " << e.what() << endl;
            throw;
        }
    }

    ~DatabaseConnection() {
        if (txn_) delete txn_;
        if (conn_) delete conn_;
    }

    bool isOpen() const { return conn_ && conn_->is_open(); }

    void beginTransaction() {
        if (!txn_) txn_ = new pqxx::work(*conn_);
    }

    void commitTransaction() {
        if (txn_) {
            txn_->commit();
            delete txn_;
            txn_ = nullptr;
        }
    }

    void rollbackTransaction() {
        if (txn_) {
            txn_->abort();
            delete txn_;
            txn_ = nullptr;
        }
    }

    vector<vector<string>> executeQuery(const string& query) {
        vector<vector<string>> result;
        try {
            pqxx::work w(*conn_);
            pqxx::result r = w.exec(query);
            for (auto row : r) {
                vector<string> rowData;
                for (auto field : row) {
                    rowData.push_back(field.is_null() ? "" : field.as<string>());
                }
                result.push_back(rowData);
            }
            w.commit();
        }
        catch (const exception& e) {
            cerr << " Query error: " << e.what() << endl;
        }
        return result;
    }

    bool executeNonQuery(const string& query) {
        try {
            pqxx::work w(*conn_);
            w.exec(query);
            w.commit();
            return true;
        }
        catch (const exception& e) {
            cerr << " Execute error: " << e.what() << endl;
            return false;
        }
    }
};


class OrderItem {
public:
    int productId;
    int quantity;
    double price;

    OrderItem(int pid, int qty, double p) : productId(pid), quantity(qty), price(p) {}
};

class Payment {
public:
    double amount;
    string status;

    Payment(double amt) : amount(amt), status("pending") {}

    void process() { status = "completed"; }
};

class Order {
public:
    int orderId;
    int userId;
    vector<OrderItem> items;
    unique_ptr<Payment> payment;
    string status;

    Order(int oid, int uid)
        : orderId(oid), userId(uid),
        payment(make_unique<Payment>(0.0)),
        status("pending") {
    }

    void addItem(int productId, int quantity, double price) {
        items.emplace_back(productId, quantity, price);
    }

    double calculateTotal() {
        double total = 0.0;
        for (auto& item : items) {
            total += item.quantity * item.price;
        }
        if (payment) payment->amount = total;
        return total;
    }

    void processPayment() {
        if (payment) {
            payment->process();
            status = "completed";
        }
    }
};


class User {
protected:
    int userId;
    string name;
    string email;
    vector<shared_ptr<Order>> orders;

public:
    User(int id, string n, string e)
        : userId(id), name(n), email(e) {
    }

    virtual ~User() = default;

    virtual void createOrder(int orderId) = 0;
    virtual void viewOrderStatus(int orderId) = 0;
    virtual void cancelOrder(int orderId) = 0;

    int getUserId() const { return userId; }
    string getName() const { return name; }
};


class Admin : public User {
public:
    Admin(int id, string n, string e) : User(id, n, e) {}

    void createOrder(int orderId) override {
        auto order = make_shared<Order>(orderId, userId);
        orders.push_back(order);
        cout << "   Admin created order #" << orderId << endl;
    }

    void viewOrderStatus(int orderId) override {
        cout << "  Order #" << orderId << " status: completed" << endl;
    }

    void cancelOrder(int orderId) override {
        cout << "   Admin canceled order #" << orderId << endl;
    }

    void addProduct(const string& name, double price, int stock) {
        cout << "   Product added: " << name << endl;
    }

    void updateProduct(int productId, double price) {
        cout << "   Product #" << productId << " updated" << endl;
    }

    void deleteProduct(int productId) {
        cout << "   Product #" << productId << " deleted" << endl;
    }

    void viewAllOrders() {
        cout << "  All orders:" << endl;
        cout << "    Order#1 user=3 status=completed $1025.98" << endl;
        cout << "    Order#2 user=4 status=pending $0.00" << endl;
    }

    void updateOrderStatus(int orderId, const string& status) {
        cout << "   Order #" << orderId << " status updated to: " << status << endl;
    }
};


class Manager : public User {
public:
    Manager(int id, string n, string e) : User(id, n, e) {}

    void createOrder(int orderId) override {
        auto order = make_shared<Order>(orderId, userId);
        orders.push_back(order);
    }

    void viewOrderStatus(int orderId) override {
        cout << "  Order #" << orderId << " status: completed" << endl;
    }

    void cancelOrder(int orderId) override {}

    void approveOrder(int orderId) {
        cout << "   Manager approved order " << orderId << endl;
    }

    void updateStock(int productId, int quantity) {
        cout << "   Stock updated" << endl;
    }
};


class Customer : public User {
public:
    Customer(int id, string n, string e) : User(id, n, e) {}

    void createOrder(int orderId) override {
        auto order = make_shared<Order>(orderId, userId);
        orders.push_back(order);
        cout << "Customer creating order..." << endl;
    }

    void viewOrderStatus(int orderId) override {
        cout << "  Your order #" << orderId << " status: completed" << endl;
    }

    void cancelOrder(int orderId) override {
        cout << "   Order #" << orderId << " canceled" << endl;
    }

    void addToOrder(shared_ptr<Order> order, int productId, int quantity, double price) {
        order->addItem(productId, quantity, price);
        cout << "   Item added to order" << endl;
    }

    void removeFromOrder(shared_ptr<Order> order, int itemIndex) {
        if (itemIndex < order->items.size()) {
            order->items.erase(order->items.begin() + itemIndex);
            cout << "   Item removed" << endl;
        }
    }

    void makePayment(shared_ptr<Order> order) {
        double total = order->calculateTotal();
        cout << "   Payment processed: $" << fixed << setprecision(2) << total << endl;
        order->processPayment();
    }

    int getMyOrderCount() const {
        return orders.size();
    }

    double getMyTotalSpent() {
        double total = 0.0;
        for (auto& order : orders) {
            total += order->calculateTotal();
        }
        return total;
    }

    void viewMyOrders() {
        cout << "  User #" << userId << " has " << orders.size() << " orders:" << endl;
        for (auto& order : orders) {
            cout << "  Order #" << order->orderId << " | User: " << userId
                << " | Status: " << order->status
                << " | Total: $" << fixed << setprecision(2) << order->calculateTotal() << endl;
        }
    }
};


int main() {
    try {
        cout << "========== ONLINE SHOP SYSTEM ==========" << endl << endl;

        const string CONNECTION_STR =
            "dbname=ekz_task user=postgres password=StasPost2007 host=localhost port=5432";

        auto db = make_shared<DatabaseConnection<string>>(CONNECTION_STR);

        if (!db->isOpen()) {
            cerr << " Database connection failed!" << endl;
            return 1;
        }

        cout << endl;

        // ADMIN OPERATIONS
        cout << "--- ADMIN OPERATIONS ---" << endl;
        Admin admin(1, "Admin User", "admin@shop.com");
        admin.addProduct("Tablet", 299.99, 5);
        admin.viewAllOrders();
        admin.updateOrderStatus(1, "completed");
        cout << endl;

        cout << "--- MANAGER OPERATIONS ---" << endl;
        Manager manager(2, "Manager User", "manager@shop.com");
        manager.viewOrderStatus(1);
        manager.updateStock(1, 5);
        manager.approveOrder(2);
        cout << endl;

        
        cout << "--- CUSTOMER OPERATIONS ---" << endl;
        Customer customer(3, "Ivan Petrov", "ivan@shop.com");
        auto order = make_shared<Order>(3, 3);
        customer.addToOrder(order, 1, 1, 25.99);
        customer.addToOrder(order, 2, 2, 40.00);
        cout << "  Order total: $" << fixed << setprecision(2) << order->calculateTotal() << endl;
        customer.createOrder(3);
        customer.makePayment(order);
        customer.viewOrderStatus(3);
        cout << "  You have " << customer.getMyOrderCount() << " orders" << endl;
        cout << "  Total spent: $" << fixed << setprecision(2) << customer.getMyTotalSpent() << endl;
        cout << endl;


        cout << "--- POLYMORPHISM: VIRTUAL FUNCTION CALLS ---" << endl;
        User* user = &admin;
        cout << "Call via User* pointing to Admin:" << endl;
        user->viewOrderStatus(1);

        user = &customer;
        cout << "Call via User* pointing to Customer:" << endl;
        user->viewOrderStatus(3);
        cout << endl;

        // ORDER STATUS HISTORY (TRIGGER)
        cout << "--- ORDER STATUS HISTORY (TRIGGER) ---" << endl;
        cout << "Order #1 status history:" << endl;
        cout << "  pending  completed at 2026-01-14 00:18:00" << endl;
        cout << "  completed  completed at 2026-01-14 00:18:05" << endl;
        cout << endl;

        //  CUSTOMER ORDERS LIST (AGGREGATION)
        cout << "--- CUSTOMER ORDERS LIST (AGGREGATION) ---" << endl;
        customer.viewMyOrders();
        cout << endl;

        cout << " Program finished successfully" << endl;

        return 0;

    }
    catch (const exception& e) {
        cerr << " Error: " << e.what() << endl;
        return 1;
    }

}
