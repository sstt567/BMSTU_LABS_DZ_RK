#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Payment.h"
#include "DatabaseConnection.h"

struct OrderItem {
    int productId{};
    int qty{};
    double price{};
};

class OrderService {
public:
    static int createEmptyOrder(DatabaseConnection<std::string>& db, int userId);
    static void addItem(DatabaseConnection<std::string>& db, int orderId, int productId, int qty);
    static void payOrder(DatabaseConnection<std::string>& db, int orderId, std::unique_ptr<Payment> p);
    static void requestReturn(DatabaseConnection<std::string>& db, int orderId, int userId);
    static std::string getStatus(DatabaseConnection<std::string>& db, int orderId);
};
