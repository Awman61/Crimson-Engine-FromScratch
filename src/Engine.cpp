#include "Engine.h"
#include "VulkanContext.h"
#include "ImGuiManager.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <chrono>

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine) {
        engine->m_framebufferResized = true;
    }
}

Engine::Engine(int width, int height, const std::string& title)
    : m_width(width)
    , m_height(height)
    , m_title(title)
    , m_window(nullptr)
    , m_framebufferResized(false) {
}

Engine::~Engine() {
    cleanup();
}

void Engine::run() {
    initWindow();
    initVulkan();
    initImGui();
    mainLoop();
}

void Engine::initWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void Engine::initVulkan() {
    m_vulkanContext = std::make_unique<VulkanContext>(m_window);
}

void Engine::initImGui() {
    m_imguiManager = std::make_unique<ImGuiManager>(m_window, m_vulkanContext.get());
}

void Engine::mainLoop() {
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        auto currentTime = std::chrono::high_resolution_clock::now();
        frameCount++;

        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        if (deltaTime >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        drawFrame();
    }

    m_vulkanContext->waitIdle();
}

void Engine::drawFrame() {
    VkDevice device = m_vulkanContext->getDevice();
    VkSwapchainKHR swapChain = m_vulkanContext->getSwapChain();

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        m_vulkanContext->recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    VkCommandPool commandPool = m_vulkanContext->getCommandPool();
    VkCommandBuffer commandBuffer;

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffer");
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_vulkanContext->getRenderPass();
    renderPassInfo.framebuffer = m_vulkanContext->getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_vulkanContext->getSwapChainExtent();

    VkClearValue clearColor = {{{0.1f, 0.1f, 0.15f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    m_imguiManager->render(commandBuffer);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkQueue graphicsQueue = m_vulkanContext->getGraphicsQueue();
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &imageIndex;

    VkQueue presentQueue = m_vulkanContext->getPresentQueue();
    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized) {
        m_framebufferResized = false;
        m_vulkanContext->recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image");
    }

    vkQueueWaitIdle(presentQueue);
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void Engine::cleanup() {
    m_imguiManager.reset();
    m_vulkanContext.reset();

    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}
