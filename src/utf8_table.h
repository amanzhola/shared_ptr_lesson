#pragma once

#include <string>
#include <vector>

struct OwnershipRow {
    std::string tex_name;
    std::string owners;
    long long use_count;
    std::string note;
};

void PrintOwnershipTable(const std::string& title, const std::vector<OwnershipRow>& rows);
