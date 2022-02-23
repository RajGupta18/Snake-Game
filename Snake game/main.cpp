#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

#include "game.h"
#include "resource_manager.h"
#include "random.h"

const unsigned int SCREEN_WIDTH = 600;
const unsigned int SCREEN_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Game* SnakeGame  = new Game(20);
int movementKey = 0;

int main() {

    //Initialise GLFW
    if(!glfwInit()) {
        std::cout<<"ERROR: Failed to initialise glfw!"<<std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake", nullptr, nullptr);
    if(!window) {
        std::cout<<"ERROR: Failed to create window!"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    float delta_time = 0.0f;
    float previous_time = 0.0f;

    //Initialize game
    Random::setSeed((unsigned int)(glfwGetTime()*1e6));
    SnakeGame->init_game();

    while(!glfwWindowShouldClose(window)) {

        //Update deltatime
        float current_time = glfwGetTime();
        delta_time = current_time - previous_time;
        previous_time = current_time;
        glfwPollEvents();

        //process game inputs
        SnakeGame->ProcessInput(movementKey);

        //update game state
        SnakeGame->update_game(delta_time);

        //render game
        glClearColor(0.90196f, 0.87843f, 0.43921f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SnakeGame->render_game();
        
        glfwSwapBuffers(window);
    }

    ResourceManager::Clear();
    delete SnakeGame;
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if(movementKey == 0 || movementKey == 1) {
        if (key == GLFW_KEY_DOWN) movementKey = 2;
        else if (key == GLFW_KEY_UP) movementKey = 3;
    }
    else {
        if(key == GLFW_KEY_RIGHT) movementKey = 0;
        else if (key == GLFW_KEY_LEFT) movementKey = 1;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}