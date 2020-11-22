#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

float vertices[] =
{
   0.5f,  0.5f, 0.0f, // Upper-right corner
   0.5f, -0.5f, 0.0f, // Bottom-right corner
  -0.5f, -0.5f, 0.0f, // Bottom-left corner
  -0.5f,  0.5f, 0.0f  // Upper-left corner
};

unsigned int indices[] =
{
  0, 1, 2, // First triangle
  0, 2, 3  // Second triangle
};

const char* vertexShaderSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char* fragmentShaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

int create_shader_program(unsigned int* shaderProgram)
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
  if (!create_shader_program(&shaderProgram))
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

  // Generate the element buffer
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  // Copy the indices array into a buffer that OpenGL can use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Setup the vertex attribute pointers.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // This line turns on wireframe drawing (useful for seeing the triangles).
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window))
  {
    // Input
    process_input(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    // Event handling and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
