//
// Created by Lobato on 21/12/2023.
//

#ifndef BENCHMARKSERVER_DOCKER_HPP
#define BENCHMARKSERVER_DOCKER_HPP

#include <string>
#include <vector>
namespace bs
{
  class Docker {
  public:
      using ArgList = std::vector<std::string>;
      struct Output {
          std::vector<std::string> data;
          int exit_code;
      };

      static Docker::Output build(
        const std::string& tag,
        const std::string& dockerfile_path,
        ArgList args,
        const ArgList& build_args = { }
      );
      
      static Docker::Output tag(const std::string& image_tag, const std::string& new_image_tag);
      
      static Docker::Output rmi(const std::string& image_tag);
      
      static Docker::Output run(const std::string& container_name, ArgList args, const ArgList& container_args = { });

  private:
      static Docker::Output spawnCommand(const char* command, const ArgList& args);
  };
}


#endif //BENCHMARKSERVER_DOCKER_HPP
