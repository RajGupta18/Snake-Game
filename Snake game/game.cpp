#include "game.h"
#include <iostream>
#include "glm/glm.hpp"
#include "resource_manager.h"

Game::Game() {
    step = 20;
    grid = std::vector<std::vector<board>>(step, std::vector<board>(step));
    grid_size = 2.0f / (float)step;
    directionIndex = 0;

    pos[0] = 10;
    pos[1] = 10;

    time = 0.0f;
}

Game::Game(const int &s) {
    if(s%2 != 0) step = s + 1;
    else step = s;

    grid = std::vector<std::vector<board>>(step, std::vector<board>(step));
    grid_size = 2.0f / (float)step;
    directionIndex = 0;

    pos[0] = 10;
    pos[1] = 10;

    time = 0.0f;
}

void Game::init_game() {
    //array is filled from (-1,-1) position
    //that is bottom left corner
    for(int i = (int)(step-1); i>=0; i--) {
        for(int j = 0; j < step; j++) {
            grid[i][j].vis = false;
            grid[i][j].x = -1.0f + j*grid_size;
            grid[i][j].y = -1.0f + (step-i)*grid_size;
        }
    }

    //load snake
    ResourceManager::LoadShader("snake", "shaders/snake/snake.vs", "shaders/snake/snake.gs", "shaders/snake/snake.fs");
    ResourceManager::resources["snake"]->UseShader();
    ResourceManager::resources["snake"]->set_float("gsize", grid_size);
    ResourceManager::resources["snake"]->set_vec3("color", glm::vec3(0.36862f, 0.23529f, 0.6f));
    s.pos[0] = grid[pos[0]][pos[1]].x;
    s.pos[1] = grid[pos[0]][pos[1]].y;

    glGenVertexArrays(1, &s.vao);
    glGenBuffers(1, &s.vbo);
    glBindVertexArray(s.vao);
    glBindBuffer(GL_ARRAY_BUFFER, s.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(s.pos), s.pos, GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::ProcessInput(int key) {
    directionIndex = key;
    //std::cout<<"Key : "<<directionIndex<<std::endl; 
}

void Game::update_game(float &deltatime) {
    time += deltatime;

    if(time > 0.1f) {
        //std::cout<<pos[0]<<" "<<pos[1]<<std::endl;
        pos[0] = (step + pos[0] + direction[directionIndex][0])%step;
        pos[1] = (step + pos[1] + direction[directionIndex][1])%step;
        s.pos[0] = grid[pos[0]][pos[1]].x;
        s.pos[1] = grid[pos[0]][pos[1]].y;
        //std::cout<<grid[pos[0]][pos[1]].x<<" "<<grid[pos[0]][pos[1]].y<<std::endl;

        glBindVertexArray(s.vao);
        glBindBuffer(GL_ARRAY_BUFFER,s.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(s.pos), s.pos);    
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        time = 0.0f;
    }
}

void Game::render_game() {
    ResourceManager::resources["snake"]->UseShader();
    glBindVertexArray(s.vao);
    glDrawArrays(GL_POINTS, 0, 1);
}

Game::~Game() {
    grid.clear();
}