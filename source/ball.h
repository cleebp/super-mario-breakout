#ifndef BALL_H
#define BALL_H

#include "inc/glew/glew.h"
#include "inc/glm/glm.hpp"

#include "texture.h"
#include "loader.h"
#include "gameobject.h"
#include "power.h"

class Ball : public GameObject
{
public:	
    GLfloat   Radius;
    GLboolean Stuck;
	GLboolean Sticky, PassThrough;
    Ball();
    Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void      Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif