add_library(bs_abstract_solution abstract_solution.cpp include/bs/abstract_solution.hpp)

target_link_libraries(bs_abstract_solution PRIVATE nlohmann_json::nlohmann_json fmt::fmt bs_git bs_docker)
target_include_directories(bs_abstract_solution PUBLIC include)