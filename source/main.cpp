/**
 * main.cpp
 *
 * @author:  Brian Clee
 * @version: 12/14/2015
 */

#include "inc/glew/glew.h"
#include "inc/glfw/glfw3.h"

#include "game.h"
#include "resource.h"

#pragma warning(disable: 4996)

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

game breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Mario Breakout", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();
    glGetError(); 

    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    breakout.init();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        breakout.input(deltaTime);
        breakout.update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        breakout.render();

        glfwSwapBuffers(window);
    }

    resource::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
        glfwSetWindowShouldClose(window, GL_TRUE);
	}
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
		{
            breakout.Keys[key] = GL_TRUE;
		}
        else if (action == GLFW_RELEASE)
		{
            breakout.Keys[key] = GL_FALSE;
			breakout.KeysProcessed[key] = GL_FALSE;
		}
    }
}