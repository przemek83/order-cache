#include "OrderCache.h"

void OrderCache::addOrder(Order order) {}

void OrderCache::cancelOrder(const std::string& orderId) {}

void OrderCache::cancelOrdersForUser(const std::string& user) {}

void OrderCache::cancelOrdersForSecIdWithMinimumQty(
    const std::string& securityId, unsigned int minQty)
{
}

unsigned int OrderCache::getMatchingSizeForSecurity(
    const std::string& securityId)
{
    return {};
}

std::vector<Order> OrderCache::getAllOrders() const { return {}; }
