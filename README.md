# Crimson Engine

A modern Vulkan-based game engine scaffold with ImGui integration, designed for high-performance graphics applications.

## Features

- **Vulkan Rendering**: Modern graphics API with validation layers for debugging
- **ImGui Integration**: Immediate mode GUI for debugging and tools
- **GLFW Window Management**: Cross-platform window and input handling
- **GLM Math Library**: OpenGL Mathematics for vector and matrix operations
- **CMake Build System**: Modern CMake configuration with presets for Visual Studio 2022
- **Modular Architecture**: Clean separation of concerns with dedicated classes for engine components

## Prerequisites

### Windows (Visual Studio 2022)

1. **Visual Studio 2022**
   - Install with "Desktop development with C++" workload
   - Ensure CMake tools are included

2. **Vulkan SDK**
   - Download and install from [LunarG](https://vulkan.lunarg.com/)
   - Version 1.2 or higher required
   - Ensure `VULKAN_SDK` environment variable is set

3. **Git**
   - Required for cloning dependencies

## Project Structure

```
CrimsonEngine/
├── CMakeLists.txt           # Root CMake configuration
├── CMakePresets.json        # CMake presets for Visual Studio
├── README.md                # This file
├── .gitignore              # Git ignore patterns
├── src/                    # Source files
│   ├── main.cpp            # Application entry point
│   ├── Engine.cpp          # Core engine implementation
│   ├── VulkanContext.cpp   # Vulkan initialization and management
│   └── ImGuiManager.cpp    # ImGui setup and rendering
├── include/                # Header files
│   ├── Engine.h
│   ├── VulkanContext.h
│   └── ImGuiManager.h
├── third_party/            # External dependencies
│   ├── glfw/               # GLFW library
│   ├── glm/                # GLM math library
│   └── imgui/              # Dear ImGui library
├── CMake/                  # CMake modules (optional)
└── presets/                # Configuration presets (optional)
```

## Building the Project

### Using Visual Studio 2022

#### Method 1: CMake Presets (Recommended)

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd CrimsonEngine
   ```

2. **Open in Visual Studio**
   - File → Open → CMake...
   - Select the `CMakeLists.txt` file
   - Visual Studio will automatically detect the CMake presets

3. **Select Configuration**
   - Use the configuration dropdown to select:
     - `windows-debug` for debug builds
     - `windows-release` for release builds

4. **Build**
   - Build → Build All (Ctrl+Shift+B)
   - Or use the Build button in the toolbar

5. **Run**
   - Select `CrimsonEngine.exe` as the startup item
   - Debug → Start Debugging (F5) or Start Without Debugging (Ctrl+F5)

#### Method 2: Command Line

1. **Configure**
   ```bash
   cmake --preset windows-debug
   ```

2. **Build**
   ```bash
   cmake --build --preset windows-debug
   ```

3. **Run**
   ```bash
   build\windows-debug\bin\Debug\CrimsonEngine.exe
   ```

### Build Configurations

- **Debug**: Includes validation layers, debug symbols, and runtime checks
- **Release**: Optimized build for performance testing

## Running the Application

When you run the application, you should see:

1. A window titled "Crimson Engine" (800x600)
2. A dark blue/purple clear color background
3. ImGui Demo Window showing various UI components
4. A "Crimson Engine Info" window displaying FPS counter
5. Console output showing FPS and selected GPU information

### Controls

- **ESC or Close Button**: Exit the application
- **ImGui Windows**: Click and drag to move, resize from edges
- **ImGui Demo**: Explore various UI widgets and features

## Architecture Overview

### Engine Class (`Engine.h/cpp`)
- Main application controller
- Manages initialization and main loop
- Handles window creation via GLFW
- Coordinates Vulkan and ImGui subsystems
- Implements frame timing and FPS display

### VulkanContext Class (`VulkanContext.h/cpp`)
- Encapsulates all Vulkan functionality
- Manages instance, device, and queues
- Handles swap chain creation and recreation
- Provides validation layers in debug mode
- Creates render passes and framebuffers

### ImGuiManager Class (`ImGuiManager.h/cpp`)
- Sets up ImGui with Vulkan backend
- Manages ImGui frame lifecycle
- Renders ImGui draw data to command buffers
- Provides demo window and engine info display

## Dependencies

All dependencies are included as Git submodules in the `third_party` directory:

- **GLFW 3.4**: Window and input management
- **GLM 1.0.1**: Mathematics library
- **Dear ImGui 1.91.5**: Immediate mode GUI
- **Vulkan SDK**: Graphics API (system-installed)

## Development

### Adding New Features

1. **New Classes**: Add headers to `include/` and implementations to `src/`
2. **Update CMakeLists.txt**: Add new source files to `ENGINE_SOURCES` and `ENGINE_HEADERS`
3. **Follow Existing Patterns**: Maintain separation of concerns and RAII principles

### Debugging

- Debug builds automatically enable Vulkan validation layers
- Check console output for validation layer messages
- Use Visual Studio debugger (F5) for breakpoints and inspection
- ImGui demo window is useful for testing UI features

### Common Issues

**Vulkan SDK not found**
- Ensure VULKAN_SDK environment variable is set
- Restart Visual Studio after installing Vulkan SDK
- Check that CMake can find Vulkan: `find_package(Vulkan REQUIRED)`

**Validation layers not available**
- Install Vulkan SDK with validation layers
- Ensure you're building in Debug configuration
- Check that `VK_LAYER_KHRONOS_validation` is available

**Window not appearing**
- Check console for error messages
- Verify GPU supports Vulkan 1.2
- Try updating graphics drivers

## Performance

- **Debug Mode**: ~60-120 FPS (with validation layers)
- **Release Mode**: ~500-1000+ FPS (hardware dependent)

FPS is displayed in the console every second and in the ImGui info window.

## Future Enhancements

Potential areas for expansion:

- [ ] Graphics pipeline for rendering primitives
- [ ] Shader compilation and management
- [ ] Texture loading and management
- [ ] Camera system
- [ ] Input handling abstraction
- [ ] Scene graph
- [ ] Entity Component System (ECS)
- [ ] Asset loading system
- [ ] Physics integration
- [ ] Audio system

## License

[Specify your license here]

## Contributing

[Add contribution guidelines if applicable]

## Acknowledgments

- [Vulkan Tutorial](https://vulkan-tutorial.com/) for excellent Vulkan documentation
- [Dear ImGui](https://github.com/ocornut/imgui) for the GUI library
- [GLFW](https://www.glfw.org/) for window management
- [GLM](https://github.com/g-truc/glm) for mathematics

## Contact

[Add your contact information]
