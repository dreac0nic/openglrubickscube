#include <iostream>
#include <sstream>

#include "gl_core_4_4.hpp"
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "glslu.hpp"

#define VIEWPORT_WIDTH  640
#define VIEWPORT_HEIGHT 480

#define ERRLOG(errstr) std::cerr << "ERR [" << __FILE__ << ":" << __LINE__ << "] " << errstr << std::endl;

void glfw_err_callback(int code, const char* message);

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::mat4;
using glm::perspective;
using glm::translate;
using glm::scale;
using glm::rotate;
using glslu::Program;

typedef enum { MOUSE_RELEASED, MOUSE_LEFT_DRAG, MOUSE_RIGHT_DRAG } mouse_state;

int main(int argc, char* argv[])
{
  int window_width, window_height;
  GLFWwindow* hWindow;

  // Set error callback, because GLFW is being persnickety.
  glfwSetErrorCallback([](int code, const char* message) -> void {
    cerr << "GLFW ERR[" << code << "]: " << message; });

  // Load application frameworks...
  cerr << "INITIALIZING SYSTEMS" << endl
       << "--------------------" << endl;

  // TODO: Do stuff with window parameters or whatever.
  window_width = VIEWPORT_WIDTH;
  window_height = VIEWPORT_HEIGHT;

  // Initialize GLFW and check for errors.
  cerr << "\tGLFW ... \t";

  if(!glfwInit()) {
    ERRLOG("Could not initialize GLFW!");

    glfwTerminate();

    return -1;
  } else {
    cerr << "OK" << endl;
  }

  // Window Creation
  cerr << "\tWindow ... \t";

  // Window hints to ensure GLFW context is proper.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  // Attempt to create window based on context.
  hWindow = glfwCreateWindow(window_width, window_height, "WINDOW", NULL, NULL);
  if(!hWindow) {
    ERRLOG("Could not create a window!");

    glfwTerminate();
    return -1;
  } else {
    cerr << "OK" << endl;
  }

  // Focus window context.
  glfwMakeContextCurrent(hWindow);

  // Initialize GL using created loader.
  cerr << "\tLoad GL ... \t";

  if(!gl::sys::LoadFunctions()) {
    ERRLOG("Could not load OpenGL!");

    glfwDestroyWindow(hWindow);
    glfwTerminate();
    return -1;
  } else {
    cerr << "OK [v" << gl::sys::GetMajorVersion() << "." << gl::sys::GetMinorVersion() << endl;
  }

  // Attempt to get context
  cerr << "\tGL Context ... \t";

  if(!glfwGetCurrentContext()) {
    ERRLOG("Could not get context!");

    glfwDestroyWindow(hWindow);
    glfwTerminate();
    return -1;
  } else {
    cerr << "OK [v" << (gl::GetString(gl::VERSION) != NULL ? (const char*)gl::GetString(gl::VERSION) : "NULL") << "; GLSL v" << gl::GetString(gl::SHADING_LANGUAGE_VERSION) << "]" << endl;
  }

  cerr << "SYSTEM ... OK" << endl
       << "RUNNING" << endl;

  // Setup shader program
  Program basicProgram;
  basicProgram.compileShader("src/shaders/simplemvp.glsl.vert");
  basicProgram.compileShader("src/shaders/weirdcolors.glsl.frag");

  basicProgram.link();

  if(!basicProgram.isLinked()) {
    ERRLOG("Could not link shader program.");

    glfwDestroyWindow(hWindow);
    glfwTerminate();
    return -1;
  } else
    basicProgram.use();

  // Setup scene matrices
  mat4 projection = perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
  mat4 view = translate(mat4(1.0f), vec3(0.0f, 0.0f, -8.0f));
  mat4 model = mat4(1.0f);
  mat4 mvp = projection*view*model;

  // Setup cube poitns
  static const vec3 vertex_data[] = {
    //*
    vec3( 0.5f,  0.5f,  0.5f),
    vec3( 0.5f,  0.5f, -0.5f),
    vec3(-0.5f,  0.5f, -0.5f),
    vec3(-0.5f,  0.5f,  0.5f),

    vec3( 0.5f, -0.5f,  0.5f),
    vec3( 0.5f, -0.5f, -0.5f),
    vec3(-0.5f, -0.5f, -0.5f),
    vec3(-0.5f, -0.5f,  0.5f)
    //*/
  };

  static const vec3 color_data[] = {
    vec3(1.0f, 0.0f, 0.0f), // RED
    vec3(0.0f, 1.0f, 0.0f), // GREEN
    vec3(1.0f, 1.0f, 0.0f), // YELLOW
    vec3(0.0f, 0.0f, 1.0f), // BLUE
    vec3(1.0f, 1.0f, 1.0f), // WHITE
    vec3(1.0f, 0.5f, 0.0f)  // ORANGE
  };

  static const float cube_data[] = {
    //*
    // Top
    vertex_data[2][0],
    vertex_data[2][1],
    vertex_data[2][2],
    vertex_data[1][0],
    vertex_data[1][1],
    vertex_data[1][2],
    vertex_data[0][0],
    vertex_data[0][1],
    vertex_data[0][2],

    vertex_data[2][0],
    vertex_data[2][1],
    vertex_data[2][2],
    vertex_data[0][0],
    vertex_data[0][1],
    vertex_data[0][2],
    vertex_data[3][0],
    vertex_data[3][1],
    vertex_data[3][2],

    // Bottom
    vertex_data[4][0],
    vertex_data[4][1],
    vertex_data[4][2],
    vertex_data[5][0],
    vertex_data[5][1],
    vertex_data[5][2],
    vertex_data[6][0],
    vertex_data[6][1],
    vertex_data[6][2],

    vertex_data[4][0],
    vertex_data[4][1],
    vertex_data[4][2],
    vertex_data[6][0],
    vertex_data[6][1],
    vertex_data[6][2],
    vertex_data[7][0],
    vertex_data[7][1],
    vertex_data[7][2],

    // Left
    vertex_data[0][0],
    vertex_data[0][1],
    vertex_data[0][2],
    vertex_data[1][0],
    vertex_data[1][1],
    vertex_data[1][2],
    vertex_data[5][0],
    vertex_data[5][1],
    vertex_data[5][2],

    vertex_data[0][0],
    vertex_data[0][1],
    vertex_data[0][2],
    vertex_data[5][0],
    vertex_data[5][1],
    vertex_data[5][2],
    vertex_data[4][0],
    vertex_data[4][1],
    vertex_data[4][2],

    // Right
    vertex_data[3][0],
    vertex_data[3][1],
    vertex_data[3][2],
    vertex_data[6][0],
    vertex_data[6][1],
    vertex_data[6][2],
    vertex_data[7][0],
    vertex_data[7][1],
    vertex_data[7][2],

    vertex_data[3][0],
    vertex_data[3][1],
    vertex_data[3][2],
    vertex_data[2][0],
    vertex_data[2][1],
    vertex_data[2][2],
    vertex_data[6][0],
    vertex_data[6][1],
    vertex_data[6][2],

    // Front
    vertex_data[3][0],
    vertex_data[3][1],
    vertex_data[3][2],
    vertex_data[0][0],
    vertex_data[0][1],
    vertex_data[0][2],
    vertex_data[4][0],
    vertex_data[4][1],
    vertex_data[4][2],

    vertex_data[3][0],
    vertex_data[3][1],
    vertex_data[3][2],
    vertex_data[4][0],
    vertex_data[4][1],
    vertex_data[4][2],
    vertex_data[7][0],
    vertex_data[7][1],
    vertex_data[7][2],

    // Back
    vertex_data[1][0],
    vertex_data[1][1],
    vertex_data[1][2],
    vertex_data[2][0],
    vertex_data[2][1],
    vertex_data[2][2],
    vertex_data[5][0],
    vertex_data[5][1],
    vertex_data[5][2],

    vertex_data[2][0],
    vertex_data[2][1],
    vertex_data[2][2],
    vertex_data[6][0],
    vertex_data[6][1],
    vertex_data[6][2],
    vertex_data[5][0],
    vertex_data[5][1],
    vertex_data[5][2]
    //*/
  };

  static const float cube_color[] = {
    // Top
    color_data[0][0],
    color_data[0][1],
    color_data[0][2],
    color_data[0][0],
    color_data[0][1],
    color_data[0][2],
    color_data[0][0],
    color_data[0][1],
    color_data[0][2],

    color_data[0][0],
    color_data[0][1],
    color_data[0][2],
    color_data[0][0],
    color_data[0][1],
    color_data[0][2],
    color_data[0][0],
    color_data[0][1],
    color_data[0][2],

    // Bottom
    color_data[5][0],
    color_data[5][1],
    color_data[5][2],
    color_data[5][0],
    color_data[5][1],
    color_data[5][2],
    color_data[5][0],
    color_data[5][1],
    color_data[5][2],

    color_data[5][0],
    color_data[5][1],
    color_data[5][2],
    color_data[5][0],
    color_data[5][1],
    color_data[5][2],
    color_data[5][0],
    color_data[5][1],
    color_data[5][2],

    // Left
    color_data[4][0],
    color_data[4][1],
    color_data[4][2],
    color_data[4][0],
    color_data[4][1],
    color_data[4][2],
    color_data[4][0],
    color_data[4][1],
    color_data[4][2],

    color_data[4][0],
    color_data[4][1],
    color_data[4][2],
    color_data[4][0],
    color_data[4][1],
    color_data[4][2],
    color_data[4][0],
    color_data[4][1],
    color_data[4][2],

    // Right
    color_data[2][0],
    color_data[2][1],
    color_data[2][2],
    color_data[2][0],
    color_data[2][1],
    color_data[2][2],
    color_data[2][0],
    color_data[2][1],
    color_data[2][2],

    color_data[2][0],
    color_data[2][1],
    color_data[2][2],
    color_data[2][0],
    color_data[2][1],
    color_data[2][2],
    color_data[2][0],
    color_data[2][1],
    color_data[2][2],

    // Front
    color_data[1][0],
    color_data[1][1],
    color_data[1][2],
    color_data[1][0],
    color_data[1][1],
    color_data[1][2],
    color_data[1][0],
    color_data[1][1],
    color_data[1][2],

    color_data[1][0],
    color_data[1][1],
    color_data[1][2],
    color_data[1][0],
    color_data[1][1],
    color_data[1][2],
    color_data[1][0],
    color_data[1][1],
    color_data[1][2],

    // Back
    color_data[3][0],
    color_data[3][1],
    color_data[3][2],
    color_data[3][0],
    color_data[3][1],
    color_data[3][2],
    color_data[3][0],
    color_data[3][1],
    color_data[3][2],

    color_data[3][0],
    color_data[3][1],
    color_data[3][2],
    color_data[3][0],
    color_data[3][1],
    color_data[3][2],
    color_data[3][0],
    color_data[3][1],
    color_data[3][2]
  };

  // State setup
  gl::ClearColor(0.95f, 0.95f, 0.95f, 1.0f);
  gl::CullFace(gl::FRONT_AND_BACK);
  gl::Enable(gl::DEPTH_TEST);

  // Setup buffers for cube.
  GLuint buffers[2];
  gl::GenBuffers(2, buffers);

  // Populate position buffer
  gl::BindBuffer(gl::ARRAY_BUFFER, buffers[0]);
  //gl::BufferData(gl::ARRAY_BUFFER, 6*2*3*3*sizeof(float), cube_data, gl::STATIC_DRAW);
  gl::BufferData(gl::ARRAY_BUFFER, sizeof(cube_data), cube_data, gl::STATIC_DRAW);

  // Bind the data buffer to the VAO
  GLuint vao;
  gl::GenVertexArrays(1, &vao);
  gl::BindVertexArray(vao);

  // Enable VAO for position
  gl::EnableVertexAttribArray(0);
  gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, NULL);

  // Populate color buffer
  gl::BindBuffer(gl::ARRAY_BUFFER, buffers[1]);
  //gl::BufferData(gl::ARRAY_BUFFER, 6*2*3*3*sizeof(float), cube_color, gl::STATIC_DRAW);
  gl::BufferData(gl::ARRAY_BUFFER, sizeof(cube_color), cube_color, gl::STATIC_DRAW);

  // Enable VAO for color
  gl::EnableVertexAttribArray(1);
  gl::VertexAttribPoint(1, 3, gl::FLOAT, gl::FALSE_, 0, NULL);

  // Set uniform, as it won't need be changed.
  basicProgram.setUniform("projection", projection);
  basicProgram.setUniform("view", view);

  // Setup rotation modifiers
  float orbit_radius = 8.0f;
  float rotate_factor = 0.005f;
  vec2 rotate_angles = vec2(3.145159f/2, 0.0f);

  // Setup mouse positions.
  glm::vec2 originalMousePosition;
  mouse_state currentMouseState = MOUSE_RELEASED;

  // Enter main loop of application.
  while(!glfwWindowShouldClose(hWindow)) {
    // Clear window
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

    // Check mouse input
    if(glfwGetMouseButton(hWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
      double x, y;
      glm::vec2 mouseDelta;

      // Set current state for pressing.
      if(currentMouseState != MOUSE_RIGHT_DRAG) {
        currentMouseState = MOUSE_RIGHT_DRAG;
        glfwSetInputMode(hWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        x = window_width/2;
        y = window_height/2;
      } else
        glfwGetCursorPos(hWindow, &x, &y);

      glfwSetCursorPos(hWindow, window_width/2, window_height/2);

      mouseDelta = glm::vec2(x - window_width/2.0, y - window_height/2.0);

      // Rotate view inversly.
      rotate_angles.x += rotate_factor*mouseDelta.x;
      rotate_angles.y += rotate_factor*mouseDelta.y;

      if(rotate_angles.y < -1)
        rotate_angles.y = -1;
      else if(rotate_angles.y > 1)
        rotate_angles.y = 1;

      // Calculate new camera position.
      vec3 cameraPosition = vec3(orbit_radius*cos(rotate_angles.x),
                                 orbit_radius*sin(rotate_angles.y),
                                 orbit_radius*sin(rotate_angles.x));

      // Set look-at vector.
      view = glm::lookAt(cameraPosition, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

      // Set uniform.
      basicProgram.setUniform("view", view);
    } else {
      double x, y;

      // Set state to released.
      if(currentMouseState != MOUSE_RELEASED) {
        currentMouseState = MOUSE_RELEASED;
        glfwSetInputMode(hWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPos(hWindow, originalMousePosition.x, originalMousePosition.y);
      }

      glfwGetCursorPos(hWindow, &x, &y);

      originalMousePosition = glm::vec2((float)(x), (float)(y));
    }

    // Draw Rubick's Cube :DDDDD
    for(int x = -1; x < 2; ++x) {
      for(int y = -1; y < 2; ++y) {
        for(int z = -1; z < 2; ++z) {
          if(x == 0 && y == 0 && z == 0)
            continue;

          mat4 model;
          float scale = 1.0f;
          float spacing = 0.1f;
          float offset = scale + spacing;

          // Calculate model matrix!
          mat4 cubeModel = glm::scale(translate(mat4(1.0f), vec3(x*offset, y*offset, z*offset)), vec3(scale));
          basicProgram.setUniform("model", cubeModel);

          // Render the current cube.
          gl::DrawArrays(gl::TRIANGLES, 0, 6*2*3);
        }
      }
    }

    gl::Flush();

    // Window housekeeping...
    glfwSwapBuffers(hWindow);
    glfwPollEvents();

    // SPAAAAAAAAACESHIP!
    [=](){;;;;};
  }

  // Cleanup application and exit.
  glfwTerminate();
  return 0;
}
