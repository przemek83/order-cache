#pragma once

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
};
