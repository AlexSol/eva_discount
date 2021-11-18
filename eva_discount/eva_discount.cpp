// eva_discount.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "NormalizeCheck.h"
#include "NormalizeCheck_TEST.h"
#include "utility.h"

int main()
{
    run_test();
        
    std::vector<std::string> in_lines{
        "product_B, 123 price., 3 et, discount 23.",
        "product_B, 150 price., 4 et, discount 23.",
        "product_A, 353 price., 5 et, discount 451."
    };

    const auto line_product = normalize_check_line(in_lines);
    
    std::cout << "in lines:\n";
    print_vector(in_lines);

    std::cout << "\nout lines:\n";
    print_vector(line_product);
}
