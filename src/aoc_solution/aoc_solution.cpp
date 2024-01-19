//
// Created by Ash-- on 02/01/2024.
//

#include "bs/aoc_solution.hpp"
#include "fmt/core.h"
#include <ranges>

using fmt::format;

bs::AocSolution::AocSolution(const std::string& author, const std::string& repo_url, int day, int part)
  : author(author), day(day), part(part),
    AbstractSolution(
      repo_url, format("{}_aoc_solution", author),
      format("{}_aoc_solution", author),
      { format("DAY={}", day), format("PART{}=1", part) }
    ) { }

bs::AocSolution::AocSolution(const nlohmann::json& j)
  : AocSolution(j["author"], j["repo_url"], j["day"], j["part"]) { }
