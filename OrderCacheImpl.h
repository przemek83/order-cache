#pragma once

#include <functional>
#include <map>
#include <vector>

#include "OrderCacheInterface.h"

class OrderCache : public OrderCacheInterface
{
public:
    void addOrder(Order order) override;
    void cancelOrder(const std::string& orderId) override;
    void cancelOrdersForUser(const std::string& user) override;
    void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId,
                                            unsigned int minQty) override;
    unsigned int getMatchingSizeForSecurity(
        const std::string& securityId) override;
    std::vector<Order> getAllOrders() const override;

private:
    static std::pair<unsigned int, int> matchOpositeOrders(
        std::vector<Order>& oppositeOrders, const Order& order,
        unsigned int fromIndex);

    static void removeOrdersUsingCondition(
        std::vector<Order>& orders,
        const std::function<bool(const Order&)>& condition);

    std::vector<Order>& getBuyOrders(const std::string& securityId);
    std::vector<Order>& getSellOrders(const std::string& securityId);

    std::pair<std::vector<Order>&, std::vector<Order>&> getOrdersForMatching(
        const std::string& securityId);

    static bool removeOrder(std::vector<Order>& orders,
                            const std::string& orderId);

    size_t countOrders() const;

    static void resetMathedQuantities(std::vector<Order>& orders);

    struct BuySellOrders
    {
        std::vector<Order> buyOrders_;
        std::vector<Order> sellOrders_;
    };

    std::map<std::string, BuySellOrders> ordersMap_;
};
