cmake_minimum_required(VERSION 3.27)
project(rengo)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Add the shared library target
add_library(rengo SHARED library.cpp)

# Specify where to find the headers
target_include_directories(rengo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add the test executable
add_executable(test_rengo main.cpp)

# Link the test executable against the shared library
target_link_libraries(test_rengo PRIVATE rengo)

# Add install targets (optional)
install(TARGETS rengo DESTINATION lib)
install(FILES library.h DESTINATION include)

# Optionally export symbols for shared library (cross-platform compatibility)
if(WIN32)
    target_compile_definitions(rengo PRIVATE RENGO_EXPORT)
endif()
