#include "ImGuiManager.h"
#include "VulkanContext.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

ImGuiManager::ImGuiManager(GLFWwindow* window, VulkanContext* vulkanContext)
    : m_window(window)
    , m_vulkanContext(vulkanContext) {
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = vulkanContext->getInstance();
    initInfo.PhysicalDevice = vulkanContext->getPhysicalDevice();
    initInfo.Device = vulkanContext->getDevice();
    initInfo.QueueFamily = vulkanContext->getQueueFamilies().graphicsFamily.value();
    initInfo.Queue = vulkanContext->getGraphicsQueue();
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = vulkanContext->getDescriptorPool();
    initInfo.Subpass = 0;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = static_cast<uint32_t>(vulkanContext->getSwapChainImageViews().size());
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    initInfo.Allocator = nullptr;
    initInfo.CheckVkResultFn = nullptr;

    ImGui_ImplVulkan_Init(&initInfo, vulkanContext->getRenderPass());

    VkCommandBuffer commandBuffer;
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vulkanContext->getCommandPool();
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(vulkanContext->getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(vulkanContext->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vulkanContext->getGraphicsQueue());

    vkFreeCommandBuffers(vulkanContext->getDevice(), vulkanContext->getCommandPool(), 1, &commandBuffer);

    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

ImGuiManager::~ImGuiManager() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::newFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Begin("Crimson Engine Info");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                1000.0f / ImGui::GetIO().Framerate, 
                ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
}

void ImGuiManager::render(VkCommandBuffer commandBuffer) {
    newFrame();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
}
