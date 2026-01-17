#include "Engine.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Engine engine(800, 600, "Crimson Engine");
        engine.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
