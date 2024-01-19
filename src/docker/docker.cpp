//
// Created by Lobato on 21/12/2023.
//

#include "bs/docker.hpp"
#include <boost/filesystem.hpp>
#include <boost/process.hpp>

namespace bp = boost::process;
namespace ba = boost::algorithm;

bs::Docker::Output bs::Docker::build(
  const std::string& tag,
  const std::string& dockerfile_path,
  ArgList args,
  const ArgList& build_args
) {
    args.insert(args.end(), { "-t", tag });
    for (const auto& arg : build_args) { args.insert(args.end(), { "--build-arg", arg }); }
    args.insert(args.end(), dockerfile_path);
    return spawnCommand("build", args);
}

bs::Docker::Output bs::Docker::tag(const std::string& image_tag, const std::string& new_image_tag) {
    return spawnCommand("tag", {image_tag, new_image_tag});
}

bs::Docker::Output bs::Docker::rmi(const std::string& image_tag) {
    return spawnCommand("rmi", {image_tag});
}

bs::Docker::Output bs::Docker::run(
  const std::string& container_name,
  ArgList args,
  const ArgList& container_args
) {
    args.insert(args.end(), container_name);
    args.insert(args.end(), container_args.begin(), container_args.end());
    return spawnCommand("run", args);
}

bs::Docker::Output bs::Docker::spawnCommand(const char* command, const ArgList& args) {
    static const boost::filesystem::path docker_path = bp::search_path("docker");
    bp::ipstream pipe_stream;
    bp::child c(docker_path, command, args, (bp::std_err & bp::std_out) > pipe_stream);
    std::vector<std::string> output = { std::string{ } + "docker" + " " + command + " " + boost::join(args, " ") };
    std::string line;

    while (c.running() && std::getline(pipe_stream, line)) {
        if (line.empty()) { continue; }
        output.push_back(line);
    }

    c.wait();
    return { output, c.exit_code() };
}

