# Build Notes

## Dependency Management

This project uses **local** third-party dependencies via `add_subdirectory()`.

- No FetchContent
- No automatic downloads
- CMake will not make any network calls

### One-time dependency setup

Clone the required dependencies into `third_party/`:

```bash
git clone https://github.com/glfw/glfw.git third_party/glfw
git clone https://github.com/g-truc/glm.git third_party/glm
git clone -b docking https://github.com/ocornut/imgui.git third_party/imgui
```

### Notes

- GLFW tests/examples/docs are disabled in CMake.
- ImGui is built as a small static library (core + GLFW/Vulkan backends) via `third_party/imgui_cmake/`.
