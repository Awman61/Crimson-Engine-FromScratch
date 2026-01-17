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
# Third-party dependencies (no network calls; expected to exist locally)
# -----------------------------------------------------------------------------

# GLFW
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/CMakeLists.txt")
    message(FATAL_ERROR
        "GLFW not found. Please clone it to third_party/glfw (e.g. git clone https://github.com/glfw/glfw.git third_party/glfw)")
endif()
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# GLM
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm/CMakeLists.txt")
    message(FATAL_ERROR
        "GLM not found. Please clone it to third_party/glm (e.g. git clone https://github.com/g-truc/glm.git third_party/glm)")
endif()
add_subdirectory(third_party/glm)

# Dear ImGui (Docking) - clone docking branch to third_party/imgui
add_subdirectory(third_party/imgui_cmake)

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
    glm::glm
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
