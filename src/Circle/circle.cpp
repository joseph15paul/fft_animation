#include "Circle/circle.h"
#include "models/Vertex.h"
#include <Shader/Shader.h>
#include <glad/glad.h>
#include <vector>

Circle::Circle(float nRadius, const Vertex &centre, float thickness)
    : shader("src/shaders/Circle/shader.vert", "src/shaders/Circle/shader.frag"),
      nRadius(nRadius), centre(centre), thickness(thickness) {
  initializeVAO();
}

Circle::~Circle() 
{
  glDeleteVertexArrays(1, &vao);
}

void Circle::draw() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  shader.use();
  shader.setVec2("uCenter", centre.position.x, centre.position.y);
  shader.setFloat("uRadius", nRadius);
  shader.setFloat("uThickness", thickness);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Circle::initializeVAO() {
  auto vertices = generateBoundingSquareVertices();
  auto indices = std::vector<unsigned int>{0, 1, 3, 1, 2, 3};

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

std::vector<Vertex> Circle::generateBoundingSquareVertices() {
  Vertex topLeft = {glm::vec3(centre.position[0] - nRadius - thickness, centre.position[1] + nRadius + thickness, 0.0f), centre.color, centre.texCoord};
  Vertex topRight = {glm::vec3(centre.position[0] + nRadius + thickness, centre.position[1] + nRadius + thickness, 0.0f), centre.color, centre.texCoord};
  Vertex bottomRight = {glm::vec3(centre.position[0] + nRadius + thickness, centre.position[1] - nRadius - thickness, 0.0f), centre.color, centre.texCoord};
  Vertex bottomLeft = {glm::vec3(centre.position[0] - nRadius - thickness, centre.position[1] - nRadius - thickness, 0.0f), centre.color, centre.texCoord};
  std::vector<Vertex> squareVertices{topLeft, topRight, bottomRight, bottomLeft};
  return squareVertices;
}
