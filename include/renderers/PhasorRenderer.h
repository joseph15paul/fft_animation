#pragma once
#include "Shader/Shader.h"
#include "models/Vertex.h"
#include "phasor/Phasor.h"

class PhasorRenderer {
  Shader shader;
  unsigned int vao = 0;
  Vertex centre;
  float thickness;

public:
  PhasorRenderer(Shader shader = Shader("src/shaders/commonShader.vert",
                                        "src/shaders/commonShader.frag"),
                 Vertex centre = {glm::vec3(0.0f), glm::vec3(1.0f),
                                  glm::vec2(0.0f)},
                 float thickness = 0.002f);
  ~PhasorRenderer();

  void draw(const Phasor &phasor, glm::mat4 transform = glm::mat4(1.0));

private:
  void initializeVAO();
  std::vector<Vertex> generateVertices(Vertex centre, float thickness);
};
