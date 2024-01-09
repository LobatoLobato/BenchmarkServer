add_library(bs_aoc_solution aoc_solution.cpp include/bs/aoc_solution.hpp)

target_link_libraries(bs_aoc_solution
        PRIVATE
        nlohmann_json::nlohmann_json
        fmt::fmt
        PUBLIC
        bs_abstract_solution
)

target_include_directories(bs_aoc_solution PUBLIC include)
