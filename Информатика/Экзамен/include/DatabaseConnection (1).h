#pragma once
#include <pqxx/pqxx>
#include <memory>
#include <string>
#include <vector>

template<class T>
class DatabaseConnection {
    std::unique_ptr<pqxx::connection> conn;

public:
    explicit DatabaseConnection(const T& connStr) : conn(std::make_unique<pqxx::connection>(connStr)) {
        if (!conn || !conn->is_open()) throw std::runtime_error("DB connection failed");
    }

    std::vector<std::vector<std::string>> executeQuery(const std::string& sql) {
        pqxx::work tx(*conn);
        pqxx::result r = tx.exec(sql);
        std::vector<std::vector<std::string>> out;
        out.reserve(r.size());
        for (auto const& row : r) {
            std::vector<std::string> line;
            line.reserve(row.size());
            for (auto const& f : row) line.push_back(f.is_null() ? "" : f.c_str());
            out.push_back(std::move(line));
        }
        tx.commit();
        return out;
    }

    void executeNonQuery(const std::string& sql) {
        pqxx::work tx(*conn);
        tx.exec(sql);
        tx.commit();
    }

    pqxx::connection& raw() { return *conn; }
};
