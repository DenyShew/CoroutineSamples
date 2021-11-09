#include "generator.hpp"

#include <iostream>

generator<int> gen()
{
    for(int i=0;i<5;i++)
    {
        co_yield i;
    }
}

int main()
{
    generator<int> val = gen();
    for(int i=0;i<5;i++)
    {
        std::cout << val() << std::endl;
    }
    return 0;
}