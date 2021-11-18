#include "NormalizeCheck_TEST.h"
#include <iostream>
#include "NormalizeCheck.h"
#include <cassert>
#include "utility.h"

void TEST_EQUAL(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs)
{
    const bool result = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    if (!result) {
        std::cout << "TEST_CASE EQUAL: failed\n"
            << "vec 1:\n";
        print_vector(lhs);
        std::cout << "vec 2:\n";
        print_vector(rhs);
        std::cout << std::endl;
    }
}

void run_test()
{
    {
        const std::vector<std::string> in_lines{
            "product_A, 353 price., 5 et, discount 45."
        };
        const std::vector<std::string> lines{
            "product_A, 353 price., 5 et, discount 45."
        };
        TEST_EQUAL(lines, normalize_check_line(in_lines));
    }

    {
        const std::vector<std::string> in_lines{
            "product_A, 353 price., 5 et, discount 451."
        };

        const std::vector<std::string> lines{
            "product_A, 353 price., 4 et, discount 90.",
            "product_A, 353 price., 1 et, discount 91."
        };

        TEST_EQUAL(lines, normalize_check_line(in_lines));
    }

    {
        const std::vector<std::string> in_lines{
            "product_B, 123 price., 3 et, discount 23.",
            "product_B, 123 price., 4 et, discount 23."
        };

        const std::vector<std::string> lines{
            "product_B, 123 price., 2 et, discount 7.",
            "product_B, 123 price., 1 et, discount 9.",
            "product_B, 123 price., 3 et, discount 5.",
            "product_B, 123 price., 1 et, discount 8."
        };
        TEST_EQUAL(lines, normalize_check_line(in_lines));
    }
}