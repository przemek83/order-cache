#include "Order.h"

// do not alter signature of this constructor
Order::Order(const std::string& ordId, const std::string& secId,
             const std::string& side, const unsigned int qty,
             const std::string& user, const std::string& company)
    : m_orderId(ordId),
      m_securityId(secId),
      m_side(side),
      m_qty(qty),
      m_user(user),
      m_company(company),
      m_leftToMatchQty(qty)
{
}

// do not alter these accessor methods
std::string Order::orderId() const { return m_orderId; }
std::string Order::securityId() const { return m_securityId; }
std::string Order::side() const { return m_side; }
std::string Order::user() const { return m_user; }
std::string Order::company() const { return m_company; }
unsigned int Order::qty() const { return m_qty; }

void Order::matchQty(unsigned int qty) { m_leftToMatchQty -= qty; }

unsigned int Order::leftToMatchQty() const { return m_leftToMatchQty; }

void Order::resetMatchedQty() { m_leftToMatchQty = m_qty; }

bool Order::isValid() const
{
    if (m_orderId.empty() || m_securityId.empty() || m_user.empty() ||
        m_company.empty() || m_qty == 0)
        return false;
    return m_side == "Sell" || isBuy();
}

bool Order::isFullyMatched() const { return m_leftToMatchQty == 0; }

bool Order::isBuy() const { return m_side == "Buy"; }
