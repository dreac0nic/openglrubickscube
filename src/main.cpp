#include <iostream>
#include <sstream>

#include "gl_core_4_4.hpp"
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glslu.hpp"

#define ERRLOG(errstr) std::cerr << "ERR [" << __FILE__ << ":" << __LINE__ << "] " << errstr << std::endl;

void glfw_err_callback(int code, const char* message);

using namespace std;
using glm::vec3;
using glm::mat4;
using glm::perspective;
using glm::translate;
using glm::scale;
using glm::rotate;
using glslu::Program;

int main(int argc, char* argv[])
{
  GLFWwindow* hWindow;

  // Set error callback, because GLFW is being persnickety.
  glfwSetErrorCallback([](int code, const char* message) -> void {
    cerr << "GLFW ERR[" << code << "]: " << message; });

  // Load application framworks00...
  cerr << "INITIALIZING SYSTEMS" << endl
       << "--------------------" << endl;

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
  hWindow = glfwCreateWindow(640, 480, "WINDOW", NULL, NULL);
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
  basicProgram.compileShader("src/shaders/red.glsl.frag");

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
  mat4 view = rotate(translate(mat4(1.0f), vec3(0.0f, 0.0f, -8.0f)), 45.0f, vec3(1.0f, 1.0f, 0.0f));
  mat4 model = mat4(1.0f);
  mat4 mvp = projection*view*model;

  // Setup cube poitns
  static const vec3 cube_data[] = {
    vec3(-0.5f, -0.5f, -0.5f),
    vec3(-0.5f, -0.5f,  0.5f),
    vec3(-0.5f,  0.5f,  0.5f),

    vec3( 0.5f,  0.5f, -0.5f),
    vec3(-0.5f, -0.5f, -0.5f),
    vec3(-0.5f,  0.5f, -0.5f),

    vec3( 0.5f, -0.5f,  0.5f),
    vec3(-0.5f, -0.5f, -0.5f),
    vec3( 0.5f, -0.5f, -0.5f),

    vec3( 0.5f,  0.5f, -0.5f),
    vec3( 0.5f, -0.5f, -0.5f),
    vec3(-0.5f, -0.5f, -0.5f),

    vec3(-0.5f, -0.5f, -0.5f),
    vec3(-0.5f,  0.5f,  0.5f),
    vec3(-0.5f,  0.5f, -0.5f),

    vec3( 0.5f, -0.5f,  0.5f),
    vec3(-0.5f, -0.5f,  0.5f),
    vec3(-0.5f, -0.5f, -0.5f),

    vec3(-0.5f,  0.5f,  0.5f),
    vec3(-0.5f, -0.5f,  0.5f),
    vec3( 0.5f, -0.5f,  0.5f),

    vec3( 0.5f,  0.5f,  0.5f),
    vec3( 0.5f, -0.5f, -0.5f),
    vec3( 0.5f,  0.5f, -0.5f),

    vec3( 0.5f, -0.5f, -0.5f),
    vec3( 0.5f,  0.5f,  0.5f),
    vec3( 0.5f, -0.5f,  0.5f),

    vec3( 0.5f,  0.5f,  0.5f),
    vec3( 0.5f,  0.5f, -0.5f),
    vec3(-0.5f,  0.5f, -0.5f),

    vec3( 0.5f,  0.5f,  0.5f),
    vec3(-0.5f,  0.5f, -0.5f),
    vec3(-0.5f,  0.5f,  0.5f),

    vec3( 0.5f,  0.5f,  0.5f),
    vec3(-0.5f,  0.5f,  0.5f),
    vec3( 0.5f, -0.5f,  0.5f),
  };

  // Setup buffer for cube.
  GLuint buffer;
  gl::GenBuffers(1, &buffer);

  // Populate buffer
  gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
  gl::BufferData(gl::ARRAY_BUFFER, 3*6*2*3*sizeof(float), cube_data, gl::STATIC_DRAW);

  // Bind the data buffer to the VAO
  GLuint vao;
  gl::GenVertexArrays(1, &vao);
  gl::BindVertexArray(vao);

  // Enable VAO for position
  gl::EnableVertexAttribArray(0);
  gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, NULL);

  gl::ClearColor(0.95f, 0.95f, 0.95f, 1.0f);

  // Set uniform, as it won't need be changed.
  basicProgram.setUniform("projection", projection);
  basicProgram.setUniform("view", view);

  // Enter main loop of application.
  while(!glfwWindowShouldClose(hWindow)) {
    // Clear window
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

    for(int x = -1; x < 2; ++x) {
      for(int y = -1; y < 2; ++y) {
        for(int z = -1; z < 2; ++z) {
          mat4 model;
          float scale = 1.0f;
          float spacing = 0.8f;
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
