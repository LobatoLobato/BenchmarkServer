#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <orm/db.hpp>
#include <orm/utils/configuration.hpp>
#include <thread>
#include <sstream>
#include "bs/routes.hpp"

void connectCurrentThreadToDatabase();

int main() {
    using httplib::Request, httplib::Response, httplib::DataSink, nlohmann::json;

    std::shared_ptr<Orm::DatabaseManager> database_manager = Orm::DB::create();
    httplib::Server server;

    server.set_exception_handler([](const Request&, Response& res, const std::exception_ptr& ep) {
        try { rethrow_exception(ep); }
        catch (std::exception& e) {
            res.status = 500;
            res.set_content(json{{ "error", e.what() }}.dump(), "application/json");
        }
        catch (...) {
            res.status = 500;
            res.set_content(json{{ "error", "Unknown Exception" }}.dump(), "application/json");
        }
      }
    );
    
    server.set_logger([](const Request& req, const Response& res) {
        std::cout << req.method << " request on " << req.path << "\n";
        for (auto [header, value] : req.headers) {
            static const std::vector<std::string> headers_to_log = {
              "Accept",
              "Accept-Enconding",
              "Connection",
              "Content-Length",
              "Content-Type",
              "REMOTE_ADDR",
              "REMOTE_PORT",
              "User-Agent",
              "Cf-Connecting-Ip",
              "Cf-Ipcountry",
            };
          
            if(std::find(headers_to_log.begin(), headers_to_log.end(), header) != headers_to_log.end()) {
                std::cout << header << ": " << value << "\n";
            }
        }
        if (!req.body.empty()) { std::cout << "Body: " << nlohmann::json::parse(req.body).dump(2) << "\n"; }
        
        std::cout << std::flush;
    });

    server.set_pre_routing_handler([](const Request& req, Response& res) {
      connectCurrentThreadToDatabase();
      return httplib::Server::HandlerResponse::Unhandled;
    });

    server.set_keep_alive_timeout(300);
    server.set_keep_alive_max_count(300);

    server.new_task_queue = [] { return new httplib::ThreadPool(1); };

    bs::routes::aoc::attachTo(server);

    std::cout << "Server listening on localhost:8080" << std::endl;

    server.listen("0.0.0.0", 8080);
    return 0;
}

void connectCurrentThreadToDatabase() {
    using namespace Orm::Constants; // NOLINT(google-build-using-namespace)
    static const QHash<QString, QVariant> connection_config{
      { driver_,    QMYSQL },
      { host_,      qEnvironmentVariable("DB_MYSQL_HOST", H127001) },
      { port_,      qEnvironmentVariable("DB_MYSQL_PORT", P3306) },
      { database_,  qEnvironmentVariable("DB_MYSQL_DATABASE") },
      { username_,  qEnvironmentVariable("DB_MYSQL_USERNAME") },
      { password_,  qEnvironmentVariable("DB_MYSQL_PASSWORD") },
      { charset_,   qEnvironmentVariable("DB_MYSQL_CHARSET", UTF8MB4) },
      { collation_, qEnvironmentVariable("DB_MYSQL_COLLATION", UTF8MB40900aici) },
      { timezone_,  TZ00 },
      { strict_,    true },
    };

    QString thread_id = QString::fromStdString((std::stringstream{ } << std::this_thread::get_id()).str());
    if (Orm::DB::connectionNames().empty()) {
        Orm::DB::addConnection(connection_config, thread_id);
    }
    Orm::DB::setDefaultConnection(thread_id);
}