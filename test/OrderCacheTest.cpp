#include <catch2/catch_test_macros.hpp>

#include "../OrderCache.h"

bool operator==(const Order& left, const Order& right)
{
    return left.orderId() == right.orderId() &&
           left.company() == right.company() && left.qty() == right.qty() &&
           left.securityId() == right.securityId() &&
           left.side() == right.side() && left.user() == right.user();
}

std::ostream& operator<<(std::ostream& os, const Order& order)
{
    os << "[" << order.orderId() << "," << order.securityId() << ","
       << order.side() << "," << order.user() << "," << order.company() << ","
       << order.qty() << "]";
    return os;
}

TEST_CASE("Adding order", "[orders]")
{
    OrderCache cache;

    SECTION("empty") { REQUIRE(cache.getAllOrders().empty()); }

    SECTION("single add")
    {
        const Order order{"order1", "sec1", "Buy", 10000, "user1", "company1"};
        cache.addOrder(order);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order});
    }

    SECTION("multiple add")
    {
        const Order order1{"ord1", "sec1", "Buy", 10000, "user1", "company1"};
        const Order order2{"ord2", "sec1", "Sell", 2000, "user1", "company2"};
        const Order order3{"ord3", "sec1", "Sell", 1000, "user1", "company2"};

        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.addOrder(order3);
        REQUIRE(cache.getAllOrders() ==
                std::vector<Order>{order1, order2, order3});
    }

    SECTION("adding same order twice")
    {
        const Order order{"order1", "sec1", "Buy", 10000, "user1", "company1"};
        cache.addOrder(order);
        REQUIRE_THROWS(cache.addOrder(order));
    }

    SECTION("adding similar order with different order id - no throw")
    {
        const Order order{"order1", "sec1", "Buy", 10000, "user1", "company1"};
        cache.addOrder(order);
        Order similarOrder{"order2",    order.securityId(), order.side(),
                           order.qty(), order.user(),       order.company()};
        REQUIRE_NOTHROW(cache.addOrder(order));
    }

    SECTION("adding similar order with different order id - check orders")
    {
        const Order order{"order1", "sec1", "Buy", 10000, "user1", "company1"};
        cache.addOrder(order);
        Order similarOrder{"order2",    order.securityId(), order.side(),
                           order.qty(), order.user(),       order.company()};
        cache.addOrder(order);
        REQUIRE(cache.getAllOrders() ==
                std::vector<Order>{order, similarOrder});
    }
}

TEST_CASE("cancel order", "[orders]")
{
    OrderCache cache;
    const std::string orderIdToCancel{"order1"};

    SECTION("cancel empty")
    {
        cache.cancelOrder(orderIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{});
    }

    SECTION("cancel not existing")
    {
        const Order order{"order2", "sec1", "Buy", 10000, "user1", "company1"};
        cache.addOrder(order);
        cache.cancelOrder(orderIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order});
    }

    SECTION("cancel existing")
    {
        const Order order1{orderIdToCancel, "sec1",  "Buy",
                           10000,           "user1", "company1"};
        const Order order2{"order2", "sec1", "Buy", 10000, "user1", "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.cancelOrder(orderIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order2});
    }

    SECTION("cancel existing twice")
    {
        const Order order1{orderIdToCancel, "sec1",  "Buy",
                           10000,           "user1", "company1"};
        const Order order2{"order2", "sec1", "Buy", 10000, "user1", "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.cancelOrder(orderIdToCancel);
        cache.cancelOrder(orderIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order2});
    }
}

TEST_CASE("cancel orders for user", "[orders]")
{
    OrderCache cache;
    const std::string userIdToCancel{"user1"};

    SECTION("cancel empty")
    {
        cache.cancelOrdersForUser(userIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{});
    }

    SECTION("cancel not existing")
    {
        const Order order{"order1", "sec1", "Buy", 10000, "user2", "company1"};
        cache.addOrder(order);
        cache.cancelOrdersForUser(userIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order});
    }

    SECTION("cancel for existing")
    {
        const Order order1{"order1", "sec1",         "Buy",
                           10000,    userIdToCancel, "company1"};
        const Order order2{"order2", "sec1", "Buy", 10000, "user2", "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.cancelOrdersForUser(userIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order2});
    }

    SECTION("cancel existing twice")
    {
        const Order order1{"order1", "sec1",         "Buy",
                           10000,    userIdToCancel, "company1"};
        const Order order2{"order2", "sec1", "Buy", 10000, "user2", "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.cancelOrdersForUser(userIdToCancel);
        cache.cancelOrdersForUser(userIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order2});
    }

    SECTION("cancel multiple")
    {
        const Order order1{"order1", "sec1",         "Buy",
                           10000,    userIdToCancel, "company1"};
        const Order order2{"order2", "sec1",         "Buy",
                           10000,    userIdToCancel, "company1"};
        const Order order3{"order3", "sec1", "Buy", 10000, "user2", "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.addOrder(order3);
        cache.cancelOrdersForUser(userIdToCancel);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order3});
    }
}

