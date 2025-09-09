#ifndef MIRROR_DEMO_H
#define MIRROR_DEMO_H

#include <iostream>
#include "Camera.h"
#include "Shader.h"

class Mirror {
public:
  void initMirror(int width, int height);
  ~Mirror();
  void renderMirror(Camera &camera);
  unsigned int mirrorFBO, mirrorTex;
  unsigned int mirrorVAO, mirrorVBO;
private:
  std::string shaderPath;
  Shader mirrorShader;
};
#endif // !MIRROR_DEMO_H
