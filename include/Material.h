#ifndef MATERIAL_H
#define MATERIAL_H

#include "Camera.h"
#include "Shader.h"
#include <glad/glad.h>
#include <map>
#include <string>
#include <vector>

struct TextureInfo {
  GLuint id = 0;
  std::string type;
  int unit = 0;
};

class Material {
public:
  Shader *shader = nullptr;
  std::vector<TextureInfo> textures;
  std::map<std::string, glm::vec3> vec3Uniforms;
  std::map<std::string, float> floatUniforms;
  std::map<std::string, int> intUniforms;

  void apply() {
    if (!shader)
      return;

    shader->use();

    // Bind all textures
    for (auto &tex : textures) {
      glActiveTexture(GL_TEXTURE0 + tex.unit);
      glBindTexture(GL_TEXTURE_2D, tex.id);
      shader->setInt(("texture_" + tex.type).c_str(), tex.unit);
    }

    // Apply uniforms
    for (auto &[name, val] : vec3Uniforms)
      shader->setVec3(name, val);
    for (auto &[name, val] : floatUniforms)
      shader->setFloat(name, val);
    for (auto &[name, val] : intUniforms)
      shader->setInt(name, val);
  }
};

#endif // MATERIAL_H
