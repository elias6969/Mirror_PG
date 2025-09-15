#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"
#include "Torus.h"
#include "Cone.h"
#include "Variables.h"
#include "mirror_demo.h"
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Dear ImGui
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

// screen settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool cursorEnabled = false;

// function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

int main() {
  // glfw init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mirror", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  // Setup Dear ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  Cube cube;
  cube.loadCube();
  Mirror mirror;
  mirror.initMirror(SCR_WIDTH, SCR_HEIGHT);

  bool showDemo = false;
  float clearColor[3] = {0.2f, 0.3f, 0.3f};

  glm::vec3 planeNormal(0.0f, 0.0f, -1.0f);
  glm::vec3 pointOnPLane(0.0f, 0.0f, -5.0f);
  float planeD = -glm::dot(planeNormal, pointOnPLane);

  Sphere sphere(1, 36, 18);
  Torus torus(1.0f, 0.3f, 36, 18);
  Cone cone(6, 2.0f, 4.0f);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // per-frame timing
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    processInput(window);

    glBindFramebuffer(GL_FRAMEBUFFER, mirror.mirrorFBO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera reflectedCam =
        Camera::getReflectedCamera(camera, planeNormal, planeD);
    //cube.render(reflectedCam, window);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube.render(camera, window);
    //sphere.Draw(camera, cube.Position);
    //torus.draw(camera, cube.Position);
    cone.draw(camera, cube.Position);
    //mirror.renderMirror(camera);
    

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug Menu");
    ImGui::Text("Hello from ImGui!");
    ImGui::Checkbox("Show demo window", &showDemo);
    ImGui::ColorEdit4("Cone light color", glm::value_ptr(cone.color));
    ImGui::ColorEdit4("Sphere light color", glm::value_ptr(sphere.color));
    ImGui::ColorEdit4("Torus light color", glm::value_ptr(torus.color));
    ImGui::ColorEdit3("Clear color", clearColor);
    ImGui::SliderFloat("Light X", &cube.Position.x, -10.0f, 10.0f);
    ImGui::SliderFloat("Light Y", &cube.Position.y, -10.0f, 10.0f);
    ImGui::SliderFloat("Light Z", &cube.Position.z, -10.0f, 10.0f);
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::End();

    if (showDemo) {
      ImGui::ShowDemoWindow(&showDemo);
    }
    // ---------------------

    // render ImGui on top
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  static bool keyHeld = false; // prevents rapid toggle while holding key
  // toggle with F or G
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
    if (!keyHeld) {
      cursorEnabled = !cursorEnabled;
      glfwSetInputMode(window, GLFW_CURSOR,
                       cursorEnabled ? GLFW_CURSOR_NORMAL
                                     : GLFW_CURSOR_DISABLED);
      keyHeld = true;
    }
  } else {
    // reset when no key pressed so it can be toggled again
    keyHeld = false;
  }

  // only move camera if cursor is disabled
  if (!cursorEnabled) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  PathManager::SCR_WIDTH = width;
  PathManager::SCR_HEIGHT = height;
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (cursorEnabled) {
    firstMouse = true;
    return;
  }
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}
