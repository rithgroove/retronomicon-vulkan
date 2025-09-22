cmake_minimum_required(VERSION 3.10)

# Create glad library (Vulkan)
add_library(glad
)

# Public include directory (so consumers can #include <glad/...>)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vulkan/include
    
)

