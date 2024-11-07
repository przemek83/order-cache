#include <iostream>
#include <set>
#include <sstream>

#include "OrderCache.h"

int main()
{
    std::istringstream input(
        "OrdId1 SecId1 Sell 100 User10 Company2\n"
        "OrdId2 SecId3 Sell 200 User8 Company2\n"
        "OrdId3 SecId1 Buy 300 User13 Company2\n"
        "OrdId4 SecId2 Sell 400 User12 Company2\n"
        "OrdId5 SecId3 Sell 500 User7 Company2\n"
        "OrdId6 SecId3 Buy 600 User3 Company1\n"
        "OrdId7 SecId1 Sell 700 User10 Company2\n"
        "OrdId8 SecId1 Sell 800 User2 Company1\n"
        "OrdId9 SecId2 Buy 900 User6 Company2\n"
        "OrdId10 SecId2 Sell 1000 User5 Company1\n"
        "OrdId11 SecId1 Sell 1100 User13 Company2\n"
        "OrdId12 SecId2 Buy 1200 User9 Company2\n"
        "OrdId13 SecId1 Sell 1300 User1 Company\n");

    OrderCache cache;

    std::set<std::string> securities;
    std::string line;
    while (std::getline(input, line))
    {
        std::string orderId;
        std::string secId;
        std::string action;
        int quantity;
        std::string userId;
        std::string company;
        std::istringstream stream(line);
        stream >> orderId >> secId >> action >> quantity >> userId >> company;
        cache.addOrder({orderId, secId, action, quantity, userId, company});
        securities.insert(secId);
    }

    std::cout << "Matching size:\n";
    for (const auto& sec : securities)
        std::cout << sec << ": " << cache.getMatchingSizeForSecurity(sec)
                  << "\n";

    return 0;
}
