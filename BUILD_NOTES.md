# Build Notes

## Dependency Management (Updated)

This project now uses CMake FetchContent for automatic dependency downloading:

- **GLFW 3.4**: Automatically downloaded from GitHub (https://github.com/glfw/glfw) on first CMake configuration
- **GLM 0.9.9.8**: Automatically downloaded from GitHub (https://github.com/g-truc/glm) on first CMake configuration
- **Dear ImGui (Docking)**: Automatically downloaded from GitHub (https://github.com/ocornut/imgui) using `GIT_TAG docking`

### First-time Build

When running CMake for the first time, it will automatically download GLFW, GLM, and Dear ImGui from their respective GitHub repositories. This may take a few moments depending on your internet connection.

### Old Third-Party Dependencies

The following directories in third_party are now deprecated and no longer used by the build system:
- third_party/glfw
- third_party/glm
- third_party/imgui

These can be safely removed from the repository if desired, as FetchContent will download fresh copies to the build directory (_build/_deps/).

### Updating Dependencies

To update to newer versions, modify the GIT_TAG values in CMakeLists.txt:
- GLFW: Change `GIT_TAG 3.4` to a newer version tag
- GLM: Change `GIT_TAG 0.9.9.8` to a newer version tag
- ImGui: Change `GIT_TAG docking` to a newer tag/commit (or keep `docking` to follow the docking branch)

Then clean your build directory and reconfigure with CMake.
