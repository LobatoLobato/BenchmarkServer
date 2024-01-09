cmake_minimum_required(VERSION 3.27)
project(BenchmarkServer)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(httplib GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git)
FetchContent_MakeAvailable(httplib)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git)
FetchContent_MakeAvailable(fmt)

find_package(Boost 1.83.0 REQUIRED system filesystem)
if (Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIR})
endif ()

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
        httplib::httplib
        nlohmann_json::nlohmann_json
        fmt::fmt
)