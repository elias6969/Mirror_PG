#ifndef SPHERE_H
#define SPHERE_H

#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"

class Sphere {
public:
  Sphere(float radius = 1.0f, unsigned int sectorCount = 36,
         unsigned int stackCount = 18);

  void Draw(Camera &camera, glm::vec3 lightPos);

  ~Sphere();

  glm::vec3 color;

private:
  void generateSphere();
  void setupMesh();
  glm::vec3 position;

  float radius;
  GLuint texture;
  unsigned int sectorCount;
  unsigned int stackCount;

  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  GLuint VAO, VBO, EBO;

  Shader shader;
};

#endif
