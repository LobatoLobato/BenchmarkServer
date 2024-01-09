//
// Created by Ash-- on 05/01/2024.
//
#include <iostream>

#include <orm/db.hpp>

#include <tom/application.hpp>

#include "migrations/2024_01_05_190308_create_leaderboards_table.hpp"
#include "migrations/2024_01_05_195217_create_leaderboard_entries_table.hpp"

using Orm::DatabaseManager;
using Orm::DB;

using TomApplication = Tom::Application;

std::shared_ptr<DatabaseManager> setupDatabaseManager();

int main(int argc, char* argv[]) {
    try {
        auto db = setupDatabaseManager();
        return TomApplication(argc, argv, std::move(db), "TOM_EXAMPLE_ENV")
          .migrations<Migrations::CreateLeaderboardsTable, Migrations::CreateLeaderboardEntriesTable>()
          .run();
    }
    catch (const std::exception& e) {
        TomApplication::logException(e);
    }

    return EXIT_FAILURE;
}

std::shared_ptr<DatabaseManager> setupDatabaseManager() {
    using namespace Orm::Constants;

    return DB::create({
        { driver_,     QMYSQL },
        { host_,       qEnvironmentVariable("DB_MYSQL_HOST", H127001) },
        { port_,       qEnvironmentVariable("DB_MYSQL_PORT", P3306) },
        { database_,   qEnvironmentVariable("DB_MYSQL_DATABASE") },
        { username_,   qEnvironmentVariable("DB_MYSQL_USERNAME") },
        { password_,   qEnvironmentVariable("DB_MYSQL_PASSWORD") },
        { charset_,    qEnvironmentVariable("DB_MYSQL_CHARSET", UTF8MB4) },
        { collation_,  qEnvironmentVariable("DB_MYSQL_COLLATION", UTF8MB40900aici) },
        { timezone_,   TZ00 },
        /* Specifies what time zone all QDateTime-s will have, the overridden default is
           the Qt::UTC, set to the Qt::LocalTime or QtTimeZoneType::DontConvert to use
           the system local time. */
        { qt_timezone, QVariant::fromValue(Qt::UTC) },
        { strict_,     true },
      },
      QStringLiteral("tinyorm_tom_mysql")); // shell:connection
}