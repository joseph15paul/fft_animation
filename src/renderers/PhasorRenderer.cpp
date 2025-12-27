#include "renderers/PhasorRenderer.h"
#include "models/Vertex.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

PhasorRenderer::PhasorRenderer(Shader shader, Vertex centre, float thickness)
    : shader(shader), thickness(thickness), centre(centre) {
  initializeVAO();
}

PhasorRenderer::~PhasorRenderer() { glDeleteVertexArrays(1, &vao); }

void PhasorRenderer::draw(const Phasor &phasor, glm::mat4 transform) {
  shader.use();
  transform =
      glm::rotate(transform, phasor.getPhase(), glm::vec3(0.0, 0.0, 1.0));
  transform = glm::scale(transform, glm::vec3{phasor.getAmplitude(), phasor.getAmplitude(), 1.0});
  shader.setMat4("transform", transform);
  shader.setVec3("uColor", centre.color);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void PhasorRenderer::initializeVAO() {
  auto vertices = generateVertices(centre, thickness);
  std::vector<unsigned int> indices = {// Rectangle
                                       0, 1, 3, 1, 2, 3,

                                       // Arrow head
                                       4, 5, 6};

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoord));
  glEnableVertexAttribArray(1);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

std::vector<Vertex> PhasorRenderer::generateVertices(Vertex centre,
                                                     float thickness) {
  float halfThick = thickness / 2;
  float bodyLength = 0.09f;
  float headLength = 0.01f;
  float headHeight = thickness + 0.005f;

  Vertex topLeft = {centre.position + glm::vec3(0.0f, halfThick, 0.0f),
                    centre.color, centre.texCoord};
  Vertex topRight = {centre.position + glm::vec3(bodyLength, halfThick, 0.0f),
                     centre.color, centre.texCoord};
  Vertex bottomRight = {centre.position +
                            glm::vec3(bodyLength, -halfThick, 0.0f),
                        centre.color, centre.texCoord};
  Vertex bottomLeft = {centre.position + glm::vec3(0.0f, -halfThick, 0.0f),
                       centre.color, centre.texCoord};

  Vertex headBaseTop = {centre.position +
                            glm::vec3(bodyLength, headHeight * 0.5f, 0.0f),
                        centre.color, centre.texCoord};
  Vertex headBaseBottom = {centre.position +
                               glm::vec3(bodyLength, -headHeight * 0.5f, 0.0f),
                           centre.color, centre.texCoord};
  Vertex headTip = {centre.position +
                        glm::vec3(bodyLength + headLength, 0.0f, 0.0f),
                    centre.color, centre.texCoord};

  return std::vector<Vertex>{topLeft,    topRight,    bottomRight,
                             bottomLeft, headBaseTop, headBaseBottom,
                             headTip};
}
