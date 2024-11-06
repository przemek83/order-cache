#pragma once

#include <string>

class Order
{
public:
    Order(const std::string& ordId, const std::string& secId,
          const std::string& side, const unsigned int qty,
          const std::string& user, const std::string& company);

    std::string orderId() const;
    std::string securityId() const;
    std::string side() const;
    std::string user() const;
    std::string company() const;
    unsigned int qty() const;

    void matchQty(unsigned int qty);

    unsigned int leftToMatchQty() const;

    void resetMatchedQty();

    bool isValid() const;

    bool isFullyMatched() const;

    bool isBuy() const;

private:
    std::string m_orderId;     // unique order id
    std::string m_securityId;  // security identifier
    std::string m_side;        // side of the order, eg Buy or Sell
    unsigned int m_qty;        // qty for this order
    std::string m_user;        // user name who owns this order
    std::string m_company;     // company for user

    unsigned int m_leftToMatchQty;
};