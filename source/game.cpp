/**
 * game.cpp
 *
 * @author:  Brian Clee
 * @version: 12/14/2015
 */

#include <algorithm>
#include <sstream>

#include "inc/irrklang/irrKlang.h"
using namespace irrklang;

#include "game.h"
#include "resource.h"
#include "loader.h"
#include "gameobject.h"
#include "ball.h"
#include "text.h"

loader *renderer;
GameObject *Player;
Ball *ball;
ISoundEngine *SoundEngine = createIrrKlangDevice();
text *Text;

game::game(GLuint width, GLuint height) 
	: state(GAME_MENU), Keys(), Width(width), Height(height), Level(0), Lives(3), Score(0) { }

game::~game()
{
	delete Player;
	delete ball;
	delete Text;
	delete renderer;
	SoundEngine->drop();
}

void game::init()
{
    resource::LoadShader("shaders/vs.glsl", "shaders/fs.glsl", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

    resource::GetShader("sprite").Use().SetInteger("sprite", 0);
    resource::GetShader("sprite").SetMatrix4("projection", projection);

	// LOAD ALL THE THINGS!!!!
	// Imagine doing this without other classes, sheesh...
    resource::LoadTexture("models/background2.jpg", GL_FALSE, "background");
	resource::LoadTexture("models/background_menu.jpg", GL_FALSE, "background_menu");
    resource::LoadTexture("models/chain_chomp.png", GL_TRUE, "ball");
	resource::LoadTexture("models/brickblock.png", GL_FALSE, "brickblock");
	resource::LoadTexture("models/questionblock.png", GL_FALSE, "questionblock");
	resource::LoadTexture("models/goldblock.png", GL_FALSE, "goldblock");
	resource::LoadTexture("models/solidblock.png", GL_FALSE, "solidblock");
    resource::LoadTexture("models/pipe.png", GL_TRUE, "paddle");
	resource::LoadTexture("models/mushroom.png", GL_TRUE, "powerup_speed");
	resource::LoadTexture("models/mushroom.png", GL_TRUE, "powerup_sticky");
	resource::LoadTexture("models/mushroom.png", GL_TRUE, "powerup_passthrough");
	resource::LoadTexture("models/mushroom.png", GL_TRUE, "powerup_increase");

    renderer = new loader(resource::GetShader("sprite"));
	Text = new text(this->Width, this->Height);
    Text->Load("inc/liberation.ttf", 24);

	// Load 4 levels
    level one; one.Load("levels/1-1.txt", this->Width, this->Height * 0.5);
    level two; two.Load("levels/1-2.txt", this->Width, this->Height * 0.5);
    level three; three.Load("levels/1-3.txt", this->Width, this->Height * 0.5);
    level four; four.Load("levels/1-4.txt", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, resource::GetTexture("paddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    ball = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, resource::GetTexture("ball"));

    SoundEngine->play2D("audio/menu.mp3", GL_TRUE);
}

void game::update(GLfloat dt)
{
    ball->Move(dt, this->Width);
    this->collisions();
	this->UpdatePowerUps(dt);
    if (ball->Position.y >= this->Height)
    {
        --this->Lives;
        if (this->Lives == 0)
        {
			this->state = GAME_OVER;
			SoundEngine->stopAllSounds();
			SoundEngine->play2D("audio/gameover.mp3", GL_FALSE);
        }
        this->resetPlayer();
    }
    if (this->state == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        this->resetPlayer();
        this->state = GAME_WIN;
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("audio/course-clear.wav", GL_FALSE);
    }
}

void game::input(GLfloat dt)
{
	if (this->state == GAME_MENU)
    {
		if ((this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) || (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]))
        {
            this->state = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;

			this->Lives = 3;
			this->Score = 0;

			SoundEngine->stopAllSounds();
			SoundEngine->play2D("audio/here.wav", GL_FALSE);
			SoundEngine->play2D("audio/game.mp3", GL_TRUE);

			if (this->Level == 0)
				this->Levels[0].Load("levels/1-1.txt", this->Width, this->Height * 0.5f);
			else if (this->Level == 1)
				this->Levels[1].Load("levels/1-2.txt", this->Width, this->Height * 0.5f);
			else if (this->Level == 2)
				this->Levels[2].Load("levels/1-3.txt", this->Width, this->Height * 0.5f);
			else if (this->Level == 3)
				this->Levels[3].Load("levels/1-4.txt", this->Width, this->Height * 0.5f);
        }
        if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
        {
            this->Level = (this->Level + 1) % 4;
            this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
        }
        if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
        {
            if (this->Level > 0)
                --this->Level;
            else
                this->Level = 3;
            this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
        }
    }
    if (this->state == GAME_WIN)
    {
		if (this->Keys[GLFW_KEY_ENTER] || this->Keys[GLFW_KEY_SPACE])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
			SoundEngine->stopAllSounds();
			SoundEngine->play2D("audio/game.mp3", GL_TRUE);
			this->state = GAME_ACTIVE;
			this->Level = (this->Level + 1) % 4;
        }
    }
	if (this->state == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT])
        {
            if (Player->Position.x >= 0)
			{
                Player->Position.x -= velocity;
				if (ball->Stuck)
                    ball->Position.x -= velocity;
			}
        }
        if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
			{
                Player->Position.x += velocity;
				if (ball->Stuck)
                    ball->Position.x += velocity;
			}
        }
		if (this->Keys[GLFW_KEY_SPACE] || this->Keys[GLFW_KEY_ENTER])
            ball->Stuck = false;
    }
	if (this->state == GAME_OVER)
	{
		if ((this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) || (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]))
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
			this->Level = 0;
			this->state = GAME_MENU;
			SoundEngine->stopAllSounds();
			SoundEngine->play2D("audio/menu.mp3", GL_TRUE);
		}
	}
}

