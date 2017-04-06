#include <iostream>
#include <math.h>

int main()
{
    int n, s;
    std::cin >> n;

    if ((n & (n - 1)) == 0) s = n;
    else s = (2 * (n - (1 << (int)floor(log2(n)))));

    std::cout << s;
}

