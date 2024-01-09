add_library(bs_aoc_route aoc_route.cpp aoc_route.hpp)

target_link_libraries(bs_aoc_route
        PRIVATE
        httplib::httplib
        nlohmann_json::nlohmann_json
        fmt::fmt
        Qt${QT_VERSION_MAJOR}::Core
        TinyOrm::TinyOrm
        bs_aoc_solution
)

target_include_directories(bs_aoc_route PRIVATE ../include)