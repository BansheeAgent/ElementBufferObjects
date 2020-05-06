#include <iostream>
#include <glad/glad.h>

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <vector>
#include <algorithm>

void processInput(GLFWwindow window[0]);

static void ErrorCallback(int error, const char* description)
{
  std::cout << "Error: " << error << ", " << description << std::endl;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action
        , int mods)
{
  scancode;
  mods;
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main()
{
  std::vector<GLFWwindow*> window;

  if(!glfwInit())
  {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  int quantity = 2;

  for(int count = 0; count < 1; count++)
  {
    window.push_back(glfwCreateWindow(640, 480,
                                      std::string("glfw window" + count).c_str() ,
                                      nullptr,
                                      nullptr));
    glfwSetErrorCallback(ErrorCallback);

    glfwMakeContextCurrent(window[count]);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));


    if(!window[count])
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window[count], KeyCallback);

    glfwSwapInterval(1);

  }

///////////////////////////////////////////////////////////////////////////////
/*
 *Element Buffer Objects
 *  There is one last thing we'd like to discuss when rendering vertices and
 *  that is element buffer objects abbreviated to EBO.
 *
 *  Suppose we want to draw a rectangle instead of a triangle. We can draw a
 *  rectangle using two triangles. we can specify the unique vertices, and
 *  then specify at which order we'd like to draw them.
 *
 *  Element buffer objects work exactly this. an EBO is a buffer, just lika a
 *  vertex buffer object, that stores indices that OpenGL uses to decide what
 *  vertices to draw. This so called indexed drawing is exactly the solution
 *  to our problem.
 */
///////////////////////////////////////////////////////////////////////////////
  float vertices[] = {
          0.5f,  0.5f, 0.0f,  // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f,  // bottom left
          -0.5f,  0.5f, 0.0f   // top left
  };
  unsigned int indices[] = {
          0, 1, 3,
          1, 2, 3
  };






  const char* vertexShaderSource = (
          "#version 330 core\n"
          "layout (location = 0) in vec3 aPos;"
          "void main()"
          "{"
          "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
          "}");


  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);


  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
              std::endl;
  }

  const char* fragSource =
          ("#version 330 core\n"
           "out vec4 FragColor;"
           ""
           "void main()"
           "{"
           "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
           "}");


  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragSource, nullptr);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }


  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();


  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);


  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog <<
              std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glUseProgram(shaderProgram);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);


  glBindVertexArray(VAO);
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  ///////////////////////////////////////////////////////////////////////////////
  /*
   * similar to the VBO we bind the EBO and copy the indices into the buffer
   * with glBufferData. Also, like VBO, we want to place those calls between
   * a bind and an unbind call, although this time we specify
   * GL_ELEMENT_ARRAY_BUFFER  as the buffer type
   *
   * move to draw arrays function call
   */
  ///////////////////////////////////////////////////////////////////////////////

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);



  double time = glfwGetTime();
  static bool allWindowsClosed = true;
  //render loop here
  do
  {
    allWindowsClosed = true;

    std::for_each(window.begin(), window.end(), [](GLFWwindow* theWindow)
    {
      if(theWindow == nullptr)
        return;

      if(glfwWindowShouldClose(theWindow))
      {
        glfwSetWindowShouldClose(theWindow, GLFW_TRUE);
      }
      else
      {

        allWindowsClosed = false;
      }
    });


    // input
    // -----


    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    ////////////////////////////////////////////////////////////////////////////
    /*
     * Optional Wireframe mode
     *  To draw your triangles in wireframe mode, you can configure how
     *  OpenGL draws its primitives via glPolygonMode(GL_FRONT_AND_BACK,
     *  GL_LINE).
     *  the first argument says we want to apply it to the front and back of
     *  all triangles and the second line tells us to draw them as lines. Any
     *  subsequent drawing calls will render the triangles in wirerfame mode
     *  until we set ti back to its default using glPolygonMode
     *  (GL_FRONT_AND_BACK, GL_FILL).
     */
    ////////////////////////////////////////////////////////////////////////////
    static int pattern = 0;
    if(pattern)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      pattern = 0;
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      pattern = 1;
    }


    ////////////////////////////////////////////////////////////////////////////
    /*
     * we're now giving GL_ELEMENT_ARRAY_BUFFER as the buffer target. The
     * Last thing left to do is replace the glDrawArrays call with
     * glDrawElements to indicate we want to render the triangles from an
     * index  buffer. When using glDrawElements we're going to draw using
     * indices provided in the element buffer object currently bound:
     *
     * The first argument specifies the mode we want to draw in.
     * The second argument is the count or the number or number of elements
     * to draw
     * The third argument is the type of the indices which is of type
     * GL_UNSIGNED_INT.
     * the last argument allows us to specify an offset in the EBO(or pass in
     * an index array, that is when you're not using element buffer objects).
     *
     * The glDrawElements function takes its indices from the EBO currently
     * bound to the GL_ELEMENT_ARRAY_BUFFER target. This means we have to
     * bind the corresponding EBO each time we want to render an object with
     * indices. VAO keep track of element buffer object bindings. The last
     * element buffer object that gets bound while a VAO is bound, is stored
     * as the VAO's element buffer object. Binding to a VAO the also
     * automatically binds that EBO.
     */
    ////////////////////////////////////////////////////////////////////////////

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window[0]);
    glfwPollEvents();


  }while(!allWindowsClosed);


  std::for_each(window.begin(), window.end(), glfwDestroyWindow);

  glfwTerminate();
  exit(EXIT_SUCCESS);


}