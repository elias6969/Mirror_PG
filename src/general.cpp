#include "Camera.h"
#include "Cube.h"
#include "Material.h"
#include "Sphere.h"
#include "Variables.h"
#include "textureManager.h"
#include <glad/glad.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#define _USE_MATH_DEFINES
#include "Cone.h"
#include "Torus.h"
#include <cmath>

float cubeVertices[] = {
    // positions          // normals        // texcoords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

void Cube::loadCube() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texcoords attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load((PathManager::texturePath + "test1.jpeg").c_str(), &width,
                &height, &nrChannels, 0);
  if (data) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    int format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
  }
  stbi_image_free(data);
  shader.LoadShaders((PathManager::shaderPath + "basiccube.vs").c_str(),
                     (PathManager::shaderPath + "basiccube.fs").c_str());
  Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Cube::render(Camera &camera, GLFWwindow *window) {
  shader.use();

  glm::mat4 projection = glm::perspective(
      glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
  glm::mat4 view = camera.GetViewMatrix();

  shader.setMat4("projection", projection);
  shader.setMat4("view", view);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, Position);
  shader.setMat4("model", model);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  shader.setInt("diffuseTex", 0);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

Cube::~Cube() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteTextures(1, &texture);
}

Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount)
    : radius(radius), sectorCount(sectorCount), stackCount(stackCount) {
  shader.LoadShaders((PathManager::shaderPath + "sphere.vs").c_str(),
                     (PathManager::shaderPath + "sphere.fs").c_str());

  generateSphere();
  setupMesh();
}

Sphere::~Sphere() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Sphere::generateSphere() {
  float x, y, z, xy;
  float sectorStep = 2 * M_PI / sectorCount;
  float stackStep = M_PI / stackCount;
  float sectorAngle, stackAngle;

  vertices.clear();
  indices.clear();

  for (unsigned int i = 0; i <= stackCount; ++i) {
    stackAngle = M_PI / 2 - i * stackStep;
    xy = radius * cosf(stackAngle);
    y = radius * sinf(stackAngle);

    for (unsigned int j = 0; j <= sectorCount; ++j) {
      sectorAngle = j * sectorStep;
      float u = (float)j / sectorCount;
      float v = (float)i / stackCount;

      x = xy * cosf(sectorAngle);
      z = xy * sinf(sectorAngle);

      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
      vertices.push_back(normal.x);
      vertices.push_back(normal.y);
      vertices.push_back(normal.z);

      vertices.push_back(u);
      vertices.push_back(v);
    }
  }

  for (unsigned int i = 0; i < stackCount; ++i) {
    for (unsigned int j = 0; j < sectorCount; ++j) {

      int first = i * (sectorCount + 1) + j;
      int second = first + sectorCount + 1;

      indices.push_back(first);
      indices.push_back(second);
      indices.push_back(first + 1);

      indices.push_back(second);
      indices.push_back(second + 1);
      indices.push_back(first + 1);
    }
  }
}

void Sphere::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texcoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  texture = TextureLoader::loadTexture(
      (PathManager::texturePath + "green.jpeg").c_str(), true);
  shader.setInt("texture1", 0);
  glBindVertexArray(0);
  position = glm::vec3(5.0f, 1.0f, 0.5f);
}

