cmake_minimum_required(VERSION 3.17)
project(GraphProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Werror)

include_directories(include)
include_directories(lib)

add_library(parameters SHARED IMPORTED)
if(APPLE)
    set_target_properties(parameters PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libparameters.dylib
    )
else()
    set_target_properties(parameters PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libparameters.so
    )
endif()

add_executable(graph src/main.cpp)
target_link_libraries(graph parameters)