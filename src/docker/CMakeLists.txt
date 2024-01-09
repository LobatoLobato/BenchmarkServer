add_library(bs_docker docker.cpp include/bs/docker.hpp)

add_compile_definitions(__kernel_entry=)

target_link_libraries(bs_docker
        PRIVATE
        $<$<BOOL:${WIN32}>:ws2_32>
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
)

target_include_directories(bs_docker PUBLIC include)