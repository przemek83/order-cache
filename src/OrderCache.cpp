#include "OrderCache.h"

#include "Order.h"

#include <algorithm>
#include <stdexcept>

void OrderCache::addOrder(Order order)
{
    if (!order.isValid())
        throw std::logic_error("Invalid order: " + order.getOrderId());

    const std::string& securityId{order.getSecurityId()};
    std::vector<Order>& orders{order.isBuy() ? getBuyOrders(securityId)
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
        [&userId = user](const auto& order)
        { return order.getUser() == userId; }};

    for (auto& [secId, buySellOrders] : ordersMap_)
    {
        removeOrdersUsingCondition(buySellOrders.buyOrders_, condition);
        removeOrdersUsingCondition(buySellOrders.sellOrders_, condition);
    }
}

void OrderCache::cancelOrdersForSecIdWithMinimumQty(
    const std::string& securityId, int minQty)
{
    if (ordersMap_.find(securityId) == ordersMap_.end())
        return;

    const std::function<bool(const Order&)> condition{
        [&id = securityId, qty = minQty](const auto& order)
        { return order.getSecurityId() == id && order.getQty() >= qty; }};

    removeOrdersUsingCondition(getBuyOrders(securityId), condition);
    removeOrdersUsingCondition(getSellOrders(securityId), condition);
}

int OrderCache::getMatchingSizeForSecurity(const std::string& securityId)
{
    if (ordersMap_.find(securityId) == ordersMap_.end())
        return 0;

    // Take longer vector as orders, shorter as opposite orders. Optimization.
    auto [orders, oppositeOrders]{getOrdersForMatching(securityId)};

    int matchedSize{0};
    // Index used to skip already matched opposite orders. Optimization.
    std::size_t startingOppositeIndex{0};
    for (const auto& order : orders)
    {
        int matchedPartial{0};
        std::tie(matchedPartial, startingOppositeIndex) =
            matchOpositeOrders(oppositeOrders, order, startingOppositeIndex);
        matchedSize += matchedPartial;
    }

    // Reset matched quantities to default values equal qty.
    resetMatchedQuantities(oppositeOrders);

    return matchedSize;
}

std::vector<Order> OrderCache::getAllOrders() const
{
    std::vector<Order> allOrders;
    // Reserve memory. Optimization.
    allOrders.reserve(countOrders());

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

std::pair<int, std::size_t> OrderCache::matchOpositeOrders(
    std::vector<Order>& oppositeOrders, const Order& order,
    std::size_t fromIndex)
{
    int matchedSum{0};
    for (size_t i{fromIndex};
         i < oppositeOrders.size() && order.getQty() > matchedSum; ++i)
    {
        Order& oppositeOrder{oppositeOrders[i]};
        if (oppositeOrder.getCompany() == order.getCompany())
            continue;

        const int matchedQty{std::min(order.getQty() - matchedSum,
                                      oppositeOrder.leftToMatchQty())};
        oppositeOrder.matchQty(matchedQty);
        matchedSum += matchedQty;

        // Move fully matched opposite order and ignore it. Optimization.
        if (i > fromIndex && oppositeOrder.isFullyMatched())
        {
            std::swap(oppositeOrder, oppositeOrders[fromIndex]);
            fromIndex++;
        }
    }

    return {matchedSum, fromIndex};
}

void OrderCache::removeOrdersUsingCondition(
    std::vector<Order>& orders,
    const std::function<bool(const Order&)>& condition)
{
    orders.erase(std::remove_if(orders.begin(), orders.end(), condition),
                 orders.end());
}

std::vector<Order>& OrderCache::getBuyOrders(const std::string& securityId)
{
    auto& ordersForSecurity{ordersMap_[securityId]};
    return ordersForSecurity.buyOrders_;
}

std::vector<Order>& OrderCache::getSellOrders(const std::string& securityId)
{
    auto& ordersForSecurity{ordersMap_[securityId]};
    return ordersForSecurity.sellOrders_;
}

std::pair<std::vector<Order>&, std::vector<Order>&>
OrderCache::getOrdersForMatching(const std::string& securityId)
{
    auto& buyOrders{getBuyOrders(securityId)};
    auto& sellOrders{getSellOrders(securityId)};

    // Take buy orders if there is more buy than sell. Do opposite otherwise.
    auto& orders{buyOrders.size() > sellOrders.size() ? buyOrders : sellOrders};
    auto& oppositeOrders{buyOrders.size() > sellOrders.size() ? sellOrders
                                                              : buyOrders};

    return {orders, oppositeOrders};
}

bool OrderCache::removeOrder(std::vector<Order>& orders,
                             const std::string& orderId)
{
    const auto condition{[&id = orderId](const auto& order)
                         { return order.getOrderId() == id; }};

    if (auto it{std::find_if(orders.begin(), orders.end(), condition)};
        it != orders.end())
    {
        orders.erase(it);
        return true;
    }

    return false;
}

size_t OrderCache::countOrders() const
{
    size_t count{0};
    for (const auto& [secId, secOrders] : ordersMap_)
        count += secOrders.buyOrders_.size() + secOrders.sellOrders_.size();

    return count;
}

void OrderCache::resetMatchedQuantities(std::vector<Order>& orders)
{
    std::for_each(orders.begin(), orders.end(),
                  [](auto& order) { order.resetMatchedQty(); });
}
