#include "Order.h"

Order::Order(const std::string& ordId, const std::string& secId,
             const std::string& side, int qty, const std::string& user,
             const std::string& company)
    : orderId_(ordId),
      securityId_(secId),
      side_(side),
      qty_(qty),
      user_(user),
      company_(company),
      leftToMatchQty_(qty),
      isBuy_{side_ == "Buy"}
{
}

std::string Order::getOrderId() const { return orderId_; }
std::string Order::getSecurityId() const { return securityId_; }
std::string Order::getSide() const { return side_; }
std::string Order::getUser() const { return user_; }
std::string Order::getCompany() const { return company_; }
int Order::getQty() const { return qty_; }

void Order::matchQty(int qty) { leftToMatchQty_ -= qty; }

int Order::leftToMatchQty() const { return leftToMatchQty_; }

void Order::resetMatchedQty() { leftToMatchQty_ = qty_; }

bool Order::isValid() const
{
    if (const bool detailsEmpty{orderId_.empty() || securityId_.empty() ||
                                user_.empty() || company_.empty()};
        detailsEmpty || (qty_ <= 0))
        return false;
    return (side_ == "Sell") || isBuy();
}

bool Order::isFullyMatched() const { return leftToMatchQty_ == 0; }

bool Order::isBuy() const { return isBuy_; }
