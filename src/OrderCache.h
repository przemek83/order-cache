#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Order.h"

class OrderCache
{
public:
    void addOrder(Order order);
    void cancelOrder(const std::string& orderId);
    void cancelOrdersForUser(const std::string& user);
    void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId,
                                            int minQty);
    int getMatchingSizeForSecurity(const std::string& securityId);
    std::vector<Order> getAllOrders() const;

private:
    static std::pair<int, std::size_t> matchOpositeOrders(
        std::vector<Order>& oppositeOrders, const Order& order,
        std::size_t fromIndex);

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

    std::map<std::string, BuySellOrders, std::less<>> ordersMap_;
};
