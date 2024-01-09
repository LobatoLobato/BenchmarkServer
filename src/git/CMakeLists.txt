add_library(bs_git git.cpp include/bs/git.hpp)

add_compile_definitions(__kernel_entry=)

target_link_libraries(bs_git
        PRIVATE
        $<$<BOOL:${WIN32}>:ws2_32>
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
)

target_include_directories(bs_git PUBLIC include)