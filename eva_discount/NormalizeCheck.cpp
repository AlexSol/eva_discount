#include "NormalizeCheck.h"
#include <fstream>
#include <regex>
#include <format>
#include <algorithm>
#include <map>

struct ReceiptLine {
    //std::string name //TODO ?
    std::size_t price;
    std::size_t amount_product;
    std::size_t total_discount;
};

ReceiptLine get_info_line(const std::string& receipt_line)
{
    std::vector<std::string> nums;
    static const std::regex regex("(\\d+)");
    std::copy(std::sregex_token_iterator(receipt_line.begin(), receipt_line.end(), regex, 0),
        std::sregex_token_iterator(),
        std::back_insert_iterator<std::vector<std::string>>(nums));

    return ReceiptLine(std::stoi(nums[0]), std::stoi(nums[1]), std::stoi(nums[2]));
}

std::vector<std::pair<int, int>> list_unique_discount(const std::vector<int>& vec)
{
    std::map<int, int> map_discount;

    for (const auto value : vec)
    {
        ++map_discount[value];
    }
    
    std::vector<std::pair<int, int>> result;

    for (const auto [discount, amount] : map_discount)
    {
        result.push_back({ amount, discount});
    }

    std::sort(result.begin(), result.end(), [](auto lhs, auto rhs) {
        return lhs > rhs;
        });

    return result;
}

std::vector<std::string> build_receipt_line(const ReceiptLine& line_info, const std::string& receipt_line)
{
    std::vector<std::string> result;
    static const auto determ = ".,";
    if (auto found = receipt_line.rfind(determ); found != std::string::npos) {
        const auto prefix_line{ receipt_line.substr(0, found) };

        const auto discount_of_part = line_info.total_discount / line_info.amount_product;

        std::vector<int> v_line(line_info.amount_product, discount_of_part);

        const auto discount_tail = line_info.total_discount - (line_info.amount_product * discount_of_part);
        std::for_each_n(v_line.begin(), discount_tail, [](auto& n) { n += 1; });

        for (const auto [amount, discount] : list_unique_discount(v_line)) {
            std::string line = prefix_line + std::format("{} {} et, discount {}.",
                determ, amount, discount);
            result.emplace_back(std::move(line));
        }
    }
    return result;
}

std::vector<std::string> split_receipt_discount_line(const std::string& receipt_line)
{
    const auto line_info = get_info_line(receipt_line);

    if (line_info.total_discount % line_info.amount_product != 0) {
        return build_receipt_line(line_info, receipt_line);
    }
    return {};
}

std::vector<std::string> normalize_receipt_line(const std::vector<std::string>& lines)
{
    std::vector<std::string> result;
    for (auto& item : lines) {

        if (const auto& lines = split_receipt_discount_line(item); !lines.empty()) {
            std::copy(lines.cbegin(), lines.cend(), std::back_inserter(result));
        }
        else {
            result.push_back(item);
        }
    }

    return result;
}

std::vector<std::string> normalize_receipt_line(const std::string& file_name)
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

    return normalize_receipt_line(res);
}