void game::render()
{
	if (this->state == GAME_ACTIVE || this->state == GAME_WIN)
    {
        renderer->DrawSprite(resource::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        this->Levels[this->Level].Draw(*renderer);
        Player->Draw(*renderer);
        for (PowerUp &powerUp : this->PowerUps)
		{
			if (!powerUp.Destroyed)
			{
				powerUp.Draw(*renderer);
			}
		}
        ball->Draw(*renderer);
		std::stringstream ss; ss << this->Lives;
		std::stringstream scs; scs << this->Score;
		std::stringstream ls; ls << this->Level;
		Text->RenderText("Lives:" + ss.str() + " Score: " + scs.str(), 5.0f, 5.0f, 1.0f);
		Text->RenderText("Current Level: " + ls.str(), this->Width-240.0f, 5.0f, 1.0f);
    }
	if (this->state == GAME_MENU)
    {
		renderer->DrawSprite(resource::GetTexture("background_menu"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		Text->RenderText("======= Controls =======", 230.0f, this->Height / 2 + 20, 1.0f);
		Text->RenderText("SPACE/ENTER | Start game", 230.0f, this->Height / 2 + 40, 1.0f);
		Text->RenderText("LEFT/A      | Move left", 230.0f, this->Height / 2 + 60, 1.0f);
		Text->RenderText("RIGHT/D     | Move right", 230.0f, this->Height / 2 + 80, 1.0f);
		Text->RenderText("ESC         | Quit game", 230.0f, this->Height / 2 + 100, 1.0f);
    }
    if (this->state == GAME_WIN)
    {
		std::stringstream ls; ls << this->Level;
        Text->RenderText("Congrats you beat level " + ls.str(), 200.0f, this->Height / 2 - 20.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        Text->RenderText("Press Enter/Space to proceed", 200.0f, this->Height / 2, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
	if (this->state == GAME_OVER)
	{
		renderer->DrawSprite(resource::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);

		std::stringstream scs; scs << this->Score;
		std::stringstream ls; ls << this->Level;

		Text->RenderText("GAME OVER", 350.0f, this->Height / 2 - 100, 1.0f);
		Text->RenderText("Your final score was " + scs.str(), 250.0f, this->Height / 2 + 20, 1.0f);
		Text->RenderText("You made it to level " + ls.str(), 250.0f, this->Height / 2, 1.0f);
		Text->RenderText("Press Enter/Space to return to the main menu", 100.0f, this->Height / 2 + 90, 1.0f);
	}
}

void game::resetPlayer()
{
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
	
	ball->Sticky = GL_FALSE;
	ball->PassThrough = GL_FALSE;
}

GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void game::UpdatePowerUps(GLfloat dt)
{
    for (PowerUp &powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;

            if (powerUp.Duration <= 0.0f)
            {
                powerUp.Activated = GL_FALSE;
                if (powerUp.Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {	
                        ball->Sticky = GL_FALSE;
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	
                        ball->PassThrough = GL_FALSE;
                    }
                }
            }
        }
    }

    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(), [](const PowerUp &powerUp) 
	{ 
		return powerUp.Destroyed && !powerUp.Activated; 
	}
    ), this->PowerUps.end());
}

GLboolean ShouldSpawn(GLuint chance)
{
    GLuint random = rand() % chance;
    return random == 0;
}

void game::SpawnPowerUps(GameObject &block)
{	
    if (ShouldSpawn(25))
	{
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(1.0f), 0.0f, block.Position, resource::GetTexture("powerup_speed")));
	}
    else if (ShouldSpawn(25))
	{
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f), 20.0f, block.Position, resource::GetTexture("powerup_sticky")));
	}
    else if (ShouldSpawn(25))
	{
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(1.0f), 10.0f, block.Position, resource::GetTexture("powerup_passthrough")));
	}
    else if (ShouldSpawn(25))
	{
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f), 0.0f, block.Position, resource::GetTexture("powerup_increase")));
	}
}

void ActivatePowerUp(PowerUp &powerUp)
{
    if (powerUp.Type == "speed")
    {
        ball->Velocity *= 2.0;
    }
    else if (powerUp.Type == "sticky")
    {
        ball->Sticky = GL_TRUE;
    }
    else if (powerUp.Type == "pass-through")
    {
        ball->PassThrough = GL_TRUE;
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        Player->Size.x += 50;
    }
}

GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.Activated)
		{
            if (powerUp.Type == type)
			{
                return GL_TRUE;
			}
		}
    }
    return GL_FALSE;
}

GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(Ball &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);

void game::collisions()
{
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*ball, box);
            if (std::get<0>(collision)) 
            {
                if (!box.IsSolid)
				{
                    box.Destroyed = GL_TRUE;
					SoundEngine->play2D("audio/coin.wav", GL_FALSE);
					this->SpawnPowerUps(box);
					this->Score += 200;
				}
				else
				{
					SoundEngine->play2D("audio/bump.wav", GL_FALSE);
				}
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(ball->PassThrough && !box.IsSolid)) 
                {
                    if (dir == LEFT || dir == RIGHT) 
                    {
                        ball->Velocity.x = -ball->Velocity.x; 
                        GLfloat penetration = ball->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
						{
                            ball->Position.x += penetration;
						}
                        else
						{
                            ball->Position.x -= penetration;
						}
                    }
                    else 
                    {
                        ball->Velocity.y = -ball->Velocity.y;
                        GLfloat penetration = ball->Radius - std::abs(diff_vector.y);
                        if (dir == UP)
						{
                            ball->Position.y -= penetration; 
						}
                        else
						{
                            ball->Position.y += penetration; 
						}
                    }
                }
            }
        }    
    }

    for (PowerUp &powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            if (powerUp.Position.y >= this->Height)
			{
                powerUp.Destroyed = GL_TRUE;
			}

            if (CheckCollision(*Player, powerUp))
            {	
				SoundEngine->play2D("audio/power.wav", GL_FALSE);
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = GL_TRUE;
                powerUp.Activated = GL_TRUE;
            }
        }
    }

    Collision result = CheckCollision(*ball, *Player);
    if (!ball->Stuck && std::get<0>(result))
    {
		SoundEngine->play2D("audio/kick.wav", GL_FALSE);
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (ball->Position.x + ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = ball->Velocity;
        ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity);
        ball->Velocity.y = -1 * abs(ball->Velocity.y);

		ball->Stuck = ball->Sticky;
    }
}

GLboolean CheckCollision(GameObject &one, GameObject &two) 
{
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
    return collisionX && collisionY;
}

Collision CheckCollision(Ball &one, GameObject &two) 
{
    glm::vec2 center(one.Position + one.Radius);
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;
    
    if (glm::length(difference) < one.Radius) 
	{
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	}
    else
	{
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
	}
}

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	
        glm::vec2(1.0f, 0.0f),	
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)	
    };

	if(target.x == 0.0f && target.y == 0.0f)
	{
		return (Direction)0;
	}

    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}   