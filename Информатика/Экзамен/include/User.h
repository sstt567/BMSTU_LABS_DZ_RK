#pragma once
#include <string>
#include <memory>
#include "DatabaseConnection.h"

class User {
protected:
    int id{};
    std::string name;
    std::string role;

public:
    User(int id_, std::string n, std::string r) : id(id_), name(std::move(n)), role(std::move(r)) {}
    virtual ~User() = default;

    virtual void menu(DatabaseConnection<std::string>& db) = 0;
    int getId() const { return id; }
};

class Admin final : public User {
public:
    Admin(int id, std::string n) : User(id, std::move(n), "admin") {}
    void menu(DatabaseConnection<std::string>& db) override;
};

class Manager final : public User {
public:
    Manager(int id, std::string n) : User(id, std::move(n), "manager") {}
    void menu(DatabaseConnection<std::string>& db) override;
};

class Customer final : public User {
public:
    Customer(int id, std::string n) : User(id, std::move(n), "customer") {}
    void menu(DatabaseConnection<std::string>& db) override;
};
