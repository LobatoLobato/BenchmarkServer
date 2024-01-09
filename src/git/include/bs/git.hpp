//
// Created by Lobato on 22/12/2023.
//

#ifndef BENCHMARKSERVER_GIT_HPP
#define BENCHMARKSERVER_GIT_HPP

#include <string>
#include <vector>
namespace bs
{
  class Git {
  public:
      using ArgList = std::vector<std::string>;
      struct Output {
          std::vector<std::string> data;
          int exit_code;
      };

      static Git::Output clone(const std::string& git_url, const std::string& clone_path);

      static Git::Output pull(const std::string& git_url, const std::string& base_dir);

  private:
      static Git::Output spawnCommand(const char* command, const ArgList& args, const ArgList& git_args = { });
  };
}

#endif //BENCHMARKSERVER_GIT_HPP
