#include "game.h"
#include <iostream>
#include "glm/glm.hpp"
#include "resource_manager.h"
#include "random.h"

Game::Game() {
    time = 0.0f;
    step = 20;
}

Game::Game(const int &s) {
    time = 0.0f;
    if(s%2 != 0) step = s + 1;
    else step = s;    
}

void Game::init_game() {
    //init snake board
    grid = std::vector<std::vector<board>>(step, std::vector<board>(step));
    grid_size = 2.0f / (float)step;
    directionIndex = 0;

    //array is filled from (-1,-1) position i.e bottom left corner
    for(int i = (int)(step-1); i>=0; i--) {
        for(int j = 0; j < step; j++) {
            grid[i][j].vis = false;
            grid[i][j].x = -1.0f + j*grid_size;
            grid[i][j].y = -1.0f + (step-i)*grid_size;
        }
    }

    //init nodes and indexMap vector
    unvis_size = step*step;
    mp = std::vector<unsigned int>(unvis_size);
    imp = std::vector<unsigned int>(unvis_size);
    for(int i=0; i<unvis_size; i++) {
        mp[i] = i;
        imp[i] = i;
    }

    //init snake
    snake.vao = 0;
    snake.vbo = 0;
    snake.positions = std::vector<float>(2*step*step);
    snake.length = 2*4;
    pos[0] = step/2;
    pos[1] = step/2;
    snake_head = nullptr;
    snake_tail = nullptr;

    //load snake
    ResourceManager::LoadShader("snake", "shaders/snake/snake.vs", "shaders/snake/snake.gs", "shaders/snake/snake.fs");
    ResourceManager::resources["snake"]->UseShader();
    ResourceManager::resources["snake"]->set_float("gsize", grid_size);
    ResourceManager::resources["snake"]->set_vec3("color", glm::vec3(0.36862f, 0.23529f, 0.6f));

    for(int i =0, j = 0; i<snake.length; ) {
        int curboardIndex = pos[0]*step + (pos[1]-j);
        if(!snake_head) {
            snake_head = new SnakeNode(i, curboardIndex);
            snake_tail = snake_head;
        }
        else {
            SnakeNode *temp = new SnakeNode(i, curboardIndex);
            temp->next = snake_tail;
            snake_tail = temp;
        }
        snake.positions[i] = grid[pos[0]][pos[1] - j].x;
        snake.positions[i+1] = grid[pos[0]][pos[1] - j].y;
        j++;
        i += 2;

        //mark visisted board indexes
        unvis_size--;
        mp[curboardIndex] = unvis_size;
        mp[unvis_size] = curboardIndex;
        imp[unvis_size] = curboardIndex;
        imp[curboardIndex] = unvis_size;
    }

    glGenVertexArrays(1, &snake.vao);
    glGenBuffers(1, &snake.vbo);
    glBindVertexArray(snake.vao);
    glBindBuffer(GL_ARRAY_BUFFER, snake.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*snake.length, &snake.positions[0], GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //load food
    ResourceManager::LoadShader("food", "shaders/snake/snake.vs", "shaders/snake/snake.gs", "shaders/snake/snake.fs");
    ResourceManager::resources["food"]->UseShader();
    ResourceManager::resources["food"]->set_float("gsize", grid_size);
    ResourceManager::resources["food"]->set_vec3("color", glm::vec3(0.90625f, 0.24609f, 0.37891f));

    unsigned int rn = Random::rand(unvis_size);
    food.position[0] = grid[mp[rn]/step][mp[rn]%step].x;
    food.position[1] = grid[mp[rn]/step][mp[rn]%step].y;

    unvis_size--;
    food.boardIndex = mp[rn];
    unsigned int lastindex = mp[unvis_size];
    mp[rn] = lastindex;
    mp[unvis_size] = food.boardIndex;
    imp[food.boardIndex] = unvis_size;
    imp[lastindex] = rn;

    glGenVertexArrays(1, &food.vao);
    glGenBuffers(1, &food.vbo);
    glBindVertexArray(food.vao);
    glBindBuffer(GL_ARRAY_BUFFER, food.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2, &food.position[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::ProcessInput(int key) {
    directionIndex = key;
}

void Game::update_game(float &deltatime) {
    time += deltatime;

    if(time > 0.1f) {
        pos[0] = (step + pos[0] + direction[directionIndex][0])%step;
        pos[1] = (step + pos[1] + direction[directionIndex][1])%step;
        unsigned int newboardIndex = pos[0]*step + pos[1];

        SnakeNode* cur;
        //if snake head is at food
        if(newboardIndex == food.boardIndex) {
            //std::cout<<"food eat"<<std::endl;
            cur = new SnakeNode(snake.length, newboardIndex);
            snake.positions[snake.length] = grid[pos[0]][pos[1]].x;
            snake.positions[snake.length+1] = grid[pos[0]][pos[1]].y;
            snake.length += 2;
            snake_head->next = cur;
            snake_head = cur;

            //update map and add new food
            unsigned int rn = Random::rand(unvis_size);
            unvis_size--;

            food.boardIndex = mp[rn];
            food.position[0] = grid[mp[rn]/step][mp[rn]%step].x;
            food.position[1] = grid[mp[rn]/step][mp[rn]%step].y;
            glBindVertexArray(food.vao);
            glBindBuffer(GL_ARRAY_BUFFER,food.vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*2, &food.position[0]);    
            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindVertexArray(0);

            unsigned int tmp = mp[unvis_size];
            mp[unvis_size] = food.boardIndex;
            mp[rn] = tmp;
            imp[tmp] = rn;
            imp[food.boardIndex] = unvis_size;
        }
        else {
            //std::cout<<"run"<<std::endl;
            cur = snake_tail;
            snake_tail = cur->next;
            cur->next = nullptr;
            snake_head->next = cur;
            snake_head = cur;

            snake.positions[cur->posIndex] = grid[pos[0]][pos[1]].x;
            snake.positions[cur->posIndex+1] = grid[pos[0]][pos[1]].y;

            //update map
            unsigned int a = imp[cur->boardIndex];
            unsigned int b = imp[newboardIndex];
            mp[b] = cur->boardIndex;
            mp[a] = newboardIndex;
            imp[newboardIndex] = a;
            imp[cur->boardIndex] = b;
            cur->boardIndex = newboardIndex;        
        }

        // glBindVertexArray(snake.vao);
        // glBindBuffer(GL_ARRAY_BUFFER,snake.vbo);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*snake.length, &snake.positions[0]);    
        // glBindBuffer(GL_ARRAY_BUFFER,0);
        // glBindVertexArray(0);

        glBindVertexArray(snake.vao);
        glBindBuffer(GL_ARRAY_BUFFER, snake.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*snake.length, &snake.positions[0], GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        /*if(newboardIndex == food.boardIndex) {
            unsigned int rn = Random::rand(unvis_size);
            food.position[0] = grid[mp[rn]/step][mp[rn]%step].x;
            food.position[1] = grid[mp[rn]/step][mp[rn]%step].y;

            glBindVertexArray(food.vao);
            glBindBuffer(GL_ARRAY_BUFFER,food.vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*2, &food.position[0]);    
            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindVertexArray(0);

            food.boardIndex = mp[rn];
            unsigned int a = imp[cur->boardIndex];
            mp[a] = food.boardIndex;
            mp[rn] = cur->boardIndex;
            imp[food.boardIndex] = a;
            imp[cur->boardIndex] = rn;
            cur->boardIndex = newboardIndex;

            lengthchanged = true;
        }
        else {
            unsigned int a = imp[cur->boardIndex];
            unsigned int b = imp[newboardIndex];
            mp[a] = newboardIndex;
            mp[b] = cur->boardIndex;
            imp[newboardIndex] = a;
            imp[cur->boardIndex] = b;
            cur->boardIndex = newboardIndex;
        }*/

        time = 0.0f;
    }
}

void Game::render_game() {
    ResourceManager::resources["snake"]->UseShader();
    glBindVertexArray(snake.vao);
    glDrawArrays(GL_POINTS, 0, snake.length/2);

    ResourceManager::resources["food"]->UseShader();
    glBindVertexArray(food.vao);
    glDrawArrays(GL_POINTS, 0, 1);
}

Game::~Game() {
    grid.clear();

    glDeleteVertexArrays(1, &snake.vao);
    glDeleteBuffers(1, &snake.vbo);
    snake.positions.clear();
    snake_head = nullptr;
    while(snake_tail) {
        SnakeNode* temp = snake_tail;
        snake_tail = temp->next;
        delete temp;
    }

    glDeleteVertexArrays(1, &food.vao);
    glDeleteBuffers(1, &food.vbo);
}