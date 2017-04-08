#include <iostream>
#include <math.h>

int main()
{
    int n;
    std::cin >> n;
    std::cout << "[DEBUG] input: " << n << std::endl;
    std::cout << ((n & (n - 1)) == 0) ? n : (2 * (n - (1 << (int)floor(log2(n))))) << '\n';
    return 0;
}

