#pragma once

#include <functional>
#include <list>
#include <map>

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
    static unsigned int matchSellOrders(std::list<Order>& sellOrders,
                                        std::list<Order>::iterator buyIt);

    static void removeOrdersUsingCondition(
        std::list<Order>& orders,
        const std::function<bool(const Order&)>& condition);

    std::list<Order>& getBuyOrders(const std::string& securityId);
    std::list<Order>& getSellOrders(const std::string& securityId);

    static bool removeOrder(std::list<Order>& orders,
                            const std::string& orderId);

    struct BuySellOrders
    {
        std::list<Order> buyOrders_;
        std::list<Order> sellOrders_;
    };

    std::map<std::string, BuySellOrders> ordersMap_;
};