TEST_CASE("cancel orders for security with minimum quantity", "[orders]")
{
    OrderCache cache;
    const std::string securityIdToCancel{"sec1"};
    const unsigned int minQty{1000};

    SECTION("cancel empty")
    {
        cache.cancelOrdersForSecIdWithMinimumQty(securityIdToCancel, minQty);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{});
    }

    SECTION("cancel not existing security")
    {
        const Order order{"order1", "sec2", "Buy", minQty, "user1", "company1"};
        cache.addOrder(order);
        cache.cancelOrdersForSecIdWithMinimumQty(securityIdToCancel, minQty);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order});
    }

    SECTION("cancel not existing quantity")
    {
        const Order order{"order1", securityIdToCancel, "Buy", minQty,
                          "user1",  "company1"};
        cache.addOrder(order);
        cache.cancelOrdersForSecIdWithMinimumQty(securityIdToCancel,
                                                 minQty + 1);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order});
    }

    SECTION("cancel for existing")
    {
        const Order order1{"order1", securityIdToCancel, "Buy", minQty,
                           "user1",  "company1"};
        const Order order2{"order2", "sec2",  "Buy",
                           minQty,   "user1", "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.cancelOrdersForSecIdWithMinimumQty(securityIdToCancel, minQty);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order2});
    }

    SECTION("cancel existing twice")
    {
        const Order order1{"order1", securityIdToCancel, "Buy", minQty,
                           "user1",  "company1"};
        const Order order2{"order2", "sec2",  "Buy",
                           minQty,   "user1", "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.cancelOrdersForSecIdWithMinimumQty(securityIdToCancel, minQty);
        cache.cancelOrdersForSecIdWithMinimumQty(securityIdToCancel, minQty);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order2});
    }

    SECTION("cancel multiple")
    {
        const Order order1{"order1", securityIdToCancel, "Buy", minQty,
                           "user1",  "company1"};
        const Order order2{"order2", securityIdToCancel, "Buy", minQty,
                           "user1",  "company1"};
        const Order order3{"order3", "sec1",  "Buy",
                           minQty,   "user1", "company1"};
        const Order order4{"order4", securityIdToCancel, "Buy", minQty - 1,
                           "user1",  "company1"};
        cache.addOrder(order1);
        cache.addOrder(order2);
        cache.addOrder(order3);
        cache.addOrder(order4);
        cache.cancelOrdersForSecIdWithMinimumQty(securityIdToCancel, minQty);
        REQUIRE(cache.getAllOrders() == std::vector<Order>{order3, order4});
    }
}

