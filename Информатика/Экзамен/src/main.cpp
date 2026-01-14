#include <iostream>
#include "DatabaseConnection.h"
#include "Menu.h"

int main() {
    try {
        DatabaseConnection<std::string> db("host=localhost user=postgres password=password dbname=ekz_task");
        std::cout << "========== ОНЛАЙН МАГАЗИН ==========" << std::endl;
        Menu::run(db);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}
