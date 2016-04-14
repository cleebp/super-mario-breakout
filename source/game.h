#ifndef GAME_H
#define GAME_H

#include <vector>
#include <tuple>

#include "inc/glew/glew.h"
#include "inc/glfw/glfw3.h"

#include "gameobject.h"
#include "level.h"
#include "power.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
	GAME_OVER
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; 

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

class game
{
public:
	std::vector<level> Levels;
    GLuint             Level;
	GLuint             Lives;
	GLuint			   Score;
	
    GameState              state;	
    GLboolean              Keys[1024];
	GLboolean              KeysProcessed[1024];
    GLuint                 Width, Height;
	std::vector<PowerUp>   PowerUps;

    game(GLuint width, GLuint height);
    ~game();

    void init();
    void input(GLfloat dt);
    void update(GLfloat dt);
    void render();

	void collisions();
	void resetPlayer();
    void SpawnPowerUps(GameObject &block);
    void UpdatePowerUps(GLfloat dt);
};

#endif