//
// Created by Lobato on 22/12/2023.
//

#include "bs/git.hpp"
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

namespace bp = boost::process;
namespace bf = boost::filesystem;

bs::Git::Output bs::Git::clone(const std::string& git_url, const std::string& clone_path) {
    return spawnCommand("clone", { git_url, clone_path });
}

bs::Git::Output bs::Git::pull(const std::string& git_url, const std::string& base_dir) {
    return spawnCommand("pull", { git_url }, { "-C", base_dir });
}

bs::Git::Output bs::Git::spawnCommand(const char* command, const ArgList& args, const ArgList& git_args) {
    static const bf::path git_path = bp::search_path("git");
    bp::ipstream pipe_stream;
    bp::child c(git_path, git_args, command, args, (bp::std_out & bp::std_err) > pipe_stream);

    std::vector<std::string> output = {
      std::string{ } + "git" + " " + boost::join(git_args, " ") + " " + command + " " + boost::join(args, " ")
    };
    std::string line;

    while (c.running() && std::getline(pipe_stream, line)) {
        if (line.empty()) { continue; }
        output.push_back(line);
    }

    c.wait();

    return { output, c.exit_code() };
}

