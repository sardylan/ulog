if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message(STATUS "CLang compiler detected.")
    add_compile_options(-Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "GCC compiler detected. Adding compile flags for errors.")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    message(STATUS "Intel compiler detected.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "MSCV compiler detected. Adding compile flags for errors.")
    add_compile_options(/Wall)
else ()
    message(STATUS "Unable to detect compiler automatically: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "No compile flag added")
endif ()

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

include_directories(.)

add_subdirectory(utilities)
add_subdirectory(rigctl)
add_subdirectory(delegators)

add_subdirectory(app)

