#include "retronomicon/graphics/renderer/vulkan_renderer.h"
#include <glad/vulkan.h>

namespace retronomicon::vulkan {

VulkanRenderer::VulkanRenderer(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {}

VulkanRenderer::~VulkanRenderer() {
    shutdown();
}

void VulkanRenderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("Failed to init SDL: " + std::string(SDL_GetError()));
    }

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width, m_height,
        SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
    );
    if (!m_window) {
        throw std::runtime_error("Failed to create SDL window: " + std::string(SDL_GetError()));
    }

    // 1. Create Vulkan instance
    createInstance();

    // 2. Create Vulkan surface via SDL
    if (!SDL_Vulkan_CreateSurface(m_window, m_instance, &m_surface)) {
        throw std::runtime_error("Failed to create Vulkan surface");
    }

    // 3. Load Vulkan functions with GLAD
    if (!gladLoaderLoadVulkan(m_instance, vkGetInstanceProcAddr)) {
        throw std::runtime_error("Failed to load Vulkan functions with GLAD");
    }

    // 4. Pick physical device + logical device
    pickPhysicalDevice();
    createLogicalDevice();

    m_initialized = true;
    std::cout << "VulkanRenderer initialized with SDL window + Vulkan instance ✅" << std::endl;
}

void VulkanRenderer::render() {
    if (!m_initialized) return;

    // TODO: swapchain + draw commands
    std::cout << "VulkanRenderer::render() frame stub" << std::endl;
}

void VulkanRenderer::shutdown() {
    if (m_device) {
        vkDeviceWaitIdle(m_device);
        vkDestroyDevice(m_device, nullptr);
        m_device = VK_NULL_HANDLE;
    }
    if (m_surface) {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }
    if (m_instance) {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    if (m_initialized) {
        SDL_Quit();
        m_initialized = false;
    }
}

void VulkanRenderer::createInstance() {
    unsigned int extCount = 0;
    if (!SDL_Vulkan_GetInstanceExtensions(m_window, &extCount, nullptr)) {
        throw std::runtime_error("Failed to get Vulkan extensions from SDL");
    }
    std::vector<const char*> extensions(extCount);
    SDL_Vulkan_GetInstanceExtensions(m_window, &extCount, extensions.data());

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = m_title.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Retronomicon";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

void VulkanRenderer::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("No Vulkan-capable GPU found");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    m_physicalDevice = devices[0]; // TODO: better selection
}

void VulkanRenderer::createLogicalDevice() {
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = 0; // TODO: query correct family
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }

    vkGetDeviceQueue(m_device, 0, 0, &m_graphicsQueue);
    m_presentQueue = m_graphicsQueue; // TODO: separate if needed
}

} // namespace retronomicon::vulkan
