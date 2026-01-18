cmake_minimum_required(VERSION 3.20)

project(CrimsonEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(MSVC)
    add_compile_options(/W4 /permissive-)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Vulkan REQUIRED)

# -----------------------------------------------------------------------------
# FetchContent dependencies (automatic download and configuration)
# -----------------------------------------------------------------------------

include(FetchContent)

# Set cache variables for GLFW before FetchContent_MakeAvailable
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Set cache variables for GLM before FetchContent_MakeAvailable
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW CACHE STRING "" FORCE)

# Declare all dependencies
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.0
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)

# Make dependencies available in order
FetchContent_MakeAvailable(glfw glm imgui)

# Create ImGui target after imgui is available
add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

target_sources(imgui PRIVATE
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp"
)

target_include_directories(imgui PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
)

target_link_libraries(imgui PUBLIC
    Vulkan::Vulkan
    glfw
)

target_compile_features(imgui PUBLIC cxx_std_17)

# -----------------------------------------------------------------------------
# Engine target
# -----------------------------------------------------------------------------

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Engine.cpp
    src/VulkanContext.cpp
    src/ImGuiManager.cpp

    include/Engine.h
    include/VulkanContext.h
    include/ImGuiManager.h
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    glm
    imgui
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:VULKAN_DEBUG>
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
        NOMINMAX
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)