#pragma once

#include <tom/migration.hpp>

namespace Migrations
{

  struct CreateLeaderboardEntriesTable : Migration {
      /*! Filename of the migration file. */
      T_MIGRATION

      /*! Run the migrations. */
      void up() const override {
          Schema::create("leaderboard_entries", [](Blueprint& table) {
            table.string("author");
            table.string("repo_url");
            table.Double("time");

            table.string("leaderboard_name");
            table.integer("leaderboard_day");
            table.integer("leaderboard_part");

            table.timestamps();

            table.primary({ "author", "leaderboard_name", "leaderboard_day", "leaderboard_part" });
            table.foreign({ "leaderboard_name", "leaderboard_day", "leaderboard_part" }, "le_lname_lday_lpart")
                 .references({ "name", "day", "part" }).on("leaderboards");
          });
      }

      /*! Reverse the migrations. */
      void down() const override {
          Schema::dropIfExists("leaderboard_entries");
      }
  };
} // namespace Migrations
