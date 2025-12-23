#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position; // 12 bytes
    glm::vec3 color;    // 12 bytes
    glm::vec2 texCoord; // 8 bytes
};

