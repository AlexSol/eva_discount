#include "NormalizeCheck.h"
#include <fstream>
#include <regex>
#include <format>

std::pair<int, int> get_amount_discount_from_line(const std::string& line)
{
    std::vector<std::string> nums;
    static const std::regex regex("(\\d+)");
    std::copy(std::sregex_token_iterator(line.begin(), line.end(), regex, 0),
        std::sregex_token_iterator(),
        std::back_insert_iterator<std::vector<std::string>>(nums));

    //@first amount_product, @second discount
    return { std::stoi(nums[1]), std::stoi(nums[2]) };
}


std::vector<std::string> check_and_split_product_line(const std::string& line)
{
    std::vector<std::string> result;
    const auto [amount_product, discount] = get_amount_discount_from_line(line);

    if (discount % amount_product != 0) {
        static const auto determ = ".,";
        if (auto found = line.rfind(determ); found != std::string::npos) {
            const auto amount_head = amount_product - 1;
            const auto amount_tail = 1;
            const auto discount_head = discount / amount_product;
            const auto discount_tail = discount - discount_head * amount_head;

            auto new_line_head{ line.substr(0, found) };
            auto new_line_tail{ line.substr(0, found) };

            std::format_to(
                std::back_inserter(new_line_head),
                "{} {} et, discount {}.",
                determ, amount_head, discount_head
            );

            std::format_to(
                std::back_inserter(new_line_tail),
                "{} {} et, discount {}.",
                determ, amount_tail, discount_tail
            );
            
            result.emplace_back(std::move(new_line_head));
            result.emplace_back(std::move(new_line_tail));
        }
    }
    return result;
}

std::vector<std::string> normalize_check_line(const std::vector<std::string>& lines)
{
    std::vector<std::string> result;
    for (auto& item : lines) {

        if (const auto& lines = check_and_split_product_line(item); !lines.empty()) {
            std::copy(lines.cbegin(), lines.cend(), std::back_inserter(result));
        }
        else {
            result.push_back(item);
        }
    }

    return result;
}

std::vector<std::string> normalize_check_line(const std::string& file_name)
{
    std::ifstream file(file_name.data());
    std::vector<std::string> res;
    try {
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line))
            {
                res.emplace_back(line);
            }
            file.close();
        }
    }
    catch (std::ifstream::failure e) {
        throw(std::string("Exception opening/reading/closing file: ") + file_name);
    }

    return normalize_check_line(res);
}