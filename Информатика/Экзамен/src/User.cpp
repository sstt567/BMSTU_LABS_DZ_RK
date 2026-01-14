#include "User.h"
#include "Order.h"
#include "Payment.h"
#include "ReportGenerator.h"
#include <iostream>
#include <algorithm>
#include <numeric>

static void printRows(const std::vector<std::vector<std::string>>& rows) {
    for (auto const& r : rows) {
        for (size_t i = 0; i < r.size(); ++i) {
            std::cout << r[i] << (i + 1 < r.size() ? " | " : "");
        }
        std::cout << "\n";
    }
}

void Admin::menu(DatabaseConnection<std::string>& db) {
    for (;;) {
        std::cout << "\nADMIN: 1)add product 2)all orders 3)set status 4)audit 5)csv 0)back\n> ";
        int c{};
        std::cin >> c;
        if (c == 0) return;

        if (c == 1) {
            std::string n; double p; int q;
            std::cout << "name price qty: ";
            std::cin >> n >> p >> q;
            pqxx::work tx(db.raw());
            tx.exec_params("INSERT INTO products(name,price,stock_quantity) VALUES($1,$2,$3)", n, p, q);
            tx.commit();
        } else if (c == 2) {
            printRows(db.executeQuery("SELECT order_id,user_id,status,total_price FROM orders ORDER BY order_id"));
        } else if (c == 3) {
            int oid; std::string st;
            std::cout << "order_id new_status(pending/completed/canceled/returned): ";
            std::cin >> oid >> st;
            pqxx::work tx(db.raw());
            tx.exec_params("SELECT update_order_status($1,$2,$3)", oid, st, id);
            tx.commit();
        } else if (c == 4) {
            printRows(db.executeQuery("SELECT log_id,entity_type,entity_id,operation,performed_by,performed_at FROM audit_log ORDER BY log_id DESC LIMIT 30"));
        } else if (c == 5) {
            ReportGenerator::exportOrdersCsv(db, "reports/audit_report.csv");
            std::cout << "saved: reports/audit_report.csv\n";
        }
    }
}

void Manager::menu(DatabaseConnection<std::string>& db) {
    for (;;) {
        std::cout << "\nMANAGER: 1)pending 2)approve 3)stock 0)back\n> ";
        int c{};
        std::cin >> c;
        if (c == 0) return;

        if (c == 1) {
            printRows(db.executeQuery("SELECT order_id,user_id,total_price FROM orders WHERE status='pending' ORDER BY order_id"));
        } else if (c == 2) {
            int oid; std::cout << "order_id: "; std::cin >> oid;
            pqxx::work tx(db.raw());
            tx.exec_params("SELECT update_order_status($1,'completed',$2)", oid, id);
            tx.commit();
        } else if (c == 3) {
            int pid, q; std::cout << "product_id new_qty: "; std::cin >> pid >> q;
            pqxx::work tx(db.raw());
            tx.exec_params("UPDATE products SET stock_quantity=$1 WHERE product_id=$2", q, pid);
            tx.commit();
        }
    }
}

void Customer::menu(DatabaseConnection<std::string>& db) {
    int currentOrder = 0;

    for (;;) {
        std::cout << "\nCUSTOMER: 1)new 2)add 3)my 4)status 5)pay 6)return 7)stl 0)back\n> ";
        int c{};
        std::cin >> c;
        if (c == 0) return;

        if (c == 1) {
            currentOrder = OrderService::createEmptyOrder(db, id);
            std::cout << "order_id=" << currentOrder << "\n";
        } else if (c == 2) {
            int pid, qty;
            std::cout << "product_id qty: ";
            std::cin >> pid >> qty;
            if (!currentOrder) std::cout << "create order first\n";
            else OrderService::addItem(db, currentOrder, pid, qty);
        } else if (c == 3) {
            printRows(db.executeQuery("SELECT order_id,status,total_price,order_date FROM orders WHERE user_id=" + std::to_string(id) + " ORDER BY order_id"));
        } else if (c == 4) {
            int oid; std::cout << "order_id: "; std::cin >> oid;
            std::cout << OrderService::getStatus(db, oid) << "\n";
        } else if (c == 5) {
            int oid; std::cout << "order_id: "; std::cin >> oid;
            std::cout << "payment 1)card 2)wallet 3)sbp: ";
            int p; std::cin >> p;
            std::unique_ptr<Payment> pay;
            if (p == 1) pay = std::make_unique<CardPayment>();
            else if (p == 2) pay = std::make_unique<WalletPayment>();
            else pay = std::make_unique<SBPPayment>();
            OrderService::payOrder(db, oid, std::move(pay));
        } else if (c == 6) {
            int oid; std::cout << "order_id: "; std::cin >> oid;
            OrderService::requestReturn(db, oid, id);
        } else if (c == 7) {
            auto rows = db.executeQuery("SELECT total_price FROM orders WHERE user_id=" + std::to_string(id));
            std::vector<double> totals;
            totals.reserve(rows.size());
            for (auto& r : rows) totals.push_back(std::stod(r[0]));

            auto it = std::find_if(totals.begin(), totals.end(), [](double v){ return v > 100.0; });
            std::vector<double> big;
            std::copy_if(totals.begin(), totals.end(), std::back_inserter(big), [](double v){ return v > 100.0; });
            double sum = std::accumulate(totals.begin(), totals.end(), 0.0);

            std::cout << "find_if>100: " << (it==totals.end()?0:1) << "\n";
            std::cout << "copy_if count>100: " << big.size() << "\n";
            std::cout << "accumulate sum: " << sum << "\n";
        }
    }
}