void Sphere::Draw(Camera &camera, glm::vec3 lightPos) {
  shader.use();
  glm::mat4 projection = glm::perspective(
      glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(glm::mat4(1.0f), position);
  shader.setVec3("light.position", lightPos);
  shader.setVec3("viewPos", camera.Position);

  // light properties
  shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  // material properties
  shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.setFloat("material.shininess", 64.0f);

  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
  shader.setVec3("color", color);

  shader.setInt("useTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

Torus::Torus(float majorRadius, float minorRadius, int sectorCount,
             int sideCount)
    : majorRadius(majorRadius), minorRadius(minorRadius),
      sectorCount(sectorCount), sideCount(sideCount) {
  shader.LoadShaders((PathManager::shaderPath + "sphere.vs").c_str(),
                     (PathManager::shaderPath + "sphere.fs").c_str());

  generateTorus();
  setupMesh();
}

Torus::~Torus() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

void Torus::addVertex(float x, float y, float z) {
  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(z);
}
void Torus::addNormal(float nx, float ny, float nz) {
  vertices.push_back(nx);
  vertices.push_back(ny);
  vertices.push_back(nz);
}
void Torus::addTexCoord(float s, float t) {
  vertices.push_back(s);
  vertices.push_back(t);
}
void Torus::addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
  indices.push_back(i1);
  indices.push_back(i2);
  indices.push_back(i3);
}

void Torus::generateTorus() {
  vertices.clear();
  indices.clear();

  float sectorStep = 2 * M_PI / sectorCount;
  float sideStep = 2 * M_PI / sideCount;

  for (int i = 0; i <= sideCount; i++) {
    float sideAngle = i * sideStep;
    float cosu = cosf(sideAngle), sinu = sinf(sideAngle);

    for (int j = 0; j <= sectorCount; j++) {
      float sectorAngle = j * sectorStep;
      float cosv = cosf(sectorAngle), sinv = sinf(sectorAngle);

      float x = (majorRadius + minorRadius * cosu) * cosv;
      float y = (majorRadius + minorRadius * cosu) * sinv;
      float z = minorRadius * sinu;

      float nx = cosu * cosv;
      float ny = cosu * sinv;
      float nz = sinu;

      addVertex(x, y, z);
      addNormal(nx, ny, nz);
      addTexCoord((float)j / sectorCount, (float)i / sideCount);
    }
  }

  for (int i = 0; i < sideCount; i++) {
    int k1 = i * (sectorCount + 1);
    int k2 = k1 + sectorCount + 1;

    for (int j = 0; j < sectorCount; j++, k1++, k2++) {
      addIndices(k1, k2, k1 + 1);
      addIndices(k1 + 1, k2, k2 + 1);
    }
  }
}

void Torus::setupMesh() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  int stride = 8 * sizeof(float); // pos(3) + normal(3) + uv(2)

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(0);

  // Normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texcoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  texture = TextureLoader::loadTexture(
      (PathManager::texturePath + "question.jpeg").c_str());
  position = glm::vec3(0.0f);
}

