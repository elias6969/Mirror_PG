#ifndef CONE_H
#define CONE_H

#include "Camera.h"
#include "Material.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

class Cone {
public:
  Cone(int stackCount, float baseRadius, float height, int sectorCount = 36);
  ~Cone();

  void buildVerticesSmooth();
  static const std::vector<float> &getUnitCircle(int sectorCount);
  std::vector<float> getSideNormals();

  void setupMesh();
  void draw(Camera & camera, glm::vec3 lightPos);

  glm::vec3 color{1.0f, 1.0f, 1.0f};
  glm::vec3 position{0.0f};

private:
  Shader shader;
  Material material;
  void addVertexData(float x, float y, float z, float nx, float ny, float nz,
                     float s, float t);
  void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

  GLuint vao{0}, vbo{0}, ebo{0};
  GLuint texture{0};

  int stackCount;
  int sectorCount;
  float baseRadius;
  float height;

  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  // ---- Caching static members ----
  static std::map<int, std::vector<float>> circleCache;

  glm::mat4 projection, view, model;

};

#endif // CONE_H
