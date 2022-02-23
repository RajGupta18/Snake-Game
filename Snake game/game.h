#ifndef GAME_H
#define GAME_H

#include<vector>

struct board {
    float x, y;
    bool vis;
    board() {}
    board(int X, int Y, bool v) : x(X), y(Y), vis(v) {}
};

const int direction[4][2] = {{0,1}, {0,-1}, {1,0}, {-1, 0}};  //right, left, down, up respectively

struct SnakeNode {
    unsigned int posIndex;
    unsigned int boardIndex;
    SnakeNode* next;
    SnakeNode() {}
    SnakeNode(int p, int b) : posIndex(p), boardIndex(b), next(nullptr) {} 
};

struct Snake {
    unsigned int vao, vbo;
    int length;
    std::vector<float> positions;
};

struct Food {
    unsigned int boardIndex;
    unsigned int vao, vbo;
    float position[2];
};

class Game
{
private:
    unsigned int step;
    float grid_size;
    float time;

    std::vector<std::vector<board>> grid;    //mark true if current board is occupied
    int directionIndex;                         //set movement direction (range: [0,3])

    Snake snake;
    SnakeNode* snake_head;
    SnakeNode* snake_tail;
    int pos[2]; //current posiion in index form

    //snake food
    Food food;

    //store visited and unvisited squares along with index map
    std::vector<unsigned int> mp, imp;
    int unvis_size;

public:
    Game();
    Game(const int &s);
    ~Game();

    void init_game();
    void ProcessInput(int key);
    void update_game(float &deltatime);
    void render_game();
};
#endif