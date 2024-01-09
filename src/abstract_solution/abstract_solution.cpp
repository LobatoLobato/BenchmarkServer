//
// Created by Ash-- on 02/01/2024.
//

#include "bs/abstract_solution.hpp"
#include "bs/git.hpp"
#include "bs/docker.hpp"
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <utility>

bs::AbstractSolution::AbstractSolution(
  std::string repo_url,
  const std::string& clone_path,
  const std::string& tag,
  const std::vector<std::string>& build_args
)
  : repo_url(std::move(repo_url)), clone_path(boost::to_lower_copy(clone_path)), tag(boost::to_lower_copy(tag)),
    full_clone_path(fmt::format("{}/{}", CLONED_REPOSITORIES_PATH, this->clone_path)), build_args(build_args) { }

void bs::AbstractSolution::clone() {
    Git::Output git_pull = Git::pull(repo_url, full_clone_path);
    if (git_pull.exit_code == 0) { return; }

    Git::Output git_clone = Git::clone(repo_url, full_clone_path);
    if (git_clone.exit_code == 0) { return; }

    git_pull.data.insert(git_pull.data.end(), git_clone.data.begin(), git_clone.data.end());
    throw std::runtime_error(boost::join(git_pull.data, "\n"));
}

void bs::AbstractSolution::build() {
    Docker::Output docker_build = Docker::build(tag, full_clone_path, { }, build_args);
    if (docker_build.exit_code == 0) { return; }

    throw std::runtime_error(boost::join(docker_build.data, "\n"));
}

std::string bs::AbstractSolution::benchmark() {
    Docker::Output docker_run = Docker::run(tag, { "--rm" });
    if (docker_run.exit_code != 0) {
        if (docker_run.data.empty()) { throw std::logic_error("Solution produced no output."); }
        throw std::runtime_error(boost::join(docker_run.data, "\n"));
    }
    for (auto& l : docker_run.data) {
        try {
            time = std::stod(l);
            if (std::isinf(time)) { continue; }
            return boost::join(docker_run.data, "\n");
        }
        catch (std::exception&) { }
    }

    docker_run.data.insert(docker_run.data.begin(), {
      "Could not parse benchmark time.",
      "Please check the output below:"
    });
    throw std::logic_error(boost::join(docker_run.data, "\n"));
}