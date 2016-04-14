/**
 * ball.cpp
 *
 * @author:  Brian Clee
 * @version: 12/14/2015
 */

#include "ball.h"

Ball::Ball() 
    : GameObject(), Radius(12.5f), Stuck(true), Sticky(false), PassThrough(false)  { }

Ball::Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    :  GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true), Sticky(false), PassThrough(false) { }

glm::vec2 Ball::Move(GLfloat dt, GLuint window_width)
{
    if (!this->Stuck)
    {
        this->Position += this->Velocity * dt;
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    return this->Position;
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Stuck = true;
	this->Sticky = false;
	this->PassThrough = false;
    this->Position = position;
    this->Velocity = velocity;
}