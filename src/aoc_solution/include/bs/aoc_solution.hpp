//
// Created by Ash-- on 02/01/2024.
//

#ifndef BENCHMARKSERVER_AOC_SOLUTION_HPP
#define BENCHMARKSERVER_AOC_SOLUTION_HPP

#include <string>
#include "nlohmann/json.hpp"
#include "bs/abstract_solution.hpp"

namespace bs
{
  class AocSolution : public AbstractSolution {
  public:
      const std::string author;
      const int day, part;

      AocSolution(const std::string& author, const std::string& repo_url, int day, int part);

      explicit AocSolution(const nlohmann::json& j);

      [[nodiscard]] inline nlohmann::json toJson() const override {
          return {{ "author",     author },
                  { "repo_url",   repo_url },
                  { "day",        day },
                  { "part",       part },
                  { "time",       time },
                  { "tag",        tag },
                  { "clone_path", clone_path }};
      }
  };
}
#endif //BENCHMARKSERVER_AOC_SOLUTION_HPP
