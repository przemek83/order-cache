#pragma once

#include <string>
#include <vector>

class Order
{
public:
    // do not alter signature of this constructor
    Order(const std::string& ordId, const std::string& secId,
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
    std::string orderId() const { return m_orderId; }
    std::string securityId() const { return m_securityId; }
    std::string side() const { return m_side; }
    std::string user() const { return m_user; }
    std::string company() const { return m_company; }
    unsigned int qty() const { return m_qty; }

    void matchQty(unsigned int qty) { m_leftToMatchQty -= qty; }

    unsigned int leftToMatchQty() const { return m_leftToMatchQty; }

    void resetMatchedQty() { m_leftToMatchQty = m_qty; }

    bool isValid() const
    {
        if (m_orderId.empty() || m_securityId.empty() || m_user.empty() ||
            m_company.empty() || m_qty == 0)
            return false;
        return m_side == "Sell" || isBuy();
    }

    bool isFullyMatched() const { return m_leftToMatchQty == 0; }

    bool isBuy() const { return m_side == "Buy"; }

private:
    // use the below to hold the order data
    // do not remove the these member variables
    std::string m_orderId;     // unique order id
    std::string m_securityId;  // security identifier
    std::string m_side;        // side of the order, eg Buy or Sell
    unsigned int m_qty;        // qty for this order
    std::string m_user;        // user name who owns this order
    std::string m_company;     // company for user

    unsigned int m_leftToMatchQty;
};

// Provide an implementation for the OrderCacheInterface interface class.
// Your implementation class should hold all relevant data structures you think
// are needed.
class OrderCacheInterface
{
public:
    virtual ~OrderCacheInterface() = default;

    // implement the 6 methods below, do not alter signatures

    // add order to the cache
    virtual void addOrder(Order order) = 0;

    // remove order with this unique order id from the cache
    virtual void cancelOrder(const std::string& orderId) = 0;

    // remove all orders in the cache for this user
    virtual void cancelOrdersForUser(const std::string& user) = 0;

    // remove all orders in the cache for this security with qty >= minQty
    virtual void cancelOrdersForSecIdWithMinimumQty(
        const std::string& securityId, unsigned int minQty) = 0;

    // return the total qty that can match for the security id
    virtual unsigned int getMatchingSizeForSecurity(
        const std::string& securityId) = 0;

    // return all orders in cache in a vector
    virtual std::vector<Order> getAllOrders() const = 0;
};
