//
// Created by Ash-- on 08/01/2024.
//

#include "aoc_route.hpp"
#include <vector>
#include <orm/db.hpp>
#include <nlohmann/json.hpp>
#include "bs/aoc_solution.hpp"

namespace bs::routes
{
  using httplib::Request, httplib::Response, httplib::Server, httplib::DataSink;
  using Orm::DB, nlohmann::json;
  using detail::Route, detail::SubRoute;

  const Route aoc::route = { // NOLINT(*-interfaces-global-init)
    SubRoute{
      &Server::Get, "/aoc/leaderboard/:day/:part", [](const Request& req, Response& res) {
        Orm::SqlQuery leaderboard_query = DB::table("leaderboard_entries")->where({
          { "leaderboard_name", "AOC" },
          { "leaderboard_day",  std::stoi(req.path_params.at("day")) },
          { "leaderboard_part", std::stoi(req.path_params.at("part")) },
        }).orderBy("time").get();


        std::vector<json> solutions;
        while (leaderboard_query.next()) {
            solutions.push_back(json{
              { "author",   leaderboard_query.value("author").toString().toStdString() },
              { "repo_url", leaderboard_query.value("repo_url").toString().toStdString() },
              { "day",      leaderboard_query.value("leaderboard_day").toInt() },
              { "part",     leaderboard_query.value("leaderboard_part").toInt() },
              { "time",     leaderboard_query.value("time").toDouble() },
            });
        }

        res.set_content((json{{ "leaderboard", solutions }}).dump(), "application/json");
      }
    },
    SubRoute{
      &Server::Post, "/aoc/solution", [](const Request& req, Response& res) {
        const static std::string padding = std::string(4096, ' ') + "\n";
        auto* solution = new bs::AocSolution(json::parse(req.body));

        res.status = httplib::PartialContent_206;
        res.set_header("Connection", "keep-alive");
        res.set_chunked_content_provider("application/stream+json", [solution](size_t, DataSink& sink) -> bool {
            try {
                sink.os << json{{ "message", "Clonando solução..." }}.dump() + padding;
                solution->clone();
                sink.os << json{{ "message", "Construindo solução..." }}.dump() + padding;
                solution->build();
                sink.os << json{{ "message", "Rodando benchmark..." }}.dump() + padding;
                std::string benchmark_output = solution->benchmark();
                sink.os << json{{ "data",             solution->toJson() },
                                { "benchmark_output", benchmark_output }}.dump();


                DB::table("leaderboards")->insertOrIgnore({
                  { "name", "AOC" },
                  { "day",  solution->day },
                  { "part", solution->part }
                });
                DB::table("leaderboard_entries")->updateOrInsert(
                  {
                    { "author",           QString::fromStdString(solution->author) },
                    { "leaderboard_name", "AOC" },
                    { "leaderboard_day",  solution->day },
                    { "leaderboard_part", solution->part },
                  },
                  {
                    { "author",           QString::fromStdString(solution->author) },
                    { "repo_url",         QString::fromStdString(solution->repo_url) },
                    { "time",             solution->time },
                    { "leaderboard_name", "AOC" },
                    { "leaderboard_day",  solution->day },
                    { "leaderboard_part", solution->part }
                  });
            }
            catch (std::exception& e) { sink.os << json{{ "error", e.what() }}.dump(); }

            sink.done();
            return true;
          }, [solution](bool) { delete solution; }
        );
      }
    }
  };
} // vrdev