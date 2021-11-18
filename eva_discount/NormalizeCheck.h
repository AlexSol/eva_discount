#pragma once
#include <vector>
#include <string>
#include <string_view>

std::vector<std::string> normalize_check_line(const std::string& file_name);
std::vector<std::string> normalize_check_line(const std::vector<std::string>& lines);