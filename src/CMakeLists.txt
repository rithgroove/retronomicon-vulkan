message(STATUS "==== Processing ${CMAKE_CURRENT_LIST_FILE} ====")

# Add submodules inside lib/
add_subdirectory(graphics)

# Define the main retronomicon engine library
add_library(retronomicon-vulkan)

# Link subsystem libraries
target_link_libraries(retronomicon-vulkan
    PUBLIC
        retronomicon-vulkan-graphics
)

