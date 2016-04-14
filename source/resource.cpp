/**
 * resource.cpp
 *
 * @author:  Brian Clee
 * @version: 12/14/2015
 */

#include "resource.h"

std::map<std::string, Texture2D>    resource::Textures;
std::map<std::string, Shader>       resource::Shaders;

Shader resource::LoadShader(const GLchar *vFile, const GLchar *fFile, const GLchar *gFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vFile, fFile, gFile);
    return Shaders[name];
}

Shader resource::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D resource::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D resource::GetTexture(std::string name)
{
    return Textures[name];
}

void resource::Clear()
{
    for (auto iter : Shaders)
	{
        glDeleteProgram(iter.second.ID);
	}
    for (auto iter : Textures)
	{
        glDeleteTextures(1, &iter.second.ID);
	}
}

Shader resource::loadShaderFromFile(const GLchar *vFile, const GLchar *fFile, const GLchar *gFile)
{
    std::string vCode, fCode, gCode;
    try
    {
        std::ifstream vertexShaderFile(vFile);
        std::ifstream fragmentShaderFile(fFile);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vCode = vShaderStream.str();
        fCode = fShaderStream.str();
        if (gFile != nullptr)
        {
            std::ifstream geometryShaderFile(gFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            gCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vCode.c_str();
    const GLchar *fShaderCode = fCode.c_str();
    const GLchar *gShaderCode = gCode.c_str();
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D resource::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    texture.Generate(width, height, image);
    SOIL_free_image_data(image);
    return texture;
}