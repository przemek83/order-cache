#include <iostream>
#include <set>
#include <sstream>

#include "OrderCache.h"

int main()
{
    OrderCache cache;
    std::set<std::string, std::less<>> securities;
    std::string line;
    while (std::getline(std::cin, line))
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

    std::cout << "Matching quantity:\n";
    for (const auto& sec : securities)
        std::cout << sec << ": " << cache.getMatchingSizeForSecurity(sec)
                  << "\n";

    return 0;
}
