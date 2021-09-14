find_package(Qt5 REQUIRED COMPONENTS
        Core Gui Widgets)

add_library(delegators
        datetime.cpp datetime.hpp
        string.cpp string.hpp)

target_link_libraries(delegators
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)

target_include_directories(delegators
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
