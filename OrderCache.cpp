#include "OrderCache.h"

#include <algorithm>
#include <stdexcept>

void OrderCache::addOrder(Order order)
{
    if (!order.isValid())
        throw std::logic_error("Invalid order: " + order.orderId());

    const std::string& securityId{order.securityId()};
    std::list<Order>& orders{order.isBuy() ? getBuyOrders(securityId)
                                           : getSellOrders(securityId)};
    orders.emplace_back(std::move(order));
}

void OrderCache::cancelOrder(const std::string& orderId)
{
    for (auto& [secId, buySellOrders] : ordersMap_)
    {
        auto& buyOrders{buySellOrders.buyOrders_};
        auto& sellOrders{buySellOrders.sellOrders_};
        if (removeOrder(buyOrders, orderId) || removeOrder(sellOrders, orderId))
            return;
    }
}

void OrderCache::cancelOrdersForUser(const std::string& user)
{
    const std::function<bool(const Order&)> condition{
        [&userId = user](const auto& order) { return order.user() == userId; }};

    for (auto& [secId, buySellOrders] : ordersMap_)
    {
        removeOrdersUsingCondition(buySellOrders.buyOrders_, condition);
        removeOrdersUsingCondition(buySellOrders.sellOrders_, condition);
    }
}

void OrderCache::cancelOrdersForSecIdWithMinimumQty(
    const std::string& securityId, unsigned int minQty)
{
    if (ordersMap_.find(securityId) == ordersMap_.end())
        return;

    const std::function<bool(const Order&)> condition{
        [&id = securityId, qty = minQty](const auto& order)
        { return order.securityId() == id && order.qty() >= qty; }};

    removeOrdersUsingCondition(getBuyOrders(securityId), condition);
    removeOrdersUsingCondition(getSellOrders(securityId), condition);
}

unsigned int OrderCache::getMatchingSizeForSecurity(
    const std::string& securityId)
{
    if (ordersMap_.find(securityId) == ordersMap_.end())
        return 0;

    unsigned int matchedSum{0};

    auto& buyOrders{getBuyOrders(securityId)};
    auto& sellOrders{getSellOrders(securityId)};

    std::for_each(sellOrders.begin(), sellOrders.end(),
                  [](auto& order) { order.resetMatchedQty(); });

    auto buyIt{buyOrders.begin()};
    while (buyIt != buyOrders.end())
    {
        buyIt->resetMatchedQty();
        const unsigned int matchedSize{matchSellOrders(sellOrders, buyIt)};
        matchedSum += matchedSize;
        buyIt++;
    }

    return matchedSum;
}

std::vector<Order> OrderCache::getAllOrders() const
{
    std::vector<Order> allOrders;
    for (const auto& [secId, buySellOrders] : ordersMap_)
    {
        const auto& sellOrders{buySellOrders.sellOrders_};
        allOrders.insert(allOrders.end(), sellOrders.cbegin(),
                         sellOrders.cend());
        const auto& buyOrders{buySellOrders.buyOrders_};
        allOrders.insert(allOrders.end(), buyOrders.cbegin(), buyOrders.cend());
    }

    return allOrders;
}

unsigned int OrderCache::matchSellOrders(std::list<Order>& sellOrders,
                                         std::list<Order>::iterator buyIt)
{
    unsigned int matchedSum{0};
    const std::string& buyCompany{buyIt->company()};

    auto sellIt{sellOrders.begin()};
    while (sellIt != sellOrders.end() && buyIt->qty() > matchedSum)
    {
        if (sellIt->company() != buyCompany && !sellIt->isFullyMatched())
        {
            const unsigned int matchedQty{
                std::min(buyIt->qty() - matchedSum, sellIt->leftToMatchQty())};
            sellIt->matchQty(matchedQty);
            matchedSum += matchedQty;
        }

        sellIt++;
    }

    return matchedSum;
}

void OrderCache::removeOrdersUsingCondition(
    std::list<Order>& orders,
    const std::function<bool(const Order&)>& condition)
{
    orders.erase(std::remove_if(orders.begin(), orders.end(), condition),
                 orders.end());
}

std::list<Order>& OrderCache::getBuyOrders(const std::string& securityId)
{
    auto& ordersForSecurity{ordersMap_[securityId]};
    return ordersForSecurity.buyOrders_;
}

std::list<Order>& OrderCache::getSellOrders(const std::string& securityId)
{
    auto& ordersForSecurity{ordersMap_[securityId]};
    return ordersForSecurity.sellOrders_;
}

bool OrderCache::removeOrder(std::list<Order>& orders,
                             const std::string& orderId)
{
    const auto condition{[&id = orderId](const auto& order)
                         { return order.orderId() == id; }};

    auto it{std::find_if(orders.begin(), orders.end(), condition)};
    if (it != orders.end())
    {
        orders.erase(it);
        return true;
    }

    return false;
}
