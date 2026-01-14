#pragma once
#include <string>
#include "DatabaseConnection.h"

class ReportGenerator {
public:
    static void exportOrdersCsv(DatabaseConnection<std::string>& db, const std::string& path);
};
