#file(GLOB MODELS models/*.hpp)
#
#add_library(Database INTERFACE ${MODELS})
#
#target_include_directories(Database INTERFACE.)

#add_subdirectory(tom)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(TinyOrm 0.36.5 CONFIG REQUIRED)

add_executable(tom tom.cpp)

target_link_libraries(tom
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        TinyOrm::TinyOrm
)