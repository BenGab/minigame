#include<stdio.h>
#include<string.h>
#include<gl/glew.h>
#include<GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

static const char *vshader = "                                      \n\
#version  330                                                       \n\
                                                                    \n\
layout(location = 0) in vec3 pos;                                   \n\
                                                                    \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);                   \n\
}                                                                   \n\
";

static const char *fshader = "                          \n\
#version  330                                           \n\
                                                        \n\
out vec4 colour;                                        \n\
                                                        \n\
                                                        \n\
void main()                                             \n\
{                                                       \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);                  \n\
}                                                       \n\
";

void createTriangle() 
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
}

void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType) 
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    GLint codeLenght[1];
    codeLenght[0] = strlen(shaderCode);
    glShaderSource(theShader, 1, theCode, codeLenght);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar elog[1024] = { 0 }; 

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if(!result) 
    {
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        printf("Error compiling the: %d shader: '%s' \n", shaderType, elog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void compileShaders() 
{
    shader = glCreateProgram();

    if(!shader) 
    {
        printf("Failed to create shader");
        return;
    }

    addShader(shader, vshader, GL_VERTEX_SHADER);
    addShader(shader, fshader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar elog[1024] = { 0 }; 

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if(!result) 
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error linking program: %s\n", elog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if(!result) 
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error validating program: %s\n", elog);
        return;
    }
}

int main() 
{
    if(!glfwInit()) 
    {
        printf("GLFW initialiyation failed");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWIndow = glfwCreateWindow(WIDTH, HEIGHT, "Minigame window", NULL, NULL);

    if(!mainWIndow) 
    {
        printf("OpenGL window init failed");
        glfwTerminate();
        return 1;
    }

    int bufferwidht, bufferheight;

    glfwGetFramebufferSize(mainWIndow, &bufferwidht, &bufferheight);
    glfwMakeContextCurrent(mainWIndow);

    glewExperimental = GL_TRUE; 

    if(glewInit() != GLEW_OK) {
        printf("glew init failed");
        glfwDestroyWindow(mainWIndow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferwidht, bufferheight);
    createTriangle();
    compileShaders();

    while(!glfwWindowShouldClose(mainWIndow)) 
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

		glUseProgram(0);

        glfwSwapBuffers(mainWIndow);
    }

    return 0;
}