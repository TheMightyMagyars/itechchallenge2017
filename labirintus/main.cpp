#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>


int main()
{
    auto ptr = std::make_unique<std::vector<std::string>>(5, "a");
    for (const auto& x : *ptr)
    {
        std::cout << x << '\n';
    }
}



