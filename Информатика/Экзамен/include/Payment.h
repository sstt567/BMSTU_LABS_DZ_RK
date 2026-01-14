#pragma once
#include <string>

class Payment {
public:
    virtual ~Payment() = default;
    virtual std::string name() const = 0;
    virtual bool pay(double amount) = 0;
};

class CardPayment final : public Payment {
public:
    std::string name() const override { return "card"; }
    bool pay(double) override { return true; }
};

class WalletPayment final : public Payment {
public:
    std::string name() const override { return "wallet"; }
    bool pay(double) override { return true; }
};

class SBPPayment final : public Payment {
public:
    std::string name() const override { return "sbp"; }
    bool pay(double) override { return true; }
};
