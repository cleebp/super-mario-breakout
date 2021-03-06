#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>


#include "inc/glew/glew.h"
#include "inc/glm/glm.hpp"

#include "texture.h"
#include "shader.h"

struct Character {
    GLuint TextureID;   
    glm::ivec2 Size;    
    glm::ivec2 Bearing; 
    GLuint Advance;     
};

class text
{
public:
    std::map<GLchar, Character> Characters; 
    Shader TextShader;
    text(GLuint width, GLuint height);
    void Load(std::string font, GLuint fontSize);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
    GLuint VAO, VBO;
};

#endif 