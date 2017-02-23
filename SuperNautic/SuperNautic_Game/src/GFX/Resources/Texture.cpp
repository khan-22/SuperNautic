#include "GFX/Resources/Texture.hpp"

#include "Core/Io/Log.hpp"

using namespace GFX;

Texture::Texture(const unsigned char* data, unsigned int width, unsigned int height, GLenum target)
: _width(width)
, _height(height)
, _target(target)
{
    glGenTextures(1, &_textureId);

    bind(0);

    initialize(data);

    unbind(0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_textureId);
}



void Texture::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(_target, _textureId);
}

void Texture::unbind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(_target, 0);
}

unsigned int Texture::getWidth() const
{
    return _width;
}

unsigned int Texture::getHeight() const
{
    return _height;
}

void Texture::initialize(const unsigned char* data) const
{
    switch(_target)
    {
    case GL_TEXTURE_2D:
        initialize2d(data);
        break;
    case GL_TEXTURE_CUBE_MAP:
        initializeCubeMap(data);
        break;

    default:
        LOG_ERROR("Unimplemented texture target: ", _target);
        break;
    }
}

void Texture::initialize2d(const unsigned char* data) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::initializeCubeMap(const unsigned char* data) const
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if(data == nullptr)
    {
        for(unsigned int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        }
    }
    else
    {
        for(unsigned int i = 0; i < 6; i++)
        {
            const size_t INDEX = i * _width * _height * 4;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data + INDEX);
        }
    }
}
