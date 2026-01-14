#include "Menu.h"
#include "User.h"
#include <iostream>
#include <memory>

void Menu::run(DatabaseConnection<std::string>& db) {
    for (;;) {
        std::cout << "1) Admin\n2) Manager\n3) Customer\n0) Exit\n> ";
        int c{};
        std::cin >> c;
        if (c == 0) return;

        std::unique_ptr<User> u;
        if (c == 1) u = std::make_unique<Admin>(1, "Admin");
        else if (c == 2) u = std::make_unique<Manager>(2, "Manager");
        else if (c == 3) u = std::make_unique<Customer>(3, "Customer");
        else continue;

        u->menu(db);
    }
}
