add_library(bs_routes include/bs/routes.hpp)

add_subdirectory(aoc_route)

target_link_libraries(bs_routes
        PRIVATE
        bs_aoc_route
)

target_include_directories(bs_routes PUBLIC include)
