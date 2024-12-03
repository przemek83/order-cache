#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include "OrderCache.h"

namespace
{
void print(const std::string& txt) { std::cout << txt; }
void println(const std::string& txt) { print(txt + '\n'); }

};  // namespace

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
        int quantity{0};
        std::string userId;
        std::string company;
        std::istringstream stream(line);
        stream >> orderId >> secId >> action >> quantity >> userId >> company;
        cache.addOrder({orderId, secId, action, quantity, userId, company});
        securities.insert(secId);
    }

    println("Matching quantity:");
    for (const auto& sec : securities)
        println(sec + ": " +
                std::to_string(cache.getMatchingSizeForSecurity(sec)));

    return 0;
}
