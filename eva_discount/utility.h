#pragma once
#include <iostream>

template<typename Vec>
void print_vector(Vec& vec)
{
    std::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<Vec::value_type>(std::cout, "\n"));
}
