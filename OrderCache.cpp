#include "OrderCache.h"

#include <algorithm>
#include <stdexcept>

void OrderCache::addOrder(Order order)
{
    // TODO order validate
    if (!orderIsUnique(order.orderId()))
        throw std::logic_error("Duplicated id while adding order: " +
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
    unsigned int matchingSize{0};
    for (size_t i = 0; i < orders_.size(); ++i)
    {
        if (orders_[i].securityId() != securityId)
            continue;

        const std::string& outerCompany{orders_[i].company()};
        const std::string& outerSide{orders_[i].side()};

        for (size_t j = 0; j < orders_.size(); ++j)
        {
            if (orders_[j].securityId() != securityId ||
                orders_[j].company() == outerCompany ||
                orders_[j].side() == outerSide)
                continue;

            const unsigned int outerQty{orders_[i].qty()};
            const unsigned int innerQty{orders_[j].qty()};
            if (outerQty == innerQty)
            {
                orders_.erase(orders_.begin() + i);
                orders_.erase(orders_.begin() + j);
                matchingSize += outerQty;
            }
            else
            {
                if (outerQty > innerQty)
                {
                    matchingSize += innerQty;
                    orders_[i].setQty(orders_[i].qty() - innerQty);
                    orders_.erase(orders_.begin() + j);
                }
                else
                {
                    matchingSize += outerQty;
                    orders_[j].setQty(orders_[j].qty() - outerQty);
                    orders_.erase(orders_.begin() + i);
                }
            }
            --i;
            break;
        }
    }

    return matchingSize;
}

std::vector<Order> OrderCache::getAllOrders() const { return orders_; }

bool OrderCache::orderIsUnique(const std::string& orderId) const
{
    const auto findResult{std::find_if(orders_.cbegin(), orders_.cend(),
                                       [&id = orderId](const auto& order)
                                       { return order.orderId() == id; })};
    return findResult == orders_.cend();
}
