#include "OrderCache.h"

#include <algorithm>
#include <stdexcept>

void OrderCache::addOrder(Order order)
{
    if (!orderIsUnique(order.orderId()))
        throw std::logic_error("Duplicated order id while adding order: " +
                               order.orderId());
    orders_.emplace_back(std::move(order));
}

void OrderCache::cancelOrder(const std::string& orderId)
{
    const auto predicate{[&id = orderId](const auto& order)
                         { return order.orderId() == id; }};
    orders_.erase(std::remove_if(orders_.begin(), orders_.end(), predicate),
                  orders_.end());
}

void OrderCache::cancelOrdersForUser(const std::string& user)
{
    const auto predicate{[&userId = user](const auto& order)
                         { return order.user() == userId; }};
    orders_.erase(std::remove_if(orders_.begin(), orders_.end(), predicate),
                  orders_.end());
}

void OrderCache::cancelOrdersForSecIdWithMinimumQty(
    const std::string& securityId, unsigned int minQty)
{
    const auto predicate{[&id = securityId, qty = minQty](const auto& order) {
        return order.securityId() == id && order.qty() >= qty;
    }};
    orders_.erase(std::remove_if(orders_.begin(), orders_.end(), predicate),
                  orders_.end());
}

unsigned int OrderCache::getMatchingSizeForSecurity(
    const std::string& securityId)
{
    return {};
}

std::vector<Order> OrderCache::getAllOrders() const { return orders_; }

bool OrderCache::orderIsUnique(const std::string& orderId) const
{
    const auto findResult{std::find_if(orders_.cbegin(), orders_.cend(),
                                       [&id = orderId](const auto& order)
                                       { return order.orderId() == id; })};
    return findResult == orders_.cend();
}
