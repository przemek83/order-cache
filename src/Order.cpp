#include "Order.h"

Order::Order(const std::string& ordId, const std::string& secId,
             const std::string& side, const unsigned int qty,
             const std::string& user, const std::string& company)
    : orderId_(ordId),
      securityId_(secId),
      side_(side),
      qty_(qty),
      user_(user),
      company_(company),
      m_leftToMatchQty(qty)
{
}

std::string Order::getOrderId() const { return orderId_; }
std::string Order::getSecurityId() const { return securityId_; }
std::string Order::getSide() const { return side_; }
std::string Order::getUser() const { return user_; }
std::string Order::getCompany() const { return company_; }
unsigned int Order::getQty() const { return qty_; }

void Order::matchQty(unsigned int qty) { m_leftToMatchQty -= qty; }

unsigned int Order::leftToMatchQty() const { return m_leftToMatchQty; }

void Order::resetMatchedQty() { m_leftToMatchQty = qty_; }

bool Order::isValid() const
{
    if (orderId_.empty() || securityId_.empty() || user_.empty() ||
        company_.empty() || qty_ == 0)
        return false;
    return side_ == "Sell" || isBuy();
}

bool Order::isFullyMatched() const { return m_leftToMatchQty == 0; }

bool Order::isBuy() const { return side_ == "Buy"; }
