#include "mirror_demo.h"
#include "Camera.h"
#include "Shader.h"
#include "filemanager.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// =======================
// Mirror Init
// =======================
void Mirror::initMirror(int width, int height) {
  // 1. framebuffer + texture
  glGenFramebuffers(1, &mirrorFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, mirrorFBO);

  glGenTextures(1, &mirrorTex);
  glBindTexture(GL_TEXTURE_2D, mirrorTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         mirrorTex, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Mirror not complete!\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // 2. mirror quad
  float quadVertices[] = {// positions   // texcoords
                          -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                          0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                          -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                          1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

  glGenVertexArrays(1, &mirrorVAO);
  glGenBuffers(1, &mirrorVBO);
  glBindVertexArray(mirrorVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mirrorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
  VirtualFileSystem vfs("../assets/");
  std::string shaderPath = vfs.getFullPath("Shaders/");
  // 3. load mirror shader
  mirrorShader.LoadShaders((shaderPath + "mirror.vs").c_str(),
                           (shaderPath + "mirror.fs").c_str());
  mirrorShader.use();
  mirrorShader.setInt("mirrorTex", 0);
}

// =======================
// Render Mirror Quad
// =======================
void Mirror::renderMirror(Camera &camera) {
  mirrorShader.use();

  glm::mat4 projection = glm::perspective(
      glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
  glm::mat4 view = camera.GetViewMatrix();

  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
  model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));

  mirrorShader.setMat4("model", model);
  mirrorShader.setMat4("view", view);
  mirrorShader.setMat4("projection", projection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mirrorTex);

  glBindVertexArray(mirrorVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

Mirror::~Mirror() {
  glDeleteVertexArrays(1, &mirrorVAO);
  glDeleteBuffers(1, &mirrorVBO);
  glDeleteTextures(1, &mirrorTex);
}
