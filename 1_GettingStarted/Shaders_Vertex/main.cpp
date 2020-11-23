#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>

float vertices[] =
{
  // Positions        Colours
   0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

const char* vertexShaderSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"   // Attribute 0 is position
  "layout (location = 1) in vec3 aColor;\n" // Attribute 1 is color
  "out vec3 ourColor;\n" // Output a color to the fragment shader
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos, 1.0);\n"
  "  ourColor = aColor;\n" // Pass through the color from the vertex data
  "}\0";

const char* fragmentShaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n" // Uniform variables can be accessed using OpenGL.
  "void main()\n"
  "{\n"
  "  FragColor = vec4(ourColor, 1.0);\n"
  "}\0";

int create_shader_program(
  unsigned int* shaderProgram,
  const char* vertexShaderSource,
  const char* fragmentShaderSource)
{
  // Compile the vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Confirm that the shader was compiled successfully
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
    return success;
  }

  // Compile the fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Confirm that the shader was compiled successfully
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
    return success;
  }

  // Create a shader program that combines the vertex and fragment shaders
  *shaderProgram = glCreateProgram();
  glAttachShader(*shaderProgram, vertexShader);
  glAttachShader(*shaderProgram, fragmentShader);
  glLinkProgram(*shaderProgram);

  // Confirm that the program linked the two shaders correctly
  glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
    std::cout << "Shader program failed to link: " << infoLog << std::endl;
    return success;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return success;
}

void framesize_buffer_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialise GLAD" << std::endl;
    return -2;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framesize_buffer_callback);

  // Create the shader program.
  unsigned int shaderProgram;
  if (!create_shader_program(
    &shaderProgram,
    vertexShaderSource,
    fragmentShaderSource))
  {
    return -3;
  }

  // Create and bind a vertex array object.
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generate the vertex buffer
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // Copy the vertex array into a buffer that OpenGL can use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Setup the vertex attribute pointers.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
    (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // This line turns on wireframe drawing (useful for seeing the triangles).
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window))
  {
    // Input
    process_input(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
    float redValue = (std::cos(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Event handling and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
