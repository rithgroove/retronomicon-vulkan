cmake_minimum_required(VERSION 3.10)

# Create glad library (Vulkan)
add_library(glad STATIC
    src/vulkan.c
)

# Public include directory (so consumers can #include <glad/...>)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: if you want to select between GL and GLES in code

#if (USE_GLES)
#    target_compile_definitions(glad PUBLIC GLAD_GLES2)
#else()
#    target_compile_definitions(glad PUBLIC GLAD_GL)
#endif()