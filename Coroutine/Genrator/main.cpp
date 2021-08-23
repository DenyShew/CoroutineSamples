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
    auto val = gen();
    for(int i=0;i<5;i++)
    {
        val.next();
        std::cout << val.get_value() << std::endl;
    }
    return 0;
}