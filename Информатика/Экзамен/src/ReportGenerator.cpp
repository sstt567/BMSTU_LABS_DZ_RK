#include "ReportGenerator.h"
#include <fstream>

void ReportGenerator::exportOrdersCsv(DatabaseConnection<std::string>& db, const std::string& path) {
    auto rows = db.executeQuery(
        "SELECT o.order_id,u.name,o.status,o.total_price,o.order_date "
        "FROM orders o JOIN users u ON u.user_id=o.user_id ORDER BY o.order_id"
    );

    std::ofstream out(path);
    out << "order_id,user,status,total,order_date\n";
    for (auto& r : rows) {
        out << r[0] << "," << r[1] << "," << r[2] << "," << r[3] << "," << r[4] << "\n";
    }
}
