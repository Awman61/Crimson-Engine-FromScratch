# Build Notes

## Dependency Management

This project now uses **CMake FetchContent** for automatic dependency management.

- All dependencies are downloaded automatically during CMake configuration
- No manual git cloning required
- No local third-party directory needed
- Works out of the box: clone repo → run CMake → everything works

### Dependencies

- **GLFW 3.4**: Windowing and input
- **GLM 1.0.0**: Math library  
- **Dear ImGui (Docking)**: UI library with GLFW/Vulkan backends
- **Vulkan SDK**: Graphics API (must be installed separately)

### Build Process

1. Install Vulkan SDK from [LunarG](https://vulkan.lunarg.com/)
2. Clone this repository
3. Run CMake (all dependencies will be downloaded automatically)
4. Build and run

### Notes

- GLFW tests/examples/docs are disabled via cache variables
- GLM is configured as header-only library
- ImGui is built as a static library with GLFW and Vulkan backends
- All dependencies use specific git tags for reproducibility