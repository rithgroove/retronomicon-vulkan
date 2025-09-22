file(GLOB_RECURSE OPENGL_GRAPHIC_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-vulkan-graphics OBJECT ${OPENGL_GRAPHIC_PLUGIN})

target_include_directories(
    retronomicon-vulkan-graphics PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_VULKAN_DIR}/include
    ${RETRO_VULKAN_DIR}/external/SDL2/include
    ${RETRO_VULKAN_DIR}/external/vulkan/include
)

target_link_libraries(retronomicon-vulkan-graphics
        SDL2::SDL2     # static SDL2 (change to SDL2::SDL2-static if needed)
        Vulkan-Headers
        )