#pragma once
#include "Shader/Shader.h"
#include "models/Vertex.h"
#include <vector>

class Circle {

private:
	float nRadius;
	Vertex centre;
	float thickness;
	Shader shader;
	unsigned int vao;

public:
	Circle(float nRadius, const Vertex& centre, float thickness = 0.015f);
	~Circle();
	void draw();

private:
	void initializeVAO();
	std::vector<Vertex> generateBoundingSquareVertices();
};

