#pragma once

#include <string>

class Order
{
public:
    Order(const std::string& ordId, const std::string& secId,
          const std::string& side, int qty, const std::string& user,
          const std::string& company);

    std::string getOrderId() const;
    std::string getSecurityId() const;
    std::string getSide() const;
    std::string getUser() const;
    std::string getCompany() const;
    int getQty() const;

    void matchQty(int qty);

    int leftToMatchQty() const;

    void resetMatchedQty();

    bool isValid() const;

    bool isFullyMatched() const;

    bool isBuy() const;

private:
    std::string orderId_;
    std::string securityId_;
    std::string side_;
    int qty_;
    std::string user_;
    std::string company_;
    int leftToMatchQty_;
    bool isBuy_;
};