TEST_CASE("matching size for security", "[orders]")
{
    OrderCache cache;

    SECTION("match on empty cache")
    {
        REQUIRE(cache.getMatchingSizeForSecurity("sec1") == 0);
    }

    SECTION("no match")
    {
        cache.addOrder({"ord1", "sec1", "Buy", 10000, "user1", "company1"});
        cache.addOrder({"ord2", "sec1", "Sell", 2000, "user1", "company2"});
        cache.addOrder({"ord3", "sec1", "Sell", 1000, "user1", "company2"});
        REQUIRE(cache.getMatchingSizeForSecurity("sec2") == 0);
    }

    SECTION("single match")
    {
        cache.addOrder({"ord1", "sec1", "Buy", 10000, "user1", "company1"});
        cache.addOrder({"ord2", "sec1", "Sell", 2000, "user1", "company2"});
        REQUIRE(cache.getMatchingSizeForSecurity("sec1") == 2000);
    }

    SECTION("multiple matches")
    {
        cache.addOrder({"ord1", "sec1", "Buy", 10000, "user1", "company1"});
        cache.addOrder({"ord2", "sec1", "Sell", 2000, "user1", "company2"});
        cache.addOrder({"ord3", "sec1", "Sell", 1000, "user1", "company2"});
        REQUIRE(cache.getMatchingSizeForSecurity("sec1") == 3000);
    }

    SECTION("cannot reuse matching size")
    {
        cache.addOrder({"ord1", "sec1", "Buy", 10000, "user1", "company1"});
        cache.addOrder({"ord2", "sec1", "Sell", 2000, "user1", "company2"});
        cache.addOrder({"ord3", "sec1", "Sell", 1000, "user1", "company2"});
        cache.getMatchingSizeForSecurity("sec1");
        REQUIRE(cache.getMatchingSizeForSecurity("sec1") == 0);
    }

    SECTION("matching multiple securities #1")
    {
        cache.addOrder({"OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"});
        cache.addOrder({"OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB"});
        cache.addOrder({"OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA"});
        cache.addOrder({"OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC"});
        cache.addOrder({"OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB"});
        cache.addOrder({"OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD"});
        cache.addOrder({"OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE"});
        cache.addOrder({"OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE"});

        REQUIRE(cache.getMatchingSizeForSecurity("SecId1") == 0);
        REQUIRE(cache.getMatchingSizeForSecurity("SecId2") == 2700);
        REQUIRE(cache.getMatchingSizeForSecurity("SecId3") == 0);
    }

    SECTION("matching multiple securities #2")
    {
        cache.addOrder({"OrdId1", "SecId1", "Sell", 100, "User10", "Company2"});
        cache.addOrder({"OrdId2", "SecId3", "Sell", 200, "User8", "Company2"});
        cache.addOrder({"OrdId3", "SecId1", "Buy", 300, "User13", "Company2"});
        cache.addOrder({"OrdId4", "SecId2", "Sell", 400, "User12", "Company2"});
        cache.addOrder({"OrdId5", "SecId3", "Sell", 500, "User7", "Company2"});
        cache.addOrder({"OrdId6", "SecId3", "Buy", 600, "User3", "Company1"});
        cache.addOrder({"OrdId7", "SecId1", "Sell", 700, "User10", "Company2"});
        cache.addOrder({"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
        cache.addOrder({"OrdId9", "SecId2", "Buy", 900, "User6", "Company2"});
        cache.addOrder(
            {"OrdId10", "SecId2", "Sell", 1000, "User5", "Company1"});
        cache.addOrder(
            {"OrdId11", "SecId1", "Sell", 1100, "User13", "Company2"});
        cache.addOrder({"OrdId12", "SecId2", "Buy", 1200, "User9", "Company2"});
        cache.addOrder({"OrdId13", "SecId1", "Sell", 1300, "User1", "Company"});

        REQUIRE(cache.getMatchingSizeForSecurity("SecId1") == 300);
        REQUIRE(cache.getMatchingSizeForSecurity("SecId2") == 100);
        REQUIRE(cache.getMatchingSizeForSecurity("SecId3") == 600);
    }

    SECTION("matching multiple securities #3")
    {
        cache.addOrder({"OrdId1", "SecId3", "Sell", 100, "User1", "Company1"});
        cache.addOrder({"OrdId2", "SecId3", "Sell", 200, "User3", "Company2"});
        cache.addOrder({"OrdId3", "SecId1", "Buy", 300, "User2", "Company1"});
        cache.addOrder({"OrdId4", "SecId3", "Sell", 400, "User5", "Company2"});
        cache.addOrder({"OrdId5", "SecId2", "Sell", 500, "User2", "Company1"});
        cache.addOrder({"OrdId6", "SecId2", "Buy", 600, "User3", "Company2"});
        cache.addOrder({"OrdId7", "SecId2", "Sell", 700, "User1", "Company1"});
        cache.addOrder({"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
        cache.addOrder({"OrdId9", "SecId1", "Buy", 900, "User5", "Company2"});
        cache.addOrder(
            {"OrdId10", "SecId1", "Sell", 1000, "User1", "Company1"});
        cache.addOrder(
            {"OrdId11", "SecId2", "Sell", 1100, "User6", "Company2"});

        REQUIRE(cache.getMatchingSizeForSecurity("SecId1") == 900);
        REQUIRE(cache.getMatchingSizeForSecurity("SecId2") == 600);
        REQUIRE(cache.getMatchingSizeForSecurity("SecId3") == 0);
    }
}
