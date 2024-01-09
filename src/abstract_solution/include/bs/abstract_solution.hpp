//
// Created by Ash-- on 02/01/2024.
//

#ifndef BENCHMARKSERVER_ABSTRACT_SOLUTION_HPP
#define BENCHMARKSERVER_ABSTRACT_SOLUTION_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace bs
{
  class AbstractSolution {
  public:
      std::string repo_url, clone_path, tag;
      double time = 0;

      AbstractSolution(
        std::string repo_url, const std::string& clone_path, const std::string& tag,
        const std::vector<std::string>& build_args
      );

      void clone();

      void build();

      std::string benchmark();

      [[nodiscard]] virtual nlohmann::json toJson() const = 0;

      [[maybe_unused, nodiscard]] inline std::string toString() const { return nlohmann::to_string(this->toJson()); }

  private:
      const std::string full_clone_path;
      const std::vector<std::string> build_args;
  };
}

#endif //BENCHMARKSERVER_ABSTRACT_SOLUTION_HPP
