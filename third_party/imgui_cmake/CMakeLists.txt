# Dear ImGui (Docking) wrapper target.
#
# Expected layout:
#   third_party/imgui/         -> clone of https://github.com/ocornut/imgui (docking branch)
#   third_party/imgui_cmake/   -> this CMake wrapper (tracked by this repository)

set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../imgui" CACHE PATH "Path to Dear ImGui source directory")

if(NOT EXISTS "${IMGUI_DIR}/imgui.cpp")
    message(FATAL_ERROR
        "Dear ImGui sources not found at '${IMGUI_DIR}'.\n"
        "Clone the docking branch into third_party/imgui:\n"
        "  git clone -b docking https://github.com/ocornut/imgui.git third_party/imgui")
endif()

add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

target_sources(imgui PRIVATE
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp"
)

target_include_directories(imgui PUBLIC
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
)

target_link_libraries(imgui PUBLIC
    Vulkan::Vulkan
    glfw
)

target_compile_features(imgui PUBLIC cxx_std_17)
