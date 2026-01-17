#pragma once

#include <vulkan/vulkan.h>

class VulkanContext;
struct GLFWwindow;

class ImGuiManager {
public:
    ImGuiManager(GLFWwindow* window, VulkanContext* vulkanContext);
    ~ImGuiManager();

    void newFrame();
    void render(VkCommandBuffer commandBuffer);

    ImGuiManager(const ImGuiManager&) = delete;
    ImGuiManager& operator=(const ImGuiManager&) = delete;

private:
    GLFWwindow* m_window;
    VulkanContext* m_vulkanContext;
};
