#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace retronomicon::vulkan {

class VulkanRenderer : public retronomicon::graphics::renderer::IRenderer {
public:
    VulkanRenderer(const std::string& title, int width, int height);
    ~VulkanRenderer();

    void init() override;
    void render() override;
    void shutdown() override;

private:
    std::string m_title;
    int m_width, m_height;

    SDL_Window* m_window = nullptr;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    bool m_initialized = false;

    void createInstance();
    void pickPhysicalDevice();
    void createLogicalDevice();
};

} // namespace retronomicon::vulkan
