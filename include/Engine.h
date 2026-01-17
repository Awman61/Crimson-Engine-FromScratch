#pragma once

#include <memory>
#include <string>

class VulkanContext;
class ImGuiManager;
struct GLFWwindow;

class Engine {
public:
    Engine(int width, int height, const std::string& title);
    ~Engine();

    void run();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    bool m_framebufferResized;

private:
    void initWindow();
    void initVulkan();
    void initImGui();
    void mainLoop();
    void cleanup();

    void drawFrame();
    void recordCommandBuffer(uint32_t imageIndex);

    int m_width;
    int m_height;
    std::string m_title;

    GLFWwindow* m_window;
    std::unique_ptr<VulkanContext> m_vulkanContext;
    std::unique_ptr<ImGuiManager> m_imguiManager;
};
