#pragma once
#include <string>
#include "DatabaseConnection.h"

class Menu {
public:
    static void run(DatabaseConnection<std::string>& db);
};