void Torus::draw(Camera &camera, glm::vec3 lightPos) {
  shader.use();
  glm::mat4 projection = glm::perspective(
      glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);

  shader.setMat4("projection", projection);
  shader.setMat4("view", view);
  shader.setMat4("model", model);

  shader.setVec3("light.position", lightPos);
  shader.setVec3("viewPos", camera.Position);

  // light properties
  shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  // material properties
  shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.setFloat("material.shininess", 64.0f);

  shader.setVec3("color", color);
  shader.setInt("useTexture", 1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

std::map<int, std::vector<float>> Cone::circleCache;

Cone::Cone(int stackCount, float baseRadius, float height, int sectorCount)
    : stackCount(stackCount), baseRadius(baseRadius), height(height),
      sectorCount(sectorCount) {

  shader.LoadShaders((PathManager::shaderPath + "spheretest.vs").c_str(),
                     (PathManager::shaderPath + "spheretest.fs").c_str());
  material.shader = &shader;
  const std::vector<float> &circle = getUnitCircle(sectorCount);

  buildVerticesSmooth();
  setupMesh();

  texture = TextureLoader::loadTexture(
      (PathManager::texturePath + "question.jpeg").c_str());
  material.textures.push_back({texture, "diffuse", 0});
}

const std::vector<float> &Cone::getUnitCircle(int sectorCount) {
  auto it = circleCache.find(sectorCount);
  if (it != circleCache.end()) {
    return it->second; // already computed
  }

  // build new circle
  std::vector<float> unitCircle;
  float sectorStep = 2 * M_PI / sectorCount;

  for (int i = 0; i <= sectorCount; ++i) {
    float sectorAngle = i * sectorStep;
    unitCircle.push_back(cos(sectorAngle)); // x
    unitCircle.push_back(sin(sectorAngle)); // y
    unitCircle.push_back(0.0f);             // z
  }

  // insert into cache
  circleCache[sectorCount] = unitCircle;
  return circleCache[sectorCount];
}
Cone::~Cone() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteTextures(1, &texture);
}

std::vector<float> Cone::getSideNormals() {
  const float PI = acos(-1.0f);
  float sectorStep = 2 * M_PI / sectorCount;

  // normal at 0 degree
  float zAngle = atan2(baseRadius, height);
  float x0 = cos(zAngle); // nx
  float z0 = sin(zAngle); // nz

  std::vector<float> normals;
  for (int i = 0; i <= sectorCount; ++i) {
    float sectorAngle = i * sectorStep;
    normals.push_back(cos(sectorAngle) * x0);
    normals.push_back(sin(sectorAngle) * x0);
    normals.push_back(z0);
  }
  return normals;
}

void Cone::buildVerticesSmooth() {
  const std::vector<float> &unitCircleVertices = getUnitCircle(sectorCount);
  float x, y, z;
  float radius;

  std::vector<float> sideNormals = getSideNormals();

  // sides
  for (int i = 0; i <= stackCount; ++i) {
    z = -(height * 0.5f) + (float)i / stackCount * height;
    radius = baseRadius * (1.0f - (float)i / stackCount);
    float t = 1.0f - (float)i / stackCount;

    for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3) {
      x = unitCircleVertices[k];
      y = unitCircleVertices[k + 1];

      addVertexData(x * radius, y * radius, z, sideNormals[k],
                    sideNormals[k + 1], sideNormals[k + 2],
                    (float)j / sectorCount, t);
    }
  }

  // base
  unsigned int baseVertexIndex =
      (unsigned int)(vertices.size() / 8); // 8 floats per vertex
  z = -height * 0.5f;

  // center
  addVertexData(0, 0, z, 0, 0, -1, 0.5f, 0.5f);

  for (int i = 0, j = 0; i < sectorCount; ++i, j += 3) {
    x = unitCircleVertices[j];
    y = unitCircleVertices[j + 1];
    addVertexData(x * baseRadius, y * baseRadius, z, 0, 0, -1, -x * 0.5f + 0.5f,
                  -y * 0.5f + 0.5f);
  }

  // side indices
  unsigned int k1, k2;
  for (int i = 0; i < stackCount; ++i) {
    k1 = i * (sectorCount + 1);
    k2 = k1 + sectorCount + 1;

    for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
      addIndices(k1, k1 + 1, k2);
      addIndices(k2, k1 + 1, k2 + 1);
    }
  }

  // base indices
  // baseIndex = (unsigned int)indices.size();
  for (int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k) {
    if (i < (sectorCount - 1))
      addIndices(baseVertexIndex, k + 1, k);
    else
      addIndices(baseVertexIndex, baseVertexIndex + 1, k);
  }
}

void Cone::addVertexData(float x, float y, float z, float nx, float ny,
                         float nz, float s, float t) {
  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(z);

  vertices.push_back(nx);
  vertices.push_back(ny);
  vertices.push_back(nz);

  vertices.push_back(s);
  vertices.push_back(t);
}

void Cone::addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
  indices.push_back(i1);
  indices.push_back(i2);
  indices.push_back(i3);
}

void Cone::setupMesh() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  int stride = 8 * sizeof(float);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(0);

  // normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texcoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Cone::draw(Camera &cam, glm::vec3 lightPos) {
  glm::mat4 projection = 
    glm::perspective(glm::radians(cam.Zoom),
                                          (float)PathManager::SCR_WIDTH /
                                          (float)PathManager::SCR_HEIGHT,
                                          0.1f, 100.0f);
  glm::mat4 view = cam.GetViewMatrix();
  glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

  // material.apply(cam, model, lightPos, projection, view);
  material.shader->use();
  material.shader->setMat4("projection", projection);
  material.shader->setMat4("view", view);
  material.shader->setMat4("model", model);

  material.vec3Uniforms["light.position"] = lightPos;
  material.vec3Uniforms["viewPos"] = cam.Position;
  material.vec3Uniforms["color"] = color;
  material.floatUniforms["material.shininess"] = 32.0f;
  material.shader->setInt("texture_diffuse", 0);
  material.shader->setInt("useTexture", 1);

  // bind textures + all uniforms
  material.apply();

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
