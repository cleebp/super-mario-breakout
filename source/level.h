#ifndef LEVEL_H
#define LEVEL_H
#include <vector>

#include "inc/glew/glew.h"
#include "inc/glm/glm.hpp"

#include "gameobject.h"
#include "loader.h"
#include "resource.h"

class level
{
public:
	std::vector<GameObject> Bricks;
	level() { }
	void      Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	void      Draw(loader &renderer);
	GLboolean IsCompleted();
private:
	void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif