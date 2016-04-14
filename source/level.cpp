/**
 * level.cpp
 *
 * @author:  Brian Clee
 * @version: 12/14/2015
 */

#include "level.h"

#include <fstream>
#include <sstream>

void level::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	this->Bricks.clear();
	GLuint tileCode;
	level level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line)) 
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
		{
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}

void level::Draw(loader &renderer)
{
	for (GameObject &tile : this->Bricks)
	if (!tile.Destroyed)
	{
		tile.Draw(renderer);
	}
}

GLboolean level::IsCompleted()
{
	for (GameObject &tile : this->Bricks)
	if (!tile.IsSolid && !tile.Destroyed)
	{
		return GL_FALSE;
	}
	return GL_TRUE;
}

void level::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / height;
	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			if (tileData[y][x] == 1)
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, resource::GetTexture("solidblock"), glm::vec3(1.0f, 1.0f, 1.0f));
				obj.IsSolid = GL_TRUE;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)	
			{
				std::string name = "";

				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2)
				{
					name = "brickblock";
				}
				else if (tileData[y][x] == 3)
				{
					name = "questionblock";
				}
				else if (tileData[y][x] == 4)
				{
					name = "goldblock";
				}
				else if (tileData[y][x] == 5)
				{
					name = "brickblock";
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);

				this->Bricks.push_back(GameObject(pos, size, resource::GetTexture(name), color));
			}
		}
	}
}