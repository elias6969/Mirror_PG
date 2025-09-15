#ifndef TORUS_H
#define TORUS_H

#include "Camera.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Torus {
public:
  Torus(float majorRadius = 1.0f, float minorRadius = 0.3f,
        int sectorCount = 36, int sideCount = 18);

  void draw(Camera &camera, glm::vec3 lightPoss);
  ~Torus();

  glm::vec3 color;
private:
  void generateTorus();
  void setupMesh();

  void addVertex(float x, float y, float z);
  void addNormal(float nx, float ny, float nz);
  void addTexCoord(float s, float t);
  void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

  Shader shader;
  GLuint vao, vbo, ebo;
  GLuint texture;

  float majorRadius, minorRadius;
  int sectorCount, sideCount;

  std::vector<float> vertices; // interleaved pos+normal+uv
  std::vector<unsigned int> indices;
  glm::vec3 position;
};

#endif
