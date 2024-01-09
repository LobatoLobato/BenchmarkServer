#pragma once

#include <tom/migration.hpp>

namespace Migrations
{

    struct CreateLeaderboardsTable : Migration
    {
        /*! Filename of the migration file. */
        T_MIGRATION

        /*! Run the migrations. */
        void up() const override
        {
            Schema::create("leaderboards", [](Blueprint &table)
            {
                table.string("name");
                table.integer("day");
                table.integer("part");
                table.primary({"name", "day", "part"});
                table.timestamps();
            });
        }

        /*! Reverse the migrations. */
        void down() const override
        {
            Schema::dropIfExists("leaderboards");
        }
    };

} // namespace Migrations
